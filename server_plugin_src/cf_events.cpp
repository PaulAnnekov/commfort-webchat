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

#include "cf_events.h"
#include "cf_functions.h"
#include "notif.h"
#include "l18n.h"
#include "global.h"
#include "debug.h"
#include "connections.h"
#include "webchat.h"
#include "db\channels_watch.h"
#include "json_writer.h"
#include "json_main.h"
#include "users.h"
#include "channels.h"
#include "online_users.h"
#include "user_channel.h"
#include "auth_queue.h"
#include <SysUtils.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Вход в чат.
void OnUserConnect(BYTE *InBuffer) {
	String name, ip, query;
	DWORD male;

	try {
		//name
		GetStreamString(&InBuffer, &name);
		//ip
		GetStreamString(&InBuffer, &ip);
		//male
		GetStreamDword(&InBuffer, &male);

		// Get user from registred users list, add it's reference to online users and update user info.
		UsersList::iterator user = FindUserByName(name);
		CFUserInfo user_info = GetUserInfo(name);
		OnlineUsersList::iterator online_user = AddOnlineUser(user, user_info.state, user_info.client_version, user_info.process, user_info.window_activity, user_info.downtime);
		UpdateUser(user, user_info.computer_id, user_info.ip, user_info.male);

		// If user is in authorization queue.
		AuthQueueMap::iterator auth_queue_item = FindAuthQueueByUser(name);
		if (IsAuthQueueElement(auth_queue_item)) {
			LogicalConnections::iterator connection = GetAuthQueueConnection(auth_queue_item);
			SetConnectionUser(connection, online_user);
			SetAuthorizationStatus(connection, 9);
			DeleteAuthQueueItem(auth_queue_item);
		}

		MapList params;
		params.insert(pair<String, String>("name", name));
		params.insert(pair<String, String>("ip", ip));
		params.insert(pair<String, String>("sex", male));

		String json_object = SetParametersObject(params, true);
		SendNotification("user_connect", json_object, GetConnectionsRef(true));
	} catch (Exception *E) {
		throw(Format(e_user_connect, ARRAYOFCONST((E->Message))));
	}
}

// Подключение к каналу виртуального пользователя.
void OnVirtualUserChannelConnect(BYTE *InBuffer) {
	String name, channel_name;

	try {
		// virtual user.
		GetStreamString (&InBuffer, &name);
		// channel.
		GetStreamString(&InBuffer, &channel_name);
		// topic.
		GetStreamString(&InBuffer, NULL);
		// greeting.
		GetStreamString(&InBuffer, NULL);

		LogicalConnections::iterator connection = FindConnectionByUser(name);
		OnlineUsersList::iterator user = GetConnectionUser(connection);
		ChannelsList::iterator channel = FindChannelByName(channel_name);
		StringList channel_users;
		if (!IsChannelMonitored(channel_name)) {
			AddChannelWatcher(name, channel_name);
			Channel channel_info = GetChannelInfo(channel_name);
			channel_users = GetChannelUsersList(name, channel_name);
			channel_info.online_users = channel_users.size();
			UpdateChannel(channel, GetChannelInfo(channel_name));
			for (unsigned int i = 0; i < channel_info.online_users; i++) {
				AddUserChannel(name, channel_name);
			}
		} else {
			AddUserChannel(name, channel_name);
			channel_users = GetChannelUsers(channel_name);
        }

		MapList params;
		params.insert(pair<String, String>("channel", channel_name));
		params.insert(pair<String, String>("topic", channel->topic));
		params.insert(pair<String, String>("greeting", channel->greeting));
		params.insert(pair<String, String>("users", "[" + implode(channel_users) + "]"));

		String json_object = SetParametersObject(params, true);
		SendNotification("virtual_user_channel_connect", json_object, connection, false);
	} catch (Exception *E) {
		throw(Format(e_virtual_user_channel_connect, ARRAYOFCONST((E->Message))));
	}
}

