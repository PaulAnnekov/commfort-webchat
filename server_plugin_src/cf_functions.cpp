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
#include <Dialogs.hpp>
#include <DateUtils.hpp>

#include "cf_functions.h"
#include "global.h"
#include "webchat.h"
#include "notif.h"
#include "l18n.h"
#include "debug.h"

#pragma package(smart_init)

// Получение адреса папки, предназначенной для хранилища настроек плагинов.
String GetPluginDirectory() {
	String dir;
	BYTE *data = NULL, *pCopy;

	DWORD size = (*CommFortGetData)(dwPluginID, 2010, NULL, NULL, NULL, NULL);
	data = new BYTE[size];
	pCopy = data;

	(*CommFortGetData)(dwPluginID, 2010, data, size, NULL, NULL);
	GetStreamString(&pCopy, &dir);

	return dir;
}

// Получение списка пользователей онлайн всего чата.
// Возвращает список пользователей с информацией о них.
CFUsersList GetChatUsersList() {
	BYTE *pCopy,
		 *data = NULL,
		 *bOutBuffer = NULL;
	DWORD i, users_sum, male;
	String nick, ip;
	CFUsersList users_list;

	try {
		DWORD size = (*CommFortGetData)(dwPluginID, 1042, NULL, NULL, NULL, NULL);
		data = new BYTE[size];
		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1042, data, size, NULL, NULL);

		GetStreamDword (&pCopy, &users_sum);

		for (i = 1; i <= users_sum; i++) {
			//nick
			GetStreamString (&pCopy, &nick);
			//ip
			GetStreamString (&pCopy, &ip);
			//male
			GetStreamDword (&pCopy, &male);

			users_list.push_back(CFUser(nick, ip, male));
		}
	} catch (Exception *E) {
		InfoAction("Произошла ошибка при получении списка зарегистрированных пользователей. Ошибка: \"" + E->Message + "\"", false, 0, 2);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
		pCopy = NULL;
	}

	return users_list;
}

// Получение списка пользователей онлайн всего чата.
// Возвращает список пользователей с информацией о них.
CFUsersList GetChatOnlineUsersList() {
	BYTE *pCopy,
		 *data = NULL,
		 *bOutBuffer = NULL;
	DWORD i, users_sum, male;
	String nick, ip;
	CFUsersList users_list;

	try {
		DWORD size = (*CommFortGetData)(dwPluginID, 1041, NULL, NULL, NULL, NULL);
		data = new BYTE[size];
		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1041, data, size, NULL, NULL);

		GetStreamDword (&pCopy, &users_sum);

		for (i = 1; i <= users_sum; i++) {
			//nick
			GetStreamString (&pCopy, &nick);
			//ip
			GetStreamString (&pCopy, &ip);
			//male
			GetStreamDword (&pCopy, &male);

			users_list.push_back(CFUser(nick, ip, male));
		}
	} catch (Exception *E) {
		InfoAction("Произошла ошибка при получении списка пользователей в чате. Ошибка: \"" + E->Message + "\"", false, 0, 2);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
		pCopy = NULL;
	}

	return users_list;
}

// Получение списка пользователей указанного канала.
StringList GetChannelUsersList(String user, String channel) {
	BYTE *pCopy,
		 *data = NULL,
		 *bOutBuffer = NULL;
	DWORD i, name_len, channel_len, users_sum, len,
		  pos_offset = 0;
	String nick;
	StringList users_list;

	try {
		name_len = user.Length();
		channel_len = channel.Length();
		len = (name_len + channel_len) * 2 + 8;
		bOutBuffer = new char[len];

		SetStreamString (bOutBuffer, &pos_offset, user, name_len);
		SetStreamString (bOutBuffer, &pos_offset, channel, channel_len);

		DWORD size = (*CommFortGetData)(dwPluginID, 1081, NULL, NULL, bOutBuffer, len);
		data = new BYTE[size];
		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1081, data, size, bOutBuffer, len);

		GetStreamDword (&pCopy, &users_sum);

		for (i = 1; i <= users_sum; i++) {
			//nick
			GetStreamString (&pCopy, &nick);
			//ip
			GetStreamString (&pCopy, NULL);
			//male
			GetStreamDword (&pCopy, NULL);

			users_list.push_back(nick);
        }
	} catch (Exception *E) {
		InfoAction("Произошла ошибка при получении списка пользователей. Ошибка: \"" + E->Message + "\"", false, 0, 2);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
		pCopy = NULL;
	}

	return users_list;
}

