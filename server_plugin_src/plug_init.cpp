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

#include <IniFiles.hpp>

#include "plug_init.h"
#include "cf_functions.h"
#include "webchat.h"
#include "notif.h"
#include "users.h"
#include "online_users.h"
#include "http_messages_queue.h"
#include "db\channels_watch.h"
#include "auth_queue.h"
#include "debug.h"
#include "http_listen_thread.h"
#include "socket_listen_thread.h"
#include "l18n.h"
#include "user_channel.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

MapList watched_channels;

// Обновляет список IP-адресов из белого списка.
void GetWhiteIPs() {
	white_ips = new TStringList();
	white_ips->LoadFromFile(plugin_data_path + "white_ips.txt");
}

// Получает и записывает в глобальные переменные настройки из файла настроек.
void GetPluginOptions() {
	TIniFile *inifile = NULL;
	try {
		config_ini_path = plugin_data_path + "settings.ini";
		inifile = new TIniFile(config_ini_path);
//		bot_prop.name      =  inifile->ReadString("Bot Properties", "Nick", "FromWeb");
//		bot_prop.ip        =  inifile->ReadString("Bot Properties", "IP Address", "192.168.0.1");
//		bot_prop.pass      =  inifile->ReadString("Bot Properties", "Password", "veryStrongPass");
//		bot_prop.is_female =  inifile->ReadBool("Bot Properties", "IsFemale", false);
//		channels_policy    =  inifile->ReadBool("Bot Properties", "Channels Policy", false);
//		chnls_line         =  inifile->ReadString("Bot Properties", "Channels", "");
		tables_prefix      =  inifile->ReadString("DB Connection Properties", "Prefix", "");
		file_log           =  inifile->ReadBool("Additional Properties", "FileLogging", true);
		log_notifications  =  inifile->ReadBool("Additional Properties", "LogNotifications", false);
		show_work_time     =  inifile->ReadBool("Additional Properties", "ShowWorkTime", true);
		isfirstrun         =  inifile->ReadBool("Additional Properties", "FirstPlugRun", 0);
		sync_autostart     =  inifile->ReadBool("Additional Properties", "AutoStart", 1);
		language           =  inifile->ReadString("Additional Properties", "Language", "ru");
		data_storage       =  inifile->ReadString("Images", "LocalFolder", plugin_data_path + "images");
		check_version      =  inifile->ReadBool("Additional Properties", "AutoVersionCheck", true);
		auto_update        =  inifile->ReadBool("Additional Properties", "AutoUpdate", false);
		db_prop.host       =  inifile->ReadString("DB Connection Properties", "HostName", "");
		db_prop.user       =  inifile->ReadString("DB Connection Properties", "User_Name", "");
		db_prop.pass       =  inifile->ReadString("DB Connection Properties", "Password", "");
		db_prop.name       =  inifile->ReadString("DB Connection Properties", "Database", "");
	} __finally {
		if (inifile) {
			delete inifile;
		}
	}
}

// Инициализирует базовые настройки плагина.
void LoadBasePluginInfo() {
    // Get main thread id to compare it in imported functions.
	main_thread_id = GetCurrentThreadId();

	if (plugin_data_path == "") {
		plugin_data_path = GetPluginDirectory() + "WebChat\\";

		// Создаем подпапки для плагина и для логов плагина.
		if (!DirectoryExists(plugin_data_path)) {
			ForceDirectories(plugin_data_path);
		}
		if (!DirectoryExists(plugin_data_path + "logs\\")) {
			ForceDirectories(plugin_data_path + "logs\\");
		}
	}

	// Если список белых IP пуст - загружаем его.
	//if (!white_ips) {
	//	GetWhiteIPs();
	//}

	GetPluginOptions();

	// Записываем время начала работы плагина.
	work_time_begin = time(NULL);

	// Load options selected localization.
	LoadLocalizationFromFile(language);

	try {
		// Инициализируем файл логов.
		LogFileInit();
	} catch (Exception *E) {
		file_log = false;
		InfoAction(__("InitError"), false, 0, 1);
		file_log = true;
	}
}

