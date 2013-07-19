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

#include "json_main.h"
#include "json_reader.h"
#include "json_writer.h"
#include "methods.h"
#include "notif.h"
#include "socket_listen_thread.h"
#include "http_listen_thread.h"
#include "http_messages_queue.h"
#include "socket_utils.h"
#include "webchat.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace meth;

JSONERROR::JSONERROR(int e_type, String e_data, String e_id) {
	type = e_type;
	data = e_data;
	id = e_id;
}

JSONERROR JSONError(int type, String data) {
	JSONERROR JSONError(type, data, NULL);

	return JSONError;
}

JSONERROR JSONError(int type, String data, String id) {
	JSONERROR JSONError(type, data, id);

	return JSONError;
}

// Get method id from method name.
int GetMethodID(String method) {
	int meth_id = -1;

	for (int i = 0; i < meth_n; i++) {
		if (methods[i] == method) {
			meth_id = i;
			break;
		}
	}

	return meth_id;
}

// Дополнительная проверка значения переменной. Проверка учитывает наличие значения, его тип
// и может устанавливать значение по умолчанию.
String GetMemberValue(map<String, String> *method_obj, String member, bool is_req, String def = "", int type = NULL) {
	/*
	type:
		1 - bool
		2 - int
	*/
	String value = GetMemberValue(method_obj, member);

	// Если значение пустое или отсутствует.
	if (value.IsEmpty()) {
		// Если значение должно быть не пустое - ошибка.
		if (is_req) {
			throw(JSONError(-32602, "Parameter " + member + " does not exist or empty"));
		// Если у нас есть значение по умолчанию для этой переменной, то берем его.
		} else if (!def.IsEmpty()) {
			value = def;
		}
    // Если поле не пустое, то проверяем его на тип.
	} else {
		if (type != NULL) {
			bool is_valid = true;
			if (type == 1 && (value != "1" && value != "0")) {
				is_valid = false;
			} else if(type == 2) {
				try {
					value.ToInt();
				} catch (Exception *E) {
					is_valid = false;
				}
			}

			if (!is_valid) {
				throw(JSONError(-32602, "Value of the " + member + " parameter is not valid"));
			}
		}
	}

	return value;
}

// Получение значения переменной.
String GetMemberValue(map<String, String> *method_obj, String member) {
	map<String, String>::iterator pair;
	String value = "";

	pair = method_obj->find(member);
	if (pair != method_obj->end()) {
		value = pair->second;
		if (value.IsEmpty()) {
			value = "";
		}
	};

	return value;
}

// Call method by it's id.
String CallMethod(String id, int meth_id, Params params, SocketСonnections::iterator socket_connection, int transport) {
	LogicalConnections::iterator conn = NULL;
	String response;

	try {
		if (meth_id == 0) {
			response = ConnectionInit(GetMemberValue(&params, "app_name"), socket_connection, transport);
		} else if (meth_id == 1) {
			response = CheckConnectionID(GetMemberValue(&params, "params", true, "", NULL));
		} else {
			// Validate connection ID.
			conn = SearchConnection(GetMemberValue(&params, "conn_id", true, "", NULL));
			if (!ConnectionExists(conn)) {
				throw JSONError(-32602, "Such conn_id value does not exist");
			}
			UpdateConnectionLastAction(conn);

			// Check if user in chat.
			if (meth_id > 6) {
				if (GetAuthorizationStatus(conn) != 9) {
					throw(JSONError(-1005, "User isn't in chat"));
                }
			}

			switch (meth_id) {
				case 2: // conn_close
					DeleteConnection(conn);
					response = "null";
				break;
				case 3: // user_connect
					response = VirtualUserConnect(conn,
						GetMemberValue(&params, "nick", true, "", NULL),
						GetMemberValue(&params, "pass", true, "", NULL)
					);
				break;
				case 4: // user_reg
					response = VirtualUserRegister(conn,
						GetMemberValue(&params, "nick", true, "", NULL),
						GetMemberValue(&params, "pass", true, "", NULL),
						GetMemberValue(&params, "sex", true, "", 1)
					);
				break;
				case 5: // get_auth_state
					response = GetAuthorizationStatus(conn);
				break;
				case 6: // get_actions
					response = GetActions(conn, socket_connection->sd);
				break;
				case 7: // send_public_message
					SendPublicMessage(conn,
						GetMemberValue(&params, "channel", true, "", NULL),
						GetMemberValue(&params, "mode", false, 0, 1),
						GetMemberValue(&params, "message", true, "", NULL), response);
				break;
				case 8: // get_chnls_list
					GetChannelsList(conn,
						GetMemberValue(&params, "user_chnls", false, 0, 1),
						response
					);
				break;
				case 9: // get_chnl_users
					GetChannelUsers(conn, GetMemberValue(&params, "chnl_name", true, "", NULL), response);
				break;
				case 10: // get_online_users
					GetOnlineUsers(conn, response);
				break;
				case 11: // get_user_channels
					GetUserChannels(conn, response);
				break;
				case 12: // user_disconnect
					VirtualUserDisconnect(conn);
					response = "null";
				break;
				case 13: // get_user_channels
					GetImage(conn, GetMemberValue(&params, "image_id", true), response);
				break;
				case 14: // channel_connect
					UserChannelConnect(conn,
						GetMemberValue(&params, "channel", true),
						GetMemberValue(&params, "visible", false, "1", 1),
						GetMemberValue(&params, "enter_type", false, "0", 1),
						response
					);
				break;
				case 15: // channel_disconnect
					UserChannelDisconnect(conn, GetMemberValue(&params, "channel", true), response);
				break;
				case 16: // get_own_user_name
					GetOwnUserName(conn, response);
				break;
			}
		}
	} catch (JSONERROR je) {
		throw(JSONError(je.type, je.data, id));
	} catch (Exception *E) {
		throw(JSONError(-32603, "Unexpected server error", id));
	} catch (...) {
		throw(JSONError(-32603, "Unexpected server error", id));
    }

	return response;
}