// Получает список каналов в чате от сервера и возвращает его.
CFChannelsList GetChannelsList() {
	BYTE * data = NULL, *pCopy;
	DWORD num, users_online;
	String chn_name, chn_topic;
	CFChannelsList channels;

	try {
	/*
		ID: 1040
		Блок данных (исходящий): [нулевое значение]
		Блок данных (входящий): число(количество) + (текст(название канала) + число(количество пользователей в канале) + текст(тема канала))*количество
	*/
		DWORD size = (*CommFortGetData)(dwPluginID, 1040, NULL, NULL, NULL, NULL);
		data = new BYTE[size];

		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1040, data, size, NULL, NULL);

		GetStreamDword (&pCopy, &num);

		for (unsigned int i = 1; i <= num; i++) {
			GetStreamString (&pCopy, &chn_name);
			GetStreamDword  (&pCopy, &users_online);
			GetStreamString (&pCopy, &chn_topic);
			channels.push_back(CFChannelShortInfo(chn_name, users_online, chn_topic));
		}
	} __finally {
		if (data) {
			delete []data;
			data = NULL;
		}
	}

	return channels;
}

// Получает информацию о указанном канале.
Channel GetChannelInfo(String channel_name) {
	BYTE *data = NULL, *bOutBuffer = NULL, *pCopy;
	DWORD length, channel_length, visibility, access, topic_edit, position_offset = 0;
	double topic_edit_time;
	Channel channel = Channel(channel_name, 0, "");

	channel_length = channel_name.Length();
	length = channel_length * 2 + 4;
	bOutBuffer = new char[length];

	try {
	/*
		ID: 1064
		Блок данных (исходящий): текст(название канала)
		Блок данных (входящий): текст(тема канала) + текст(имя последнего пользователя, изменившего тему) +
		дата_и_время(последнего редактирования темы) + текст(приветствие) + число(режим публикации изображений) +
		число(видимость в списке) + число(режим доступности входа) + число(режим доступности редактирования темы)
	*/
		SetStreamString(bOutBuffer, &position_offset, channel_name, channel_length);
		DWORD size = (*CommFortGetData)(dwPluginID, 1064, NULL, NULL, bOutBuffer, length);

		data = new BYTE[size];

		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1064, data, size, bOutBuffer, length);

		// channel topic.
		GetStreamString(&pCopy, &channel.topic);
		// имя последнего пользователя, изменившего тему.
		GetStreamString(&pCopy, &channel.topic_editor);
		// последнего редактирования темы.
		GetStreamTime(&pCopy, topic_edit_time);
		channel.topic_date = (topic_edit_time != 0) ? Dateutils::DateTimeToUnix(topic_edit_time) : 0;
		// приветствие.
		GetStreamString(&pCopy, &channel.greeting);
		// режим публикации изображений.
		GetStreamDword(&pCopy, &channel.images_type);
		// видимость в списке.
		GetStreamDword(&pCopy, &visibility);
		channel.visibility = visibility;
		// режим доступности входа.
		GetStreamDword(&pCopy, &access);
		channel.access = access;
		// режим доступности редактирования темы.
		GetStreamDword(&pCopy, &topic_edit);
		channel.topic_edit = topic_edit;
	} catch (Exception *E) {
		InfoAction("Произошла ошибка во время получения информации о канале \"" + channel_name + "\". Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
	}

	return channel;
}

