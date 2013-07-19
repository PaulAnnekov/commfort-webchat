//---------------------------------------------------------------------------

#include <vcl.h>
#include <string.h>
#include "webchat.h"
#include "global.h"

DWORD dwPluginID;

typeCommFortProcess CommFortProcess;
typeCommFortGetData CommFortGetData;

extern "C" __declspec(dllexport) BOOL PluginStart(DWORD dwThisPluginID, typeCommFortProcess, typeCommFortGetData);
extern "C" __declspec(dllexport) VOID PluginStop();

extern "C" __declspec(dllexport) VOID PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize);
extern "C" __declspec(dllexport) DWORD PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize);


TMainForm *main_form;
TAboutForm *about_form;
TSettingsForm *settings_form;

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
   return 1;
}
//---------------------------------------------------------------------------
void plug_initialize()
{
	BYTE * data = NULL, *pCopy;
	TIniFile *inifile = NULL;
	try
	{
        if (plugin_data_path == "")
		{
			DWORD size = (*CommFortGetData)(dwPluginID, 2010, NULL, NULL, NULL, NULL);
			data = new BYTE[size];
			pCopy = data;

			(*CommFortGetData)(dwPluginID, 2010, data, size, NULL, NULL);
			get_stream_string(&pCopy, &plugin_data_path);

			plugin_data_path += "WebChat\\";

			if (!DirectoryExists(plugin_data_path))
				ForceDirectories(plugin_data_path);
			if (!DirectoryExists(plugin_data_path + "Logs\\"))
				ForceDirectories(plugin_data_path + "Logs\\");
		}

		if (!white_ips)
		{
			white_ips = new TStringList();
			white_ips->LoadFromFile(plugin_data_path + "white_ips.txt");
		}

		config_ini_path = plugin_data_path + "webchat_settings.ini";
		inifile = new TIniFile(config_ini_path);

		bot_name        =  inifile->ReadString("Bot Properties", "Nick", "FromWeb");
		bot_ip          =  inifile->ReadString("Bot Properties", "IP Address", "192.168.0.1");
		bot_pass        =  decode(inifile->ReadString("Bot Properties", "Password", "veryStrongPass"));
		bot_is_female   =  inifile->ReadBool("Bot Properties", "IsFemale", false);
		active_channel  =  inifile->ReadString("Bot Properties", "ActiveChannel", "main");
		tables_prefix   =  inifile->ReadString("BD Connection Properties", "Prefix", "");
		keep_conn       =  inifile->ReadInteger("BD Connection Properties", "KeepConnection", 0);
		visual_log      =  inifile->ReadBool("Additional Properties", "VisualLogging", true);
		file_log        =  inifile->ReadBool("Additional Properties", "FileLogging", true);
		auto_bd_clear   =  inifile->ReadBool("Additional Properties", "AutoBDClear", true);
		auto_log_clear  =  inifile->ReadBool("Additional Properties", "AutoLogClear", true);
		isfirstrun      =  inifile->ReadBool("Additional Properties", "FirstPlugRun", 0);
		auto_start      =  inifile->ReadBool("Additional Properties", "AutoStart", 1);
		conn_address    =  inifile->ReadString("BD Connection Properties", "HostName", "127.0.0.1");

	}
	__finally
	{
		if (data)
		{
			delete []data;
			if (inifile)
				delete inifile;
		}
	}

	is_initialized = true;
}

