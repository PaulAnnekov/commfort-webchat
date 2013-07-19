/*
	CommFort WebChat 5 Plugin
	Copyright (C) 2012  Paul Annekov (global@steelrat.info)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "socket_utils.h"
#include "notif.h"
#include "l18n.h"
#include "debug.h"
#include "socket_listen_thread.h"
#include "http_listen_thread.h"
#include "connections.h"
#include "json_reader.h"
#include "connections_form.h"
#include <IdURI.hpp>
#pragma package(smart_init)

volatile long listen_stop;

String GetSocketError(SOCKET *sock) {
	String error = NULL;
	int err;

	if (sock != NULL) {
		int errlen = sizeof(err);
		getsockopt(*sock, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
	} else {
		err = WSAGetLastError();
	}

	if (err != NO_ERROR) {
		error = FormatError(err);
	}

	return error;
}

// Set up the three FD sets used with select() with the sockets in the
// connection list.  Also add one for the listener socket, if we have
// one.
void SetupFDSet(fd_set& ReadFDs, SocketСonnections &connections) {
	// Add client connections
	SocketСonnections::iterator it = connections.begin();
	while (it != connections.end()) {
		FD_SET(it->sd, &ReadFDs);

		++it;
	}
}

int SocketRead(SOCKET sd, char **data) {
	int len = 0;
	ioctlsocket(sd, FIONREAD, (DWORD *) &len);

	// Если длинна данных в буфере - 0, значит сокет закрылся.
	if (len != 0) {
		*data = new char[len];
		len = recv(sd, *data, len, 0);
		if (len == SOCKET_ERROR) {
			if (*data) {
				delete [] *data;
			}
			// Something bad happened on the socket.  It could just be a
			// "would block" notification, or it could be something more
			// serious.  Let caller handle the latter case.  WSAEWOULDBLOCK
			// can happen after select() says a socket is readable under
			// Win9x: it doesn't happen on WinNT/2000 or on Unix.
			SocketWarn(Format(e_tcp_read, ARRAYOFCONST((GetSocketError(&sd)))));
		}
	}

	return len;
}

// Copies new portion of the received data to the end of json request string.
// Launches parser if json request is fully received.
String GetSocketMessage(SocketСonnection &conn, char *data, int len) {
	int copy_to_end = conn.json_length - conn.in_buf_length;
	bool content_received = false;
	String request = "";

	if (len < copy_to_end) {
		copy_to_end = len;
	} else {
		content_received = true;
	}

	char *tmp = new char[conn.in_buf_length + copy_to_end];
	memcpy(tmp, conn.in_buffer, conn.in_buf_length);
	memcpy(tmp + conn.in_buf_length, data, copy_to_end);
	delete [] conn.in_buffer;
	conn.in_buffer = NULL;

	if (content_received) {
		request = String(tmp).SetLength(conn.json_length);
		conn.in_buf_length = 0;
		conn.json_length_received = false;
		conn.json_length = 0;
		delete [] tmp;
	} else {
		conn.in_buffer = tmp;
		conn.in_buf_length += copy_to_end;
	}

	return request;
}

// The connection is writable, so send any pending data.  Returns
// false on failure.  (WSAEWOULDBLOCK doesn't count as a failure.)
void SocketWrite(SOCKET sd, String response, bool header_needed) {
	RawByteString enc_resp = UTF8Encode(response);
	int sended = 0, nBytes = 0;
	long json_length = enc_resp.Length();

	// Permanently set socket as blocking.
	u_long nNoBlock = 0;
	ioctlsocket(sd, FIONBIO, &nNoBlock);
	if (header_needed) {
		RawByteString header = UTF8Encode("HTTP/1.1 200 OK\r\nContent-Length: " + String(enc_resp.Length()) + "\r\nContent-Type: application/json\r\n\r\n");
		send(sd, header.c_str(), header.Length(), 0);
	} else {
		send(sd, (char *) &json_length, 4, 0);
	}

	do {
		sended = send(sd, enc_resp.c_str() + nBytes, json_length - nBytes, 0);
		nBytes += sended;
		if (sended == SOCKET_ERROR) {
			// Something bad happened on the socket.  Deal with it.
			SocketWarn(Format(e_tcp_write, ARRAYOFCONST((GetSocketError(NULL)))));
			break;
		}
	} while (nBytes < json_length);
	// Set socket as non blocking.
	nNoBlock = 1;
	ioctlsocket(sd, FIONBIO, &nNoBlock);
}

// Validate dotted representation of IP address.
bool ValidateIP(String ip) {
	bool valid = false;

	if (!ip.IsEmpty()) {
		unsigned long binary_ip = inet_addr(AnsiString(ip).c_str());
		if (binary_ip != INADDR_NONE && binary_ip != INADDR_ANY) {
        	valid = true;
		}
	}


	return valid;
}

// Get socket ip or empty string on error.
String GetSocketIP(SOCKET sd) {
	char *ip = NULL;
	sockaddr_in addr;

	int addr_size = sizeof(addr);
	if (SOCKET_ERROR != (getpeername(sd, (sockaddr*)&addr, &addr_size))) {
		ip = inet_ntoa(addr.sin_addr);
	}

	return (ip != NULL) ? String(ip) : String("");
}

// Search socket with the same ip as in parameter.
bool SearchSocketByIP(SocketСonnections &connections, String ip) {
	bool found = false;
	SocketСonnections::iterator it = connections.begin();

	while (it != connections.end()) {
		if (it->ip == ip) {
			found = true;
			break;
		}
		it++;
	}

	return found;
}

// Close socket.
void SocketClose(SOCKET sd) {
	int res = closesocket(sd);
	if (res == SOCKET_ERROR) {
		SocketWarn(Format(e_tcp_socket_close, ARRAYOFCONST((GetSocketError(NULL)))));
	}
}

// Spin forever handling connections.  If something bad happens, we return.
void SocketAccept(SOCKET listening_sock, SocketСonnections &connections, int (*GetSocketMessageLength)(SocketСonnection &, char *, int), bool is_persistent) {
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);
	int socket_result;
	timeval interval;
	interval.tv_sec = 1;
	interval.tv_usec = 0;
	fd_set fds, read_fds_copy, except_fds_copy;

    FD_ZERO(&fds);

	// Add the listener socket to the read and except FD sets, if there is one.
	if (listening_sock != INVALID_SOCKET) {
		FD_SET(listening_sock, &fds);
	}

	read_fds_copy = except_fds_copy = fds;

	while (1) {
		if (listen_stop) {
            /* TODO : Освобождать память всех буферов всех соединений при отключении прослушки портов */
			break;
		}

		// Check thread sockets for new events.
		socket_result = select(0, &read_fds_copy, NULL, &except_fds_copy, &interval);
		if (socket_result != SOCKET_ERROR && socket_result != 0) {
			// Проверка прослушивающего сокета на подключение нового клиента и на ошибки.
			if (FD_ISSET(listening_sock, &read_fds_copy)) {
				SOCKET sd = accept(listening_sock,
						(sockaddr*)&sinRemote, &nAddrSize);

				if (sd != INVALID_SOCKET) {
					char *ip = inet_ntoa(sinRemote.sin_addr);
					if (ip != NULL) {
						// Check if socket with the same IP address is already exists (don't allow double sockets from one ip).
						String str_ip = String(ip);
						// Set socket as non blocking.
						u_long nNoBlock = 1;
						ioctlsocket(sd, FIONBIO, &nNoBlock);
						// Add socket to
						connections.push_back(SocketСonnection(sd, str_ip));
						FD_SET(sd, &fds);
					}
				} else {
					SocketWarn(Format(e_tcp_socket_accept, ARRAYOFCONST((GetSocketError(NULL)))));
				}
			} else if (FD_ISSET(listening_sock, &except_fds_copy)) {
				SocketWarn(Format(e_tcp_during_listen, ARRAYOFCONST((GetSocketError(&listening_sock)))));
			}

            // Проверка клиентских сокетов на события.
			SocketСonnections::iterator it = connections.begin();
			SocketСonnections::iterator next_it = it;
			while (it != connections.end()) {
				bool is_success = true;
				String err;
                FILETIME now;

				GetSystemTimeAsFileTime(&now);
				// Проверяем установлен ли указатель на данный сокет в массиве сокетов с событиями.
				if (FD_ISSET(it->sd, &except_fds_copy)) {
					is_success = false;
					err = Format(e_tcp_client_during_listen, ARRAYOFCONST((GetSocketError(&(it->sd)))));
					FD_CLR(it->sd, &except_fds_copy);
				} else if (FD_ISSET(it->sd, &read_fds_copy)) {
					char *data = NULL;
					int len = SocketRead(it->sd, &data);
					if (len == 0) {
						is_success = false;
					} else if (len > 0) {
						int copied;
						if (!it->json_length_received) {
							copied = (*GetSocketMessageLength)(*it, data, len);

							len = ShiftBytes(&data, len, copied);

							if (it->json_length_received) {
								delete [] it->in_buffer;
								it->in_buffer = NULL;
								it->in_buf_length = 0;
							}
						}

						if (it->json_length_received) {
							if (it->json_length == 0) {
								it->json_length_received = false;
								is_success = false;
                            // Check if anything left in data variable to add to the json string.
							} else if (len > 0) {

								String request = GetSocketMessage(*it, data, len);
								if (!request.IsEmpty()) {
									// Если длинна данных меньше 45 символов, то выбиваем ошибку (минимальный запрос состоит из 45 символов).
									if (request.Length() > 45) {
										String response = ParseJSONRequest(request, it, !is_persistent);
										if (!response.IsEmpty()) {
											// !is_persistent detect the protocol type - HTTP or Sockets.
											SocketWrite(it->sd, response, !is_persistent);
										}
									} else {
										is_success = false;
									}
								}
							}
						}

						delete [] data;
					}

					err = GetSocketError(&(it->sd));
					if (err != NULL) {
						SocketWarn(Format(e_tcp_client_during_read, ARRAYOFCONST((err))));
					}
					FD_CLR(it->sd, &read_fds_copy);
				}

				// Set the pointer to the next iterator. So if next block will delete
				// current iterator, then the cycle will know which is the next iterator.
				next_it = ++it;
				it--;

				// Если что-то случилось с сокетом или клиент отключился, то закрываем сокет и удаляем его из списка.
				if (!is_success) {
					// Find logical connection with such socket.
					LogicalConnections::iterator connection = SearchBySocket(it->sd);
					// If connection exists and it's persistent (mobile clients) then delete it. Else - unset connection socket.
					if (ConnectionExists(connection)) {
						(is_persistent) ? DeleteConnection(connection) : SetConnectionSocket(connection, NULL);
					}

                    FD_CLR(it->sd, &fds);

					SocketClose(it->sd);

					connections.erase(it);
				}

				// Переходим на следующий сокет.
				it = next_it;
			}
		} else if (socket_result == SOCKET_ERROR) {
			throw(Format(e_tcp_connections_check, ARRAYOFCONST((GetSocketError(NULL)))));
		}

		read_fds_copy = except_fds_copy = fds;
	}
}

// Запуск прослушки порта.
SOCKET SocketListen(int port) {
	SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd != INVALID_SOCKET) {
		sockaddr_in sinInterface;
		sinInterface.sin_family = AF_INET;
		sinInterface.sin_addr.s_addr = inet_addr("0.0.0.0");
		sinInterface.sin_port = htons(port);
		if (bind(sd, (sockaddr*)&sinInterface, sizeof(sockaddr_in)) != SOCKET_ERROR) {
			if (listen(sd, SOMAXCONN) == SOCKET_ERROR) {
				throw Exception(Format(__("ListenStartError"), ARRAYOFCONST((GetSocketError(NULL)))));
			};
		} else {
			throw Exception(Format(__("PortBindError"), ARRAYOFCONST((GetSocketError(NULL)))));
		}
	} else {
		throw Exception(Format(__("ConnectionCreateError"), ARRAYOFCONST((GetSocketError(NULL)))));
	}

	return sd;
}