// Получает полную информацию о пользователе.
CFUserInfo GetUserInfo(String name) {
	BYTE *data = NULL, *bOutBuffer = NULL, *pCopy;
	DWORD len, name_len, pos_offset = 0;
	String state;
	CFUserInfo user_info;

	name_len = name.Length();
	len = name_len * 2 + 4;
	bOutBuffer = new char[len];

	try {
		SetStreamString(bOutBuffer, &pos_offset, name, name_len);
		DWORD size = (*CommFortGetData)(dwPluginID, 1063, NULL, NULL, bOutBuffer, len);

		if (size > 0) {
        	data = new BYTE[size];
			pCopy = data;

			(*CommFortGetData)(dwPluginID, 1063, data, size, bOutBuffer, len);
			// IP-адрес.
			GetStreamString(&pCopy, &user_info.ip);
			// публично отображаемый IP-адрес.
			GetStreamString(&pCopy, &user_info.public_ip);
			// ID компьютера.
			GetStreamString(&pCopy, &user_info.computer_id);
			// иконка пользователя.
			GetStreamDword(&pCopy, &user_info.male);
			// состояние.
			GetStreamString(&pCopy, &user_info.state);
			// версия клиента.
			GetStreamString(&pCopy, &user_info.client_version);
			// состояние активности окна программы.
			GetStreamDword(&pCopy, &user_info.window_activity);
			// время простоя в секундах.
			GetStreamDword(&pCopy, &user_info.downtime);
			// активный процесс.
			GetStreamString(&pCopy, &user_info.process);
		}
	} catch (Exception *E) {
		InfoAction("Произошла ошибка во время получения информации о пользователе \"" + name + "\". Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
	}

	return user_info;
}

// Проверка зарегистрирован ли пользователь (проверка IP адреса).
bool GetUserRegState(String nick) {
	BYTE * data = NULL, *bOutBuffer = NULL, *pCopy;
	DWORD len, nick_len, pos_offset = 0;
	String ip = "";
	bool result = false;

	nick_len = nick.Length();
	len = nick_len * 2 + 4;
	bOutBuffer = new char[len];

	try {
		SetStreamString (bOutBuffer, &pos_offset, nick, nick_len);
		DWORD size = (*CommFortGetData)(dwPluginID, 1061, NULL, NULL, bOutBuffer, len);

		data = new BYTE[size];
		pCopy = data;

		(*CommFortGetData)(dwPluginID, 1061, data, size, bOutBuffer, len);
		GetStreamString(&pCopy, &ip);
	} catch (Exception *E) {
		InfoAction("Произошла ошибка во время проверки регистрации пользователя \"" + nick + "\". Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
	}

	if (ip != "") {
		result = true;
    }

	return result;
}

// Get user computer ID.
String GetComputerID(String name) {
	BYTE *data = NULL, *bOutBuffer = NULL, *pCopy;
	DWORD len, name_length, pos_offset = 0;
	String computer_id = "";

	name_length = name.Length();
	len = name_length * 2 + 4;
	bOutBuffer = new char[len];

	try {
		SetStreamString (bOutBuffer, &pos_offset, name, name_length);
		DWORD size = (*CommFortGetData)(dwPluginID, 1062, NULL, NULL, bOutBuffer, len);

		data = new BYTE[size];
		pCopy = data;

		(*CommFortGetData)(dwPluginID, 1062, data, size, bOutBuffer, len);
		GetStreamString(&pCopy, &computer_id);
	} catch (Exception *E) {
		InfoAction(Format(__("GetComputerIDWarning"), ARRAYOFCONST((name, E->Message))), false, 0, 1);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
	}

	return computer_id;
}

// Проверка скрытости IP-адреса пользователя.
// Возвращаемое значение - N/A, если IP скрыт или ник пользователя в белом списке; IP-адрес, если IP не скрыт.
bool IsHiddenIP(String name) {
	BYTE * data = NULL, *bOutBuffer = NULL, *pCopy;
	DWORD len, nick_len, ip_state, pos_offset = 0;

	nick_len = name.Length();
	len = nick_len * 2 + 4;
	bOutBuffer = new char[len];

	try {
		SetStreamString (bOutBuffer, &pos_offset, name, nick_len);
		DWORD size = (*CommFortGetData)(dwPluginID, 1050, NULL, NULL, bOutBuffer, len);

		data = new BYTE[size];
		pCopy = data;

		(*CommFortGetData)(dwPluginID, 1050, data, size, bOutBuffer, len);
		GetStreamDword(&pCopy, &ip_state);
	} catch (Exception *E) {
		InfoAction("Произошла ошибка во время проверки состояния скрытия IP пользователя \"" + name + "\". Ошибка: \"" + E->Message + "\"", false, 2, 1);
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
	}

	return ip_state;
}

// Вход виртуального пользователя в чат.
// pass_encrypt:
//  0 – пароль в открытом виде
//  1 – 32х символьный MD5 хэш-код пароля
// is_female:
//  0 - male
//  1 - female
//  2 - not set (for connect, not register)
void UserConnect(String name, String ip, bool pass_encrypt, String pass, int is_female) {
	DWORD len, pos_offset = 0, name_len, ip_len, pass_len;
	name_len = name.Length();
	ip_len = ip.Length();
	pass_len = pass.Length();
	//текст(имя) + текст(IP-адрес) + число(тип пароля) + текст(пароль) + число(иконка)
	//len = 4 + bot_prop.name.Length() * 2 + 4 + bot_prop.ip.Length() * 2 + 4 + 4 + bot_prop.pass.Length() * 2 + 4;

	len = 20 + (name_len + ip_len + pass_len) * 2;
	BYTE * bOutBuffer = new char[len];
	try {
		SetStreamString(bOutBuffer, &pos_offset, name, name_len);
		SetStreamString(bOutBuffer, &pos_offset, ip, ip_len);
		SetStreamString(bOutBuffer, &pos_offset, NULL, pass_encrypt);
		SetStreamString(bOutBuffer, &pos_offset, pass, pass_len);
		if (is_female != -1) {
			SetStreamString (bOutBuffer, &pos_offset, NULL, is_female);
		}

		(*CommFortProcess)(dwPluginID, 1001, bOutBuffer, len);
	} __finally {
		delete []bOutBuffer;
	}
}

// Выход виртуального пользователя из чата.
void UserDisconnect(String name) {
	DWORD len, pos_offset = 0, name_len;
	name_len = name.Length();

	//текст(имя)
	len = 4 + name_len * 2;
	BYTE * bOutBuffer = new char[len];
	try {
		SetStreamString(bOutBuffer, &pos_offset, name, name_len);
		//ShowMessage(String(main_thread_id) + " " + String(GetCurrentThreadId()));
		if (main_thread_id == GetCurrentThreadId()) {
			//ShowMessage(dwPluginID);
			(*CommFortProcess)(dwPluginID, 1002, bOutBuffer, len);
		} else {
			//ShowMessage("1");
			//ShowMessage(len);
			SendMessage(main_form->Handle, MF_CF_PROCESS, 1002, (LPARAM)&RequestData(bOutBuffer, len));
			//ShowMessage("2");
		}


	} __finally {
		delete []bOutBuffer;
	}
}

// Подключение виртуального пользователя к каналу.
bool UserChannelConnect(String user_name, String chnl_name, int visibility, int enter_type) {
	DWORD len, pos_offset = 0, user_name_len, chnl_name_len;
	user_name_len = user_name.Length();
	chnl_name_len = chnl_name.Length();
	bool result = false;

	//текст(имя виртуального пользователя) + текст(название канала) + число(видимость) + число(тип входа)
	//len = 4 + user_name_len.Length() * 2 + 4 + chnl_name_len.Length() * 2 + 4 + 4;

	len = 16 + (user_name_len + chnl_name_len) * 2;
	BYTE * bOutBuffer = new char[len];
	try {
		SetStreamString (bOutBuffer, &pos_offset, user_name, user_name_len);
		SetStreamString (bOutBuffer, &pos_offset, chnl_name, chnl_name_len);
		SetStreamString (bOutBuffer, &pos_offset, NULL, visibility);
		SetStreamString (bOutBuffer, &pos_offset, NULL, enter_type);

		(*CommFortProcess)(dwPluginID, 1026, bOutBuffer, len);

		result = true;
	} __finally {
		delete []bOutBuffer;
	}

	return result;
}

// Отключение виртуального пользователя от канала.
bool UserChannelDisconnect(String user_name, String chnl_name) {
	DWORD len, pos_offset = 0, user_name_len, chnl_name_len;
	user_name_len = user_name.Length();
	chnl_name_len = chnl_name.Length();
	bool result = false;

	//текст(имя виртуального пользователя) + текст(название канала)
	//len = 4 + user_name_len.Length() * 2 + 4 + chnl_name_len.Length() * 2;

	len = 8 + (user_name_len + chnl_name_len) * 2;
	BYTE * bOutBuffer = new char[len];
	try {
		SetStreamString (bOutBuffer, &pos_offset, user_name, user_name_len);
		SetStreamString (bOutBuffer, &pos_offset, chnl_name, chnl_name_len);

		(*CommFortProcess)(dwPluginID, 1027, bOutBuffer, len);
		result = true;
	} __finally {
		delete []bOutBuffer;
	}

	return result;
}

// Получение списка каналов, к которым подключен виртуальный пользователь.
// Возвращаеет список каналов.
StringList UserChannels(String user_name) {
	BYTE *pCopy,
		 *data = NULL,
		 *bOutBuffer = NULL;
	DWORD i, name_len, len,
		  users_sum = 0,
		  pos_offset = 0;
	String channel;
	StringList channels;

	try {
		name_len = user_name.Length();
		len = name_len * 2 + 4;
		bOutBuffer = new char[len];

		SetStreamString (bOutBuffer, &pos_offset, user_name, name_len);

		DWORD size = (*CommFortGetData)(dwPluginID, 1080, NULL, NULL, bOutBuffer, len);
		data = new BYTE[size];
		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1080, data, size, bOutBuffer, len);

		GetStreamDword (&pCopy, &users_sum);

		for (i = 0; i < users_sum; i++) {
			//channel
			GetStreamString (&pCopy, &channel);
			//channel users sum
			GetStreamDword (&pCopy, NULL);
			//channel theme
			GetStreamString (&pCopy, NULL);

			channels.push_back(channel);
		}
	} catch (Exception *E) {
		InfoAction("Произошла ошибка при получении списка каналов, к которым подключен бот. Ошибка: \"" + E->Message + "\"", false, 2, 2);
		channels.clear();
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;
	if (data) {
		delete []data;
		data = NULL;
		pCopy = NULL;
	}

	return channels;
}

// Установка состояния виртуальному пользователю.
void SetUserState(String name, String state) {
	/*
		ID: 1025
		Блок данных: текст(имя виртуального пользователя) + текст(новое состояние)
		len = 4 + name_len * 2 + 4 + state_len * 2;
	*/
	DWORD len, state_len, name_len, pos_offset;
	state_len = state.Length();
	name_len = name.Length();
	len = 8 + (name_len + state_len) * 2;
	BYTE * data = new char[len];
	try
	{
		pos_offset = 0;
		SetStreamString (data, &pos_offset, name, name_len);

		SetStreamString (data, &pos_offset, state, state_len);

		(*CommFortProcess)(dwPluginID, 1025, data, len);
	} __finally {
		delete []data;
	}
}

// Публикация сообщения от виртуального пользователя.
bool SendChannelMessage(String name, unsigned int rezim, String channel, String message) {
	/*
		ID: 1020
		Блок данных: текст(имя виртуального пользователя) + число(режим) + текст(канал) + текст(сообщение)
		len = 4 + bot_prop.name_len * 2 + 4 + 4 + channel_len * 2 + 4 + body_len * 2
	*/
	bool result = true;
	DWORD body_len, name_len, channel_len, len, pos_offset = 0;

	body_len = message.Length();
	name_len = name.Length();
	channel_len = channel.Length();
	len = 16 + (name_len + channel_len + body_len) * 2;

	BYTE *bOutBuffer = new char[len];
	try {
		SetStreamString (bOutBuffer, &pos_offset, name, name_len);
		SetStreamString (bOutBuffer, &pos_offset, NULL, rezim);
		SetStreamString (bOutBuffer, &pos_offset, channel, channel_len);
		SetStreamString (bOutBuffer, &pos_offset, message, body_len);

		(*CommFortProcess)(dwPluginID, 1020, bOutBuffer, len);
	} catch (Exception *E) {
		result = false;
	}

	delete []bOutBuffer;
	bOutBuffer = NULL;

	return result;
}
//---------------------------------------------------------------------------