// Подключение к каналу стороннего пользователя.
void OnUserChannelConnect(BYTE *InBuffer) {
	String bot, name, channel, ip, query;
	DWORD male;

	try {
		//virtual user
		GetStreamString (&InBuffer, &bot);
		//channel
		GetStreamString(&InBuffer, &channel);

		// If this user is channel watcher.
		if (UserIsMonitoring(bot, channel)) {
			//name
			GetStreamString(&InBuffer, &name);
			//ip
			GetStreamString(&InBuffer, &ip);
			//male
			GetStreamDword(&InBuffer, &male);

			AddUserChannel(name, channel);

            MapList params;
			params.insert(pair<String, String>("user", name));
			params.insert(pair<String, String>("channel", channel));

			String json_object = SetParametersObject(params);
			SendNotification("user_channel_connect", json_object, GetChannelConnections(channel));
		}
	} catch (Exception *E) {
		throw(Format(e_user_channel_connect, ARRAYOFCONST((E->Message))));
	}
}

// Отключение от канала стороннего или виртуального пользователя.
void OnUserChannelDisconnect(BYTE *InBuffer) {
	String bot, channel, name;

	try {
		// virtual user.
		GetStreamString(&InBuffer, &bot);
		// channel.
		GetStreamString(&InBuffer, &channel);
		if (UserIsMonitoring(bot, channel)) {
            // name.
			GetStreamString(&InBuffer, &name);

			MapList params;
			params.insert(pair<String, String>("user", name));
			params.insert(pair<String, String>("channel", channel));

			String json_object = SetParametersObject(params);
			SendNotification("user_channel_disconnect", json_object, GetChannelConnections(channel));

			DeleteUserChannel(name, channel);

			// If channel watcher was disconnected, then find new watcher.
			if (name == bot) {
				ChangeChannelWatcher(channel);
			}
		}
	} catch (Exception *E) {
		throw(Format(e_user_channel_disconnect, ARRAYOFCONST((E->Message))));
	}
}

// Публикация в общий канал.
void OnPublicChannelMessage(BYTE *InBuffer) {
	String bot, name, channel, text;
	DWORD mode, male, image_length;

	try {
		//bot nick
		GetStreamString (&InBuffer, &bot);
		//nick
		GetStreamString(&InBuffer, &name);
		//ip
		GetStreamString(&InBuffer, NULL);
		//male
		GetStreamDword(&InBuffer, &male);
		//channel
		GetStreamString(&InBuffer, &channel);

		// Если виртуальный пользователь является пользователем, отслеживающим данный канал.
		if (UserIsMonitoring(bot, channel)) {
			//rezim
			GetStreamDword(&InBuffer, &mode);
			//text
			GetStreamString(&InBuffer, &text);

			GetStreamDword(&InBuffer, &image_length);

			MapList params;
			params.insert(pair<String, String>("sender", name));
			params.insert(pair<String, String>("channel", channel));
			params.insert(pair<String, String>("mode", mode));
			params.insert(pair<String, String>("message", text));

            if (image_length > 0) {
				String image_name = SaveData(InBuffer, image_length);
				if (!image_name.IsEmpty()) {
					params.insert(pair<String, String>("image", image_name));
				}
			}
			String json_object = SetParametersObject(params);
			SendNotification("public_mess", json_object, GetChannelConnections(channel));
	   	}
	} catch (Exception *E) {
		throw(Format(e_user_mess_recieve, ARRAYOFCONST((E->Message))));
	}
}

// Изменение темы.
void OnChannelTopicChanged(BYTE *InBuffer) {
	String bot, name, channel, topic;

	try {
		//bot_prop.name
		GetStreamString(&InBuffer, &bot);

		// Если виртуальный пользователь является пользователем, отслеживающим данный канал.
		if (UserIsMonitoring(bot, channel)) {
			//channel
			GetStreamString(&InBuffer, &channel);
			//name
			GetStreamString(&InBuffer, &name);
			//ip
			GetStreamString(&InBuffer, NULL);
			//male
			GetStreamDword(&InBuffer, NULL);
			//new theme
			GetStreamString(&InBuffer, &topic);

			int timestamp = time(NULL);

			UpdateChannelTopic(channel, topic, name, timestamp);

			MapList params;
			params.insert(pair<String, String>("channel", channel));
			params.insert(pair<String, String>("topic_editor", name));
			params.insert(pair<String, String>("topic", topic));
			params.insert(pair<String, String>("timestamp", timestamp));

			String json_object = SetParametersObject(params);
			SendNotification("channel_topic_changed", json_object, GetChannelConnections(channel));
		}
	} catch (Exception *E) {
		throw(Format(e_chnl_theme_changed, ARRAYOFCONST((E->Message))));
	}
}

