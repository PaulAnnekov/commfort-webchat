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

#include "connections.h"
#include "global.h"
#include "json_main.h"
#include "http_listen_thread.h"
#include "notif.h"
#include "l18n.h"
#include "cf_functions.h"
#include "channels_watch.h"
#include "user_channel.h"
#include <time.h>
#include "connections_form.h"
#include "main_form.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
LogicalConnections logical_connections;

// Генерирует случайный хеш для идентификатора соединения.
String RandomHashGen() {
	int rand_num;
	String conn_id;
	bool is_unique = false;

	while (!is_unique) {
		/* initialize random seed: */
		srand(time(NULL));

		/* generate secret number: */
		rand_num = rand() % 8998 + 1001;
		conn_id = GetMD5(rand_num);

		if (SearchConnection(conn_id) == logical_connections.end()) {
			is_unique = true;
		}
  }

  return conn_id;
}

// Добавляет соединение.
// Возвращает идентификатор созданного соединения.
String AddConnection(String app_name, String ip, SOCKET sock, bool auto_actions_get, unsigned int transport) {
	String conn_id = RandomHashGen();

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		logical_connections.push_back(LogicalConnection(conn_id, app_name, time(NULL), ip, sock, auto_actions_get, transport));
		LogicalConnection conn = logical_connections.back();
		if (conn_list_form) {
			SendMessage(conn_list_form->Handle, CL_DO_ACTION, 0, (LPARAM)&conn);
		}

		if (main_form) {
			PostMessage(main_form->Handle, MF_DO_ACTION, 0, logical_connections.size());
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't add new connection: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't add new connection", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return conn_id;
}

// Ищет соединение по идентификатору.
// Возвращает итератор.
LogicalConnections::iterator SearchConnection(String conn_id) {
	LogicalConnections::iterator res;
	bool is_found = false;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		res = logical_connections.end();
		if (!logical_connections.empty()) {
			for (res = logical_connections.begin(); res != logical_connections.end(); res++) {
				if (res->conn_id == conn_id) {
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				res = logical_connections.end();
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't search connection by id: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't search connection by id", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return res;
}

// Ищет соединение по сокету.
// Возвращает итератор.
LogicalConnections::iterator SearchBySocket(SOCKET &sock) {
	LogicalConnections::iterator res;
	bool is_found = false;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		res = logical_connections.end();
		if (!logical_connections.empty()) {
			for (res = logical_connections.begin(); res != logical_connections.end(); res++) {
				if (res->sock == sock) {
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				res = logical_connections.end();
			}
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't search connection socket: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't search connection socket", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return res;
}

// Search connection by IP.
// Return iterator.
LogicalConnections::iterator SearchByIP(String ip) {
	LogicalConnections::iterator res;
	bool is_found = false;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		res = logical_connections.end();

		if (!logical_connections.empty()) {
			for (res = logical_connections.begin(); res != logical_connections.end(); res++) {
				if (res->ip == ip) {
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				res = logical_connections.end();
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't search connection by ip: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't search connection by ip", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return res;
}

// Ищет соединение по имени виртуального пользователя.
// Возвращает итератор.
LogicalConnections::iterator FindConnectionByUser(String user) {
	LogicalConnections::iterator result, connection;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		result = logical_connections.end();
		if (!logical_connections.empty()) {
			for (connection = logical_connections.begin(); connection != logical_connections.end(); connection++) {
				if (GetOnlineUserName(connection->user) == user) {
					result = connection;
					break;
				}
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't search connection by user name: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't search connection by user name", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return result;
}

// Get vector of connections whose virtual users is connected to channel.
LogicalConnectionsRef GetChannelConnections(String channel) {
	LogicalConnectionsRef connections;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (!logical_connections.empty()) {
            LogicalConnections::iterator connection;
			for (connection = logical_connections.begin(); connection != logical_connections.end(); connection++) {
				if (IsOnlineUserSet(connection->user) && IsUserInChannel(GetOnlineUserName(connection->user), channel)) {
					connections.push_back(connection);
				}
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connections sockets by channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connections sockets by channel", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return connections;
}

// Get vector of all connections or filter by connection user online state.
LogicalConnectionsRef GetConnectionsRef(bool only_online_users) {
	LogicalConnectionsRef connections;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (!logical_connections.empty()) {
            LogicalConnections::iterator connection;
			for (connection = logical_connections.begin(); connection != logical_connections.end(); connection++) {
                // Add if we need all users or only online users and connection user is online.
				if (!only_online_users || (only_online_users && connection->auth_status == 9)) {
					connections.push_back(connection);
				}
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get all connections references: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get all connections references", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return connections;
}

// Get vector of all connections with online users.
LogicalConnectionsRef GetConnectionsRef() {
	LogicalConnectionsRef connections;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (!logical_connections.empty()) {
            LogicalConnections::iterator connection;
			for (connection = logical_connections.begin(); connection != logical_connections.end(); connection++) {
				connections.push_back(connection);
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get all connections references: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get all connections references", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return connections;
}

// Get list of all connections.
LogicalConnections GetConnections() {
	LogicalConnections connections;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		connections = logical_connections;
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get all connections: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get all connections", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return connections;
}

// Get expired connections (too old last action time).
void DeleteExpiredConnections() {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (!logical_connections.empty()) {
			int current_time = time(NULL);
			// Timeout in seconds (5 minutes).
			unsigned int timeout = 300;
			LogicalConnections::iterator it = logical_connections.begin(), next_it;
			do {
				// Set the pointer to the next iterator. So if next block will delete
				// current iterator, then the cycle will know which is the next iterator.
				next_it = ++it;
				it--;
				// Check timeout and if connection not persistent.
				if (it->sock == NULL && current_time - it->last_act_t > timeout ) {
					DeleteConnection(it);
				}
				it = next_it;
			} while (it != logical_connections.end());
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't delete expired connections: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't delete expired connections", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Disconects connection user from chat.
void ConnectionUserDisconnect(LogicalConnections::iterator conn) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end() && IsOnlineUserSet(conn->user)) {
			UserDisconnect(GetOnlineUserName(conn->user));
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't disconnect user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't disconnect user", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Disconnects user from chat and call UnsetConnectionUser to unset iterator.
void FullUserConnectionDelete(LogicalConnections::iterator conn) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end()) {
			if (IsOnlineUserSet(conn->user)) {
				// If user is online then send message to server to disconnect him. When response will be received
				// by cf_events - it will delete user iterator.
				if (conn->auth_status == 9) {
					UserDisconnect(GetOnlineUserName(conn->user));
				}
				UnsetConnectionUser(conn);
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't fully delete connection user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't fully delete connection user", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Удаляет соединение по его id.
void DeleteConnection(String conn_id) {
	DeleteConnection(SearchConnection(conn_id));
}

// Удаляет соединение по заранее известному итератору + отключение пользователя, если он не аноним.
void DeleteConnection(LogicalConnections::iterator conn) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end()) {
			FullUserConnectionDelete(conn);

			if (conn_list_form) {
				SendMessage(conn_list_form->Handle, CL_DO_ACTION, 1, (LPARAM)&(*conn));
			}

			logical_connections.erase(conn);

			if (main_form) {
				PostMessage(main_form->Handle, MF_DO_ACTION, 0, logical_connections.size());
			}
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't delete connection: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't delete connection", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Устанавливает итератор виртуального пользователя в соединении.
void SetConnectionUser(LogicalConnections::iterator conn, OnlineUsersList::iterator user) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end()) {
			conn->user = user;

			if (conn_list_form) {
				SendMessage(conn_list_form->Handle, CL_DO_ACTION, 2, (LPARAM)&(*conn));
			}
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't set connection user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't set connection user", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Устанавливает итератор виртуального пользователя на конец, убирает состояние авторизации и удаляет пользователя.
void UnsetConnectionUser(String user) {
	UnsetConnectionUser(FindConnectionByUser(user));
}

// Set user iterator to the end and delete the user.
void UnsetConnectionUser(LogicalConnections::iterator conn) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end()) {
			// If user was set.
			if (IsOnlineUserSet(conn->user)) {
				conn->user = GetEmptyOnlineUser();
				// Set authorization state to 6 - exitted from chat.
				SetAuthorizationStatus(conn, 6);
				// Update connections list form.
				if (conn_list_form) {
					SendMessage(conn_list_form->Handle, CL_DO_ACTION, 3, (LPARAM)&(*conn));
				}
			}
		}
    } catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't unset connection user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't unset connection user", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Обновляет время последнего действия для соединения.
void UpdateConnectionLastAction(String conn_id) {
	LogicalConnections::iterator conn = SearchConnection(conn_id);

	UpdateConnectionLastAction(conn);
}

void UpdateConnectionLastAction(LogicalConnections::iterator conn) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (conn != logical_connections.end()) {
			conn->last_act_t = time(NULL);
			if (conn_list_form) {
				SendMessage(conn_list_form->Handle, CL_DO_ACTION, 5, (LPARAM)&(*conn));
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't update connection last action: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't update connection last action", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Получение состояния авторизации соединения по его id.
void SetAuthorizationStatus(LogicalConnections::iterator connection, int authorization_status) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			connection->auth_status = authorization_status;

			// Send notification with new user authorization state.
			SendNotification("user_auth_state", authorization_status, connection, true);

			if (conn_list_form) {
				SendMessage(conn_list_form->Handle, CL_DO_ACTION, 4, (LPARAM)&(*connection));
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't set connection authorization status: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't set connection authorization status", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Set connection socket by iterator.
void SetConnectionSocket(LogicalConnections::iterator connection, SOCKET sd) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			connection->sock = sd;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't set connection socket: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't set connection socket", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Set connection auto actions get parameter by iterator.
void SetConnectionActionsGet(LogicalConnections::iterator connection, bool auto_actions_get) {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			connection->auto_actions_get = auto_actions_get;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't set connection auto actions get: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't set connection auto actions get", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);
}

// Получение состояния авторизации соединения по его id.
void SetAuthorizationStatus(String conn_id, int authorization_status) {
	LogicalConnections::iterator connection = SearchConnection(conn_id);

	SetAuthorizationStatus(connection, authorization_status);
}

int GetAuthorizationStatus(LogicalConnections::iterator connection) {
	int authorization_status = 7;

	try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (connection != logical_connections.end()) {
			authorization_status = connection->auth_status;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection authorization status: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection authorization status", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return authorization_status;
}

// Получение состояния авторизации соединения по его id.
int GetAuthorizationStatus(String conn_id) {
	return GetAuthorizationStatus(SearchConnection(conn_id));
}

bool IsHTTPConnection(LogicalConnections::iterator connection) {
	bool is_http = false;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			if (connection->sock == NULL) {
				is_http = true;
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't check if connection is http: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't check if connection is http", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return is_http;
}

// Проверка соединения на существование по указанному идентификатору.
bool ConnectionExists(LogicalConnections::iterator connection) {
	bool result = false;

    try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (connection != logical_connections.end()) {
			result = true;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't check if connection exists: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't check if connection exists", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return result;
}

// Проверка соединения на существование по указанному идентификатору.
bool ConnectionExists(String conn_id) {
	bool result = ConnectionExists(SearchConnection(conn_id));

	return result;
}

// Очистка списка пользователей.
void Clear() {
	try {
		EnterCriticalSection(&connections_manipulation_cs);

		logical_connections.clear();
	} __finally {
		LeaveCriticalSection(&connections_manipulation_cs);
	}
}

// Запрашивает состояние авторизации пользователя у соединения.
// Возвращает ответ на вопрос: Пользователь онлайн или возможно скоро будет онлайн?
bool GetUserOnlineStatus(LogicalConnections::iterator conn) {
	bool in_chat = false;

	try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (conn != logical_connections.end()) {
			if (conn->auth_status == 9 || conn->auth_status == 8) {
				in_chat = true;
			}
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection user online status: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection user online status", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return in_chat;
}

// Get user by connection iterator.
OnlineUsersList::iterator GetConnectionUser(LogicalConnections::iterator connection) {
	OnlineUsersList::iterator user;

	try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (connection != logical_connections.end()) {
			user = connection->user;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection user", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return user;
}

// Get connection iterator.
LogicalConnection GetConnection(LogicalConnections::iterator connection_ref) {
	LogicalConnection connection;

	try {
		EnterCriticalSection(&connections_manipulation_cs);
		if (connection_ref != logical_connections.end()) {
			connection = *connection_ref;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return connection;
}

// Get user name by connection iterator.
String GetConnectionUserName(LogicalConnections::iterator conn) {
	String name = "";

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (conn != logical_connections.end()) {
			name = GetOnlineUserName(conn->user);
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection user name: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection user name", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return name;
}

// Get connection id.
String GetConnectionID(LogicalConnections::iterator connection) {
	String conn_id;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			conn_id = connection->conn_id;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection ID: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection ID", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return conn_id;
}


// Get connection id.
String GetConnectionIP(LogicalConnections::iterator connection) {
	String ip = "";

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			ip = connection->ip;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection ip: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection ip", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return ip;
}

// Get connection socket.
SOCKET GetConnectionSocket(LogicalConnections::iterator connection) {
	SOCKET socket;

	try {
		EnterCriticalSection(&connections_manipulation_cs);

		if (connection != logical_connections.end()) {
			socket = connection->sock;
		}
	} catch (Exception *E) {
		InfoAction(__("ConnectionsManipulationWarning") + ". Can't get connection socket: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ConnectionsManipulationError") + ". Can't get connection socket", false, 3, 2);
	}

	LeaveCriticalSection(&connections_manipulation_cs);

	return socket;
}