// Проверяет опции, которые должны проверяться и выполняться при запуске плагина.
void CheckActionOptions() {
	if (sync_autostart) {
		StartSync();
	}
	if (check_version && main_form) {
    	main_form->cron_timer->Enabled = true;
	}
}
// Обновляет список каналов.void UpdateChannelsList() {
	CFChannelsList channels = GetChannelsList();

	try {
		if (!channels.empty()) {
			CFChannelsList::iterator channel;
			for (channel = channels.begin(); channel != channels.end(); channel++) {
				AddChannel(channel->name, channel->users_online, channel->topic);
			}
		} else {
			throw(Exception(__("NoChannelsInChat")));
		}
	} catch (Exception *E) {
		throw Exception(Format(__("UpdateChannelsError"), ARRAYOFCONST((E->Message))));
	}
}
// Обновляет список пользователей.
void UpdateUsersList() {
	CFUsersList cf_users_list = GetChatUsersList();
	CFUsersList cf_online_users_list = GetChatOnlineUsersList();

	int users_online_count = cf_online_users_list.size();

	CFUsersList::iterator registred_user, online_user;
	for (registred_user = cf_users_list.begin(); registred_user != cf_users_list.end(); registred_user++) {
		AddUser(registred_user->name, GetComputerID(registred_user->name), registred_user->ip, registred_user->sex, IsHiddenIP(registred_user->name));
	}

	CFUserInfo user_info;
	if (users_online_count > 0) {
		for (online_user = cf_online_users_list.begin(); online_user != cf_online_users_list.end(); online_user++) {
			user_info = GetUserInfo(online_user->name);
			AddOnlineUser(FindUserByName(online_user->name), user_info.state, user_info.client_version, user_info.process, user_info.window_activity, user_info.downtime);
		}
	}
}

// Выполняет действия по загрузке всей базовой информации.
bool SetBaseInfo() {
	bool success = false;

	// Прерываем обработку поступающих от сервера событий на время получения базовых данных.
	sync_enabled = false;

	if (main_form) {
		main_form->plug_sync_but->Enabled = true;
	}

	try {
		// Обновляем список каналов.
		UpdateChannelsList();

		// Полностью обновляем список пользователей.
		UpdateUsersList();

		success = true;
	} catch (Exception *E) {
		InfoAction(__("CacheLoadError") + ". " + E->Message, false, 0, 2);
	}

	return success;
}

// Включает синхронизацию.
void StartSync() {
	InitializeChannelsList();
	InitializeOnlineUsersList();
	InitializeUsersList();
	InitializeMessagesMap();
	InitializeAuthQueueMap();
	InitializeChannelsWatchersMap();
	InitializeUserChannelMap();

	bool success = SetBaseInfo();
	if (success) {
		InitializeCriticalSection(&connections_manipulation_cs);
		InitializeCriticalSection(&users_manipulation_cs);
		InitializeCriticalSection(&channels_manipulation_cs);

		// Запускаем прослушку порта.
		listen_stop = 0;
		success = SocketThreadStart();
		if (success) {
			success = HTTPThreadStart();
		}

		if (success) {
        	// Запускаем таймер проверки соединений.
			//conn_check_timer = new TConnCheckTimer();

			if (main_form) {
				main_form->plug_sync_but->Glyph->LoadFromResourceName((unsigned int)Instance, "plug_off");
				main_form->plug_sync_but->Hint = t_plug_off;
				main_form->plugin_state_item->Caption = t_plug_off;
				main_form->show_conn_but->Enabled = true;
				main_form->web_users_state->Enabled = true;
			}

			sync_enabled = true;

			MainEvent(__("SyncStarted"), 0);
		}
	}
}

void FinishSync() {
    // Отключаем прослушку портов.
	InterlockedIncrement(&listen_stop);
	HANDLE handles[2] = {HTTPThread, SocketThread};
	WaitForMultipleObjects(2, handles, true, INFINITE);

	if (main_form) {
		main_form->plug_sync_but->Glyph->LoadFromResourceName((unsigned int)Instance, "plug_on");

		main_form->show_conn_but->Enabled = false;

		main_form->web_users_state->Enabled = false;
		main_form->web_users_state->Caption = t_connections_off;

		main_form->plug_sync_but->Hint = t_plug_on;
		main_form->plugin_state_item->Caption = t_plug_on;
	}

	sync_enabled = false;

	MainEvent(__("SyncStopped"), 0);
	DeleteChannelsList();
	DeleteOnlineUsersList();
	DeleteUsersList();
	DeleteMessagesMap();
	DeleteAuthQueueMap();
	DeleteChannelsWatchersMap();
	DeleteUserChannelMap();
	DeleteCriticalSection(&connections_manipulation_cs);
	DeleteCriticalSection(&users_manipulation_cs);
	DeleteCriticalSection(&channels_manipulation_cs);
}
