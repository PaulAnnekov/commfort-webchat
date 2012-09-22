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

#include "methods.h"
#include "json_writer.h"
#include "json_main.h"
#include "global.h"
#include "connections.h"
#include "cf_functions.h"
#include "socket_listen_thread.h"
#include "socket_utils.h"
#include "notif.h"
#include "l18n.h"
#include "user_channel.h"
#include "auth_queue.h"
#include "http_messages_queue.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

// Инициализация соединения.
String meth::ConnectionInit(String app_name, SocketСonnections::iterator socket_connection, int transport) {
	bool persistent;
	SOCKET sd = socket_connection->sd;
	String ip = socket_connection->ip;

	if (ip.IsEmpty()) {
		throw(JSONError(-1001, "Can't determine your IP-address"));
	}

	// If this is HTTP connection then set socket handle to NULL.
	if (transport == 1) {
		sd = NULL;
	}

	// For HTTP connection !transport will return false and plugin won't try to send actions.
	String conn_id = AddConnection(app_name, ip, sd, !transport, transport);

	return "{\"conn_id\":\"" + conn_id + "\"}";
}

// Закрытие соединения.
String meth::ConnectionClose(String conn_id) {
	DeleteConnection(conn_id);
	return "null";
}

// Инициализация соединения.
String meth::CheckConnectionID(String conn_id) {
	return (ConnectionExists(conn_id)) ? "true" : "false";
}

// Подключение пользователя.
String meth::VirtualUserConnect(LogicalConnections::iterator conn, String user_nick, String user_pass) {
	String response = "";

	if (!GetUserOnlineStatus(conn)) {
		AddAuthQueue(conn, user_nick);
		SetAuthorizationStatus(conn, 8);
		UserConnect(user_nick, GetConnectionIP(conn), true, user_pass, -1);
		response = "null";
	} else {
		throw(JSONError(-1002, "You have already connected virtual user"));
	}

	return response;
}

// Disconnect connection user.
void meth::VirtualUserDisconnect(LogicalConnections::iterator conn) {
	ConnectionUserDisconnect(conn);
}


// Регистрация пользователя.
String meth::VirtualUserRegister(LogicalConnections::iterator conn, String user_nick, String user_pass, String is_female) {
	String response = "";

	if (!GetUserOnlineStatus(conn)) {
		AddAuthQueue(conn, user_nick);
		SetAuthorizationStatus(conn, 8);
		UserConnect(user_nick, GetConnectionIP(conn), 1, user_pass, -1);
		response = "null";
	} else {
		throw(JSONError(-1002, "You have already connected virtual user"));
	}

	return response;
}

// Отправка сообщения в общий канал.
void meth::SendPublicMessage(LogicalConnections::iterator conn, String channel, String mode, String message, String &response) {
	String user = GetOnlineUserName(GetConnectionUser(conn));

	if (IsUserInChannel(user, channel)) {
		/* TODO : Сделать проверку бана на пользователе */
		SendChannelMessage(user, mode.ToInt(), channel, message);
		response = "null";
	} else {
		throw(JSONError(-1007, "User isn't connected to this channel"));
	}
}

// Получение последних событий.
String meth::GetActions(LogicalConnections::iterator conn, SOCKET sd) {
	String response = "";

	//if (IsHTTPConnection(conn)) {
		response = GetMessagesByConnection(GetConnectionID(conn));

		if (response.IsEmpty()) {
			SetConnectionSocket(conn, sd);
			SetConnectionActionsGet(conn, true);
		}
	//} else {
	//	throw(JSONError(-1006, "Method is only for HTTP connections"));
	//}

	return response;
}

// Получение состояния авторизации.
void meth::GetAuthState(String conn_id, String &response) {
	LogicalConnections::iterator connection = SearchConnection(conn_id);
	response = String(GetAuthorizationStatus(conn_id));
}

// Get channels list.
void meth::GetChannelsList(LogicalConnections::iterator conn, String user_chnls, String &response) {
	ChannelsList channels = GetChannels();
	bool first_channel = true;

	response = "[";
	for (ChannelsList::iterator channel = channels.begin(); channel != channels.end(); channel++) {
		if (channel->visibility == 1) {
			// Add comma before each channel except first.
			if (!first_channel) {
				response += ",";
			} else {
            	first_channel = false;
            }
			response += "{";
			AddPair(response, "name", channel->name, true, true);
			AddPair(response, "topic", channel->topic, true, true);
			AddPair(response, "users_online", channel->online_users, false, true);
			AddPair(response, "access", (int) channel->access, false, false);
			response += "}";
		}
	}

	response += "]";
}

// Получение списка пользователей указанного канала.
void meth::GetChannelUsers(LogicalConnections::iterator conn, String channel_name, String &response) {
	if (IsUserInChannel(GetOnlineUserName(GetConnectionUser(conn)), channel_name)) {
		response = "[" + implode(::GetChannelUsers(channel_name)) + "]";
	} else {
		throw(JSONError(-32001, "Virtual user is not connected to this channel. You must be in this channel to get it's members"));
	}
}

// Get users list with info.
void meth::GetOnlineUsers(LogicalConnections::iterator conn, String &response) {
	OnlineUsersList online_users = ::GetOnlineUsers();
	bool first_user = true;
	User user;

	response = "[";
	for (OnlineUsersList::iterator online_user = online_users.begin(); online_user != online_users.end(); online_user++) {
		// Add comma before each user except first.
		if (!first_user) {
			response += ",";
		} else {
			first_user = false;
		}
		response += "{";

		user = GetUser(online_user->user_ref);
		AddPair(response, "name", user.name, true, true);
		AddPair(response, "sex", (int) user.sex, false, true);
		AddPair(response, "ip", (user.hidden_ip) ? String("N/A") : user.ip, true, true);
		AddPair(response, "state", online_user->state, true, false);
		response += "}";
	}

	response += "]";
}

// Get user channels.
void meth::GetUserChannels(LogicalConnections::iterator conn, String &response) {
	StringList channels = ::GetUserChannels(GetConnectionUserName(conn));

	if (!channels.empty()) {
		response = "[" + implode(channels) + "]";
	} else {
		response = "null";
	}
}

// Get image.
void meth::GetImage(LogicalConnections::iterator conn, String image_id, String &response) {
	response = "\"" + LoadImage(image_id) + "\"";
	if (response.IsEmpty()) {
		throw JSONError(-1008, "Image with such id doesn't exist");
	}
}

// Connect to channel.
void meth::UserChannelConnect(LogicalConnections::iterator conn, String channel, String visibile, String enter_type, String &response) {
	::UserChannelConnect(GetConnectionUserName(conn), channel, visibile.ToInt(), enter_type.ToInt());
	response = "null";
}

// Disconnect from channel.
void meth::UserChannelDisconnect(LogicalConnections::iterator conn, String channel, String &response) {
	::UserChannelDisconnect(GetConnectionUserName(conn), channel);
	response = "null";
}

// Get own user name.
void meth::GetOwnUserName(LogicalConnections::iterator conn, String &response) {
	response = "\"" + GetConnectionUserName(conn) + "\"";
}
