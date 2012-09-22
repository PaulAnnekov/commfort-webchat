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


#pragma hdrstop

#include <StrUtils.hpp>
#include "http_listen_thread.h"
#include <process.h>
#include "json_reader.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
HANDLE HTTPThread;
//volatile long http_listen_stop;

SOCKET http_listening_socket;
Socket�onnections http_connections;

unsigned __stdcall HTTPListenLoop(void* pArguments) {
	try {
		SocketAccept(http_listening_socket, http_connections, GetHTTPMessageLength, false);
		SocketClose(http_listening_socket);
	} __finally {
		_endthreadex(0);
	}

	return 0;
}

bool HTTPThreadStart() {
	//http_listen_stop = 0;
    bool success = true;

	try {
		http_listening_socket = SocketListen(9752);
		HTTPThread = (HANDLE)_beginthreadex(NULL, 0, &HTTPListenLoop, NULL, 0, NULL);
    	if (HTTPThread == 0) {
			throw Exception(__("ThreadMemoryError"));
		}
	} catch (Exception *E) {
		success = false;
		InfoAction(__("HTTPThreadStartError") + ". " + E->Message, false, 3, 2);
	}

	return success;
}

// Get HTTP header value. Check value for comma-separated values, if multi_values_check is true.
String GetHTTPHeaderValue(String header, String field, bool multi_values_check) {
	String value;
	unsigned int field_pos = header.Pos(field + ":");

	if (field_pos > 0) {
		unsigned int field_offset = field_pos + field.Length() + 1,
					 value_end_pos = PosEx("\r\n", header, field_offset);
		if (value_end_pos > 0) {
			value = header.SubString(field_offset, value_end_pos - field_offset);
			if (multi_values_check) {
				unsigned int comma_pos = value.Pos(",");
				if (comma_pos > 0) {
					value = value.SubString(0, comma_pos - 1);
				}
			}
			value = value.TrimLeft();
		}
	}

	return value;
}

int GetHTTPContentLength(char *header, int length) {
	char *len_str = NULL;
	int offset = 0, already_read = 0, output_length = -1;
	bool found = false;

	//14 - parameter length, 2 - new line length, 2 - ": " length, 2 - minimum length of json request.
	while (already_read <= length - 20) {
		if (!strncmp(header, "Content-Length", 14)) {
			found = true;
			header += 14;
			already_read += 14;
			if (!strncmp(header, ": ", 2)) {
				header += 2;
				already_read += 2;
				// offset <= 5 - maxlength is 99999 - 5 digits.
				while (already_read <= length - 2 && offset <= 5) {
					if (!strncmp(header + offset, "\r", 1)) {
						len_str = new char[offset + 1];
						memcpy(len_str, header, offset);
						memcpy(len_str + offset, "\0", 1);
						break;
					} else {
						offset++;
						already_read++;
					}
				}
				if (!len_str) {
                	//error. Header parameter end not found.
					break;
				}
			} else {
				//error. Bad header parameter format.
				break;
            }
		} else {
			header++;
			already_read++;
        }
	}

	if (!found) {
		output_length = 0;
	} else {
		try {
			output_length = atoi(len_str);
			if (output_length == 0 || output_length == INT_MAX || output_length == INT_MIN) {
				output_length = 0;
			}
		} catch (...) {
			output_length = 0;
		}
	}

	return output_length;
}

int GetHTTPMessageLength(Socket�onnection &conn, char *received_data, int len) {
	// ������ �� ������� ������ � ������� ������, ������ ���������� �� ����� ������ � ������������ ��������� ������
	// ���������, ������������ ������� ������ ���� �������� � ������ ������.
	int read_length = 2000 - conn.in_buf_length;
	if (len < read_length) {
		read_length = len;
	}

	// ������� ����� ����� � �������� � ���� �� ����������� ������ + ������, ������� ������ �� �����.
	char *new_buffer = new char[conn.in_buf_length + read_length];
	if (conn.in_buf_length != 0) {
		memcpy(new_buffer, conn.in_buffer, conn.in_buf_length);
	}
	memcpy(new_buffer + conn.in_buf_length, received_data, read_length);

	// ���� ������ ������ ������ ������ 4 ��������, �� ����� ����� ��������� ����� ���������.
	if (conn.in_buf_length + read_length >= 4) {
		// �������� ��������� �� ����� ����� ��� �������� �� ����.
		char *tmp = new_buffer;

		// ��������� ����������� ��������� ���������� ������������ �� ����� ����� ������.
		int loop = read_length;

		int offset = 0;

		// ���� ������ ������� ������ ���� ������ 3-�� ��������, �� �������� � ��� ��������� ���� ��������
		// ����������� ��������� "\r\n\r", ������� ����� ���� ������� ����� ���������. ������������� ����
		// �������� ����� � ������ ���� 3-�� ��������.
		if (conn.in_buf_length >= 3) {
			offset = conn.in_buf_length - 3;
			tmp += offset;
			loop += 3;
		}

		int read = 0;

		// ���� ���������� ����������� �������� ������ ��� ���� ���������.
		while (read < loop) {
			// ���� ������ ������ � "\r".
			if (!strncmp(tmp, "\r", 1)) {
				// ���� 4 ������� ������� � "\r\n\r\n".
				if (!strncmp(tmp, "\r\n\r\n", 4)) {
					// ���������� ��������� �� ��������� ������� �������� ���������.
					String header = String(new_buffer);
					header.SetLength(offset + read + 2);
					conn.json_length = GetHTTPHeaderValue(header, "Content-Length", false).ToInt();
					conn.json_length_received = true;

                    // Get client ip, if proxy detected.
					String new_ip = GetHTTPHeaderValue(header, "X-Forwarded-For", true);
					if (ValidateIP(new_ip) && conn.ip != new_ip) {
						conn.ip = new_ip;
					}

					// ���� ������ ����� ����������� - ������� ���.
//					if (conn.length > 0) {
//						delete [] conn.buffer;
//					}

					// ���������� ������, ������� �� ��� ������� � ����� �����.
//					conn.length = (conn.length + read_length) - (offset + read + 4);

					// ���� ������ ������������ ������ ������ ��� ����� ������ ��������, ������� ���� �������,
					// �� �� �������� �������.
//					if (conn.length >= conn.content_length) {
//						conn.length = conn.content_length;
//						content_received = true;
//					}

					// ���������� � ������ ����� ������ � ������� ��������� ������.
//					conn.buffer = new char[conn.content_length];
//					memcpy(conn.buffer, new_buffer + offset + read + 4, conn.buffer_length);
					read_length = read + 4;

					delete [] new_buffer;

					//conn.headers_received = true;
					break;
				} else {
					// ��������� �� 4 �������.
					tmp += 4;
					read += 4;
				}
			} else {
				// ��������� �� 1 ������.
				tmp++;
				read++;
			}
		}
	}

	// ����  �� �� ����� ����� ���������, �� � ������ ��������� ��� ����� ��� ��������� 2000 ��������.
	if (!conn.json_length_received) {
		//if (conn.length >= 2000) {
		//	delete [] conn.buffer;
			//send error.
			//result = false;
		//} else {
			// ���� ��������� ��� �� �������, �� ���� ����������� ��������� ����� � ����� ����������.
			conn.in_buffer = new_buffer;
			conn.in_buf_length += read_length;
		//}
	} else {
		delete [] conn.in_buffer;
		conn.in_buffer = NULL;
		conn.in_buf_length = 0;
    }

	return read_length;
}