DWORD CreateInstanse(LPVOID)
{
if (!main_form)
			main_form = new TMainForm(NULL);
		main_form->Show();
return 1;
}
//---------------------------------------------------------------------------
BOOL PluginStart(DWORD dwThisPluginID, typeCommFortProcess func1, typeCommFortGetData func2)
{
	try
	{
		dwPluginID = dwThisPluginID;
	//При инициализации планину присваивается уникальный идентификатор
	//его необходимо обязательно сохранить, и указывать
	//в качестве первого параметра при инициировании событий

		CommFortProcess = func1;
	//указываем функцию обратного вызова,
	//с помощью которой плагин сможет инициировать события

		CommFortGetData = func2;
	//указываем функцию обратного вызова,
	//с помощью которой можно будет запрашивать необходимые данные от программы

		/*BYTE * data;
		(*CommFortGetData)(dwPluginID, 2000, data, 4, NULL, NULL);
		if ((DWORD) * data == 0) {*/

		if (!is_initialized) plug_initialize();

		//main_form
		if (!main_form)
			main_form = new TMainForm(NULL);
		main_form->Show();
		/*}
		else {
			MessageBox(Application->Handle, "Запуск плагина возможен только на серверном приложении", "Запуск не возможен", MB_OK + MB_ICONASTERISK + MB_TASKMODAL);
			result = false;
		}*/
	}
	catch (Exception *E)
	{
		MessageBox(Application->Handle, ("Произошла ошибка во время старта плагина. Ошибка: \"" + E->Message + "\"").t_str(), "Серьёзная ошибка", MB_OK + MB_ICONASTERISK + MB_TASKMODAL);
	}


	//Возвращаемые значения:
	//TRUE - запуск прошел успешно
	//FALSE - запуск невозможен
	return TRUE;
}
//---------------------------------------------------------------------------
VOID PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize)
  //Функция приема данных от программы к плагину
	//Параметры:
	//dwMessageID - идентификатор блока данных
	//bMessage - указатель на данные
	//dwMessageLength - указатель на переменную содержащую объем данных в байтах

	//Возвращаемое значение:
	//на данный момент не используется
{
	if (is_plugin_active) {
		UnicodeString bot, nick, ip, channel, text, state, query, datetime;
		BYTE * data, *pCopy;
		DWORD rezim, male;
		int line;
		TParams *params = new TParams;

		pCopy = bInBuffer;

		switch (dwID)
		{
			case 1070 : //Публикация в общий канал
				try {

				ShortDateFormat  = "yyyy-mm-dd";
				LongTimeFormat = "hh:mm:ss";
				datetime = DateToStr(Now())  + " " + TimeToStr(Now());

				//bot nick
				get_stream_string (&pCopy, &bot);
				//nick
				get_stream_string (&pCopy, &nick);
				//ip
				get_stream_string (&pCopy, NULL);
				//male
				get_stream_dword (&pCopy, &male);
				//channel
				get_stream_string (&pCopy, &channel);
				//rezim
				get_stream_dword (&pCopy, &rezim);
				//text
				get_stream_string (&pCopy, &text);

				//params->CreateParam(ftString, "text", ptInput);
				//params->ParamByName("text")->AsString = text;

				//query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`) VALUES (:text, \"%s\", \"%d\", \"%s\", \"%s\", \"%d\");",
				//		tables_prefix.w_str(), table_messages_name, nick, male, channel, datetime, rezim);
				query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`) VALUES (:text, \"%s\", \"%d\", \"%s\", \"%s\", \"%d\");",
						tables_prefix.w_str(), table_messages_name, nick, male, channel, datetime, rezim);


				main_form->add_new_action_list("Сообщение от пользователя " + String(nick) + " добавлено: \"" + String(text) + "\"", query, false, 0);
				if (main_form->establish_connection(false) == true)
					main_form->bd_sql_connection->ExecSQL(query, &Variant(text)/*params*/, NULL);
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события получения опубликованного сообщения. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1072: //Подключение к каналу стороннего пользователя
				try
				{
					ShortDateFormat  = "yyyy-mm-dd";
					LongTimeFormat = "hh:mm:ss";
					datetime = DateToStr(Now())  + " " + TimeToStr(Now());

					//bot_name
					get_stream_string (&pCopy, NULL);
					//channel
					get_stream_string (&pCopy, &channel);
					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, &ip);
					//male
					get_stream_dword (&pCopy, &male);

					if (channel == active_channel) {
						main_form->online_users_state->Caption = main_form->online_users_state->Caption.ToInt() + 1;
					}
					for (line = 0; line < white_ips->Count; line++)
						if (nick == white_ips->Strings[line])
						{
							ip = "N/A";
							line = white_ips->Count;
						}

					if (main_form->establish_connection(false))
					{
						query.sprintf(L"INSERT INTO `%s%s` (`nick`, `ip`, `male`) VALUES (\"%s\", '%s', '%d');",
									tables_prefix.w_str(), table_users_online, nick, ip, male
						);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
						main_form->add_new_action_list(
										"В канал " + channel + " зашёл пользователь: " + nick + " / "
										+ ip, query, false, 0
						);
						query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`)"
									" VALUES ('', \"%s\", \"%d\", \"%s\", '%s', '3');" ,
									tables_prefix.w_str(), table_messages_name, nick, male, channel, datetime
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
					}
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события входа пользователя в канал. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1078: //Вход в чат
				try
				{
					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, &ip);
					//male
					get_stream_dword (&pCopy, &male);

					if (nick == bot_name) {
						main_form->virt_user_init(true, NULL);
					}
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события входа пользователя в чат. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1073: //Отключение от канала стороннего или виртуального пользователя
				try
				{
					ShortDateFormat  = "yyyy-mm-dd";
					LongTimeFormat = "hh:mm:ss";
					datetime = DateToStr(Now())  + " " + TimeToStr(Now());

					get_stream_string (&pCopy, NULL);

					//channel
					get_stream_string (&pCopy, &channel);
					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, NULL);
					//male
					get_stream_dword (&pCopy, &male);

					main_form->online_users_state->Caption = main_form->online_users_state->Caption.ToInt() - 1;
					if (main_form->establish_connection(false))
					{
						query.sprintf(L"DELETE FROM `%s%s` WHERE `nick`=\"%s\";",
									tables_prefix.w_str(), table_users_online, nick
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
						main_form->add_new_action_list(
										"Из канала " + channel + " вышел пользователь: " + nick, query, false, 0
						);
						query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`)"
									" VALUES ('', \"%s\", \"%d\", \"%s\", '%s', '4');" ,
									tables_prefix.w_str(), table_messages_name, nick, male, channel, datetime
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
					}
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события выхода пользователя из канала. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1079: //Выход из чата
				try
				{
					ShortDateFormat  = "yyyy-mm-dd";
					LongTimeFormat = "hh:mm:ss";
					datetime = DateToStr(Now())  + " " + TimeToStr(Now());

					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, NULL);
					//male
					get_stream_dword (&pCopy, &male);

                    main_form->online_users_state->Caption = main_form->online_users_state->Caption.ToInt() - 1;

					if (main_form->establish_connection(false))
					{
						query.sprintf(L"DELETE FROM `%s%s` WHERE `nick`=\"%s\";",
									tables_prefix.w_str(), table_users_online, nick
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
						main_form->add_new_action_list(
										"Из чата вышел пользователь: " + nick, query, false, 0
						);
						query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`)"
									" VALUES ('', \"%s\", \"%d\", 'global', '%s', '4');" ,
									tables_prefix.w_str(), table_messages_name, nick, male, datetime
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
					}
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события выхода пользователя из чата. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1077: //Изменение состояния пользователя
				try
				{
					ShortDateFormat  = "yyyy-mm-dd";
					LongTimeFormat = "hh:mm:ss";
					datetime = DateToStr(Now())  + " " + TimeToStr(Now());

					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, NULL);
					//male
					get_stream_dword (&pCopy, &male);
					//state
					get_stream_string (&pCopy, &state);

					params->CreateParam(ftString, "state", ptInput);
					params->ParamByName("state")->AsString = state;

					if (nick != bot_name && main_form->establish_connection(false))
					{
						query.sprintf(L"UPDATE `%s%s` SET `state`=:state WHERE `nick`=\"%s\";",
									tables_prefix.w_str(), table_users_online, nick
								);
						main_form->bd_sql_connection->ExecSQL(query, &Variant(state), NULL);
						main_form->add_new_action_list(
										"Пользователь " + nick + " изменил статус на \""
										+ state + "\"", query, false, 0
						);

						params->ParamByName("state")->AsString = state;

						query.sprintf(L"INSERT INTO `%s%s` (`body`, `nick`, `male`, `channel`, `datetime`, `type`)"
									" VALUES (:state, \"%s\", \"%d\", 'global', '%s', '2');" ,
									tables_prefix.w_str(), table_messages_name, nick, male, datetime
								);
						main_form->bd_sql_connection->ExecSQL(query, &Variant(state), NULL);
					}
                }
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события изменения статуса пользователя. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1076: //Изменение иконки пользователя
				try
				{
					//name
					get_stream_string (&pCopy, &nick);
					//ip
					get_stream_string (&pCopy, NULL);
					//male
					get_stream_dword (&pCopy, &male);
					//new male
					get_stream_dword (&pCopy, &male);

					if (main_form->establish_connection(false))
					{
						query.sprintf(L"UPDATE `%s%s` SET `male`='%d' WHERE `nick`=\"%s\";",
									tables_prefix.w_str(), table_users_online, male, nick
								);
						main_form->bd_sql_connection->ExecSQL(query, NULL, NULL);
						main_form->add_new_action_list(
							"Пользователь " + nick + " изменил иконку на индекс: " + male,
							query, false, 0
						);
					}
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события изменения иконки пользователя. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;

			case 1090: //Авторизация виртуального пользователя невозможна
				try
				{
					//bot_name
					//get_stream_string (&pCopy, &nick);
					//error_code
					get_stream_dword (&pCopy, &rezim);

					main_form->virt_user_init(false, rezim);
				}
				catch (Exception *E)
				{
					main_form->add_new_action_list("Произошла ошибка при обработке события неудачной авторизации пользователя. Ошибка: \"" + E->Message + "\"", "", false, 2);
				}
			break;
		}
		delete params;
  }
}
DWORD PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize)
{
	//функция передачи данных программе
	//при значении dwOutBufferSize равным нулю функция должна вернуть объем данных, ничего не записывая

	DWORD result = 0;
	switch (dwID) {
		case 2800: //Предназначение плагина
			result = 4;
			if (dwOutBufferSize != 0) {
				DWORD pr_type = 1;
				memcpy(bOutBuffer, &pr_type, 4);
			}
			break;
		case 2810: //Название плагина
			int size = (plu_name + " " + plu_ver).Length();
			result = size * 2 + 4;
			if (dwOutBufferSize != 0) {
				memcpy(bOutBuffer, &size, 4);
				memcpy(bOutBuffer + 4, (plu_name + " " + plu_ver).c_str(), size * 2);
			}
			break;
	}

	return result;//возвращаемое значение - объем записанных данных
}
VOID PluginShowOptions()
{
	if (!settings_form)
	{
		if (!is_initialized)
			MessageBox(Application->Handle, "Для редактирования настроек надо запустить плагин", "Предупреждение",
			MB_OK + MB_ICONINFORMATION + MB_APPLMODAL);
		else
		{
			settings_form = new TSettingsForm(NULL);
			settings_form->Position = poMainFormCenter;
			settings_form->ShowModal();
			delete settings_form;
			settings_form = NULL;
		}
	}
	else
		settings_form->SetFocus();
}
//---------------------------------------------------------------------------
VOID PluginShowAbout()
{
	if (!about_form)
	{
		about_form = new TAboutForm(NULL);
		about_form->Position = poMainFormCenter;
		about_form->ShowModal();
		delete about_form;
		about_form = NULL;
	}
	else
		about_form->SetFocus();
}
//---------------------------------------------------------------------------
VOID PluginStop()
{
	try
	{
		if (main_form)
		{
			delete main_form;
			main_form = NULL;
		}
		if (white_ips)
		{
			delete white_ips;
			white_ips = NULL;
		}
		is_initialized = false;
		is_plugin_active = false;
	}
	catch (Exception *E)
	{
		MessageBox(Application->Handle, ("Произошла ошибка во время отключения плагина. Ошибка: \"" + E->Message + "\"").t_str(), "Серьёзная ошибка", MB_OK + MB_ICONASTERISK + MB_TASKMODAL);
	}
}
//---------------------------------------------------------------------------