// Выход из чата.
void OnUserDisconnect(BYTE *InBuffer) {
	String name;

	try {
		//name
		GetStreamString(&InBuffer, &name);

		RemoveUserFromChannels(name);

		// Проверяем, является ли пользователь виртуальным. Если да, то проверяем список отслеживаний каналов.
		LogicalConnections::iterator connection = FindConnectionByUser(name);
		if (ConnectionExists(connection)) {
			UnsetConnectionUser(connection);
		}

		RemoveOnlineUser(name);

		MapList params;
		params.insert(pair<String, String>("user", name));

		String json_object = SetParametersObject(params);
		SendNotification("user_disconnect", json_object, GetConnectionsRef(true));
	} catch (Exception *E) {
		throw(Format(e_user_disconnect, ARRAYOFCONST((E->Message))));
	}
}

// Изменение состояния пользователя.
void OnUserStateChanged(BYTE *InBuffer) {
	String name, state;

	try {
		//name
		GetStreamString(&InBuffer, &name);
		//ip
		GetStreamString(&InBuffer, NULL);
		//male
		GetStreamDword(&InBuffer, NULL);
		//state
		GetStreamString(&InBuffer, &state);

        UpdateOnlineUserState(name, state);

        MapList params;
		params.insert(pair<String, String>("user", name));
		params.insert(pair<String, String>("state", state));

		String json_object = SetParametersObject(params);
		SendNotification("user_state_changed", json_object, GetConnectionsRef(true));
	} catch (Exception *E) {
		throw(Format(e_user_state_changed, ARRAYOFCONST((E->Message))));
	}
}

// Изменение иконки пользователя.
void OnUserSexChanged(BYTE *InBuffer) {
	String name;
	DWORD sex;

	try {
		//name
		GetStreamString(&InBuffer, &name);
		//ip
		GetStreamString(&InBuffer, NULL);
		//male
		GetStreamDword(&InBuffer, NULL);
		//new male
		GetStreamDword(&InBuffer, &sex);

		UpdateOnlineUserSex(name, sex);

        MapList params;
		params.insert(pair<String, String>("user", name));
		params.insert(pair<String, String>("sex", sex));

		String json_object = SetParametersObject(params);
		SendNotification("user_sex_changed", json_object, GetConnectionsRef(true));
	} catch (Exception *E) {
		throw(Format(e_user_sex_changed, ARRAYOFCONST((E->Message))));
	}
}

// Авторизация виртуального пользователя невозможна.
void OnUnsuccessAuth(BYTE *InBuffer) {
	String name;
	DWORD authorization_status;

	try {
		//name
		GetStreamString (&InBuffer, &name);
		//rezim
		GetStreamDword (&InBuffer, &authorization_status);

		AuthQueueMap::iterator auth_queue_item = FindAuthQueueByUser(name);
		LogicalConnections::iterator connection = GetAuthQueueConnection(auth_queue_item);
		DeleteAuthQueueItem(auth_queue_item);

		SetAuthorizationStatus(connection, authorization_status + 10);
		UnsetConnectionUser(connection);
	} catch (Exception *E) {
		throw(Format(e_unsuccess_auth, ARRAYOFCONST((E->Message))));
	}
}

// New user registered.
void OnUserRegistered(BYTE *InBuffer) {
	String name, ip;
	DWORD sex;

	try {
		//name
		GetStreamString(&InBuffer, &name);
		//ip
		GetStreamString(&InBuffer, &ip);
		//sex
		GetStreamDword(&InBuffer, &sex);

		AddUser(name, NULL, ip, sex, false);
	} catch (Exception *E) {
		throw(Format(__("OnUserRegisteredError"), ARRAYOFCONST((E->Message))));
	}
}
//---------------------------------------------------------------------------