// Check method object.
pRequest CheckObject(Params method_obj) {
	Params params;
	String err = NULL, method = NULL, id = NULL, value;
	int meth_id = -1;

	id = GetMemberValue(&method_obj, "id");

	value = GetMemberValue(&method_obj, "jsonrpc");
	if (value.IsEmpty() || value != "2.0" ) {
		throw(JSONError(-32600, "JSON version member doesn't exists, or version is not 2.0", id));
	}

	method = GetMemberValue(&method_obj, "method");
	meth_id = GetMethodID(method);
	if (value.IsEmpty()) {
		throw(JSONError(-32600, "Method member doesn't exists in object", id));
	} else if (meth_id == -1) {
		throw(JSONError(-32601, "Method " + method + " doesn't exists", id));
	}

	value = GetMemberValue(&method_obj, "params");
	if (value.IsEmpty()) {
		throw(JSONError(-32600, "Params member doesn't exists in object", id));
	}

	if (IsJSONStructure(value)) {
		params = ParseJSONObject(GetJSONStructure(&value));
	} else {
		params.insert(pair<String,String>("params", (IsJSONString(value)) ? ParseJSONString(&value) : value));
	}

	if (params.empty()) {
		throw(JSONError(-32600, "List of params is empty", id));
	}

	return pRequest(id, meth_id, method, params);
}

// Sends notification for socket clients or save notification to DB for HTTP clients.
// single_value indicates that "params" variable has string, number or literal value.
void SendNotification(String method, String json_object, LogicalConnections::iterator connection_ref, bool single_value = false) {
	if (ConnectionExists(connection_ref)) {
		if (!single_value) {
			json_object = "{" + json_object + "}";
		}
		LogicalConnection connection = GetConnection(connection_ref);
		if (connection.sock != NULL && connection.auto_actions_get) {
			SetJSONResponseObject(json_object, method, "");

			// If transport == 1 (http connection), then send headers.
			SocketWrite(connection.sock, json_object, (connection.transport == 1));
			if (connection.transport == 1) {
				SetConnectionActionsGet(connection_ref, false);
			}
		} else {
			AddMessagesMap(connection.conn_id, Format("{\"method\":\"%s\",\"result\":%s}", ARRAYOFCONST((method, json_object))));
		}
	}
}

void SendNotification(String method, String json_object, LogicalConnectionsRef connections) {
	LogicalConnectionsRef::iterator connection;

	if (!connections.empty()) {
		for (connection = connections.begin(); connection != connections.end(); connection++) {
			SendNotification(method, json_object, *connection);
		}
	}
}

