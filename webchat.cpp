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
#include <string.h>
#include "webchat.h"
#include "global.h"
#include "cf_functions.h"
#include "notif.h"
#include "cf_events.h"
#include "plug_init.h"

DWORD dwPluginID;

typeCommFortProcess CommFortProcess;
typeCommFortGetData CommFortGetData;

extern "C" __declspec(dllexport) BOOL PluginStart(DWORD dwThisPluginID, typeCommFortProcess, typeCommFortGetData);
extern "C" __declspec(dllexport) VOID PluginStop();

extern "C" __declspec(dllexport) VOID PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize);
extern "C" __declspec(dllexport) DWORD PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize);

TAboutForm *about_form;
TSettingsForm *settings_form;

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
   Instance = hinst;
   return 1;
}
//---------------------------------------------------------------------------
void plug_initialize() {
//	BYTE * data = NULL, *pCopy;
//	String chnls_line;
//
//	TIniFile *inifile = NULL;
//	try {
//		if (plugin_data_path == "") {
//			DWORD size = (*CommFortGetData)(dwPluginID, 2010, NULL, NULL, NULL, NULL);
//			data = new BYTE[size];
//			pCopy = data;
//
//			(*CommFortGetData)(dwPluginID, 2010, data, size, NULL, NULL);
//			GetStreamString(&pCopy, &plugin_data_path);
//
//			plugin_data_path += "WebChat\\";
//
//			if (!DirectoryExists(plugin_data_path))
//				ForceDirectories(plugin_data_path);
//			if (!DirectoryExists(plugin_data_path + "Logs\\"))
//				ForceDirectories(plugin_data_path + "Logs\\");
//		}
//
//		if (!white_ips) {
//			white_ips = new TStringList();
//			white_ips->LoadFromFile(plugin_data_path + "white_ips.txt");
//		}
//
//		config_ini_path = plugin_data_path + "webchat_settings.ini";
//		inifile = new TIniFile(config_ini_path);
//
//		bot_prop.name      =  inifile->ReadString("Bot Properties", "Nick", "FromWeb");
//		bot_prop.ip        =  inifile->ReadString("Bot Properties", "IP Address", "192.168.0.1");
//		bot_prop.pass      =  inifile->ReadString("Bot Properties", "Password", "veryStrongPass");
//		bot_prop.is_female =  inifile->ReadBool("Bot Properties", "IsFemale", false);
//		channels_policy    =  inifile->ReadBool("Bot Properties", "Channels Policy", false);
//		chnls_line         =  inifile->ReadString("Bot Properties", "Channels", "");
//		tables_prefix      =  inifile->ReadString("DB Connection Properties", "Prefix", "");
//		file_log           =  inifile->ReadBool("Additional Properties", "FileLogging", true);
//		auto_db_clear      =  inifile->ReadBool("Additional Properties", "AutoBDClear", true);
//		show_work_time     =  inifile->ReadBool("Additional Properties", "ShowWorkTime", true);
//		no_white_ips       =  inifile->ReadBool("Additional Properties", "NoWhiteIPs", false);
//		isfirstrun         =  inifile->ReadBool("Additional Properties", "FirstPlugRun", 0);
//		sync_autostart         =  inifile->ReadBool("Additional Properties", "AutoStart", 1);
//		db_prop.host       =  inifile->ReadString("DB Connection Properties", "HostName", "");
//		db_prop.user       =  inifile->ReadString("DB Connection Properties", "User_Name", "");
//		db_prop.pass       =  Base64Decode(inifile->ReadString("DB Connection Properties", "Password", ""));
//		db_prop.name       =  inifile->ReadString("DB Connection Properties", "Database", "");
//		//auto_log_clear     =  inifile->ReadBool("Additional Properties", "AutoLogClear", true);
//		//visual_log         =  inifile->ReadBool("Additional Properties", "VisualLogging", true);
//	} __finally {
//		if (data) {
//			delete []data;
//			if (inifile) {
//				delete inifile;
//			}
//		}
//	}
//
//    // ���������� ����� ������ ������ �������.
//	work_time_begin = time(NULL);
//
//	//��������� ������ �������
//	condit_chnls = new TStringList();
//	if (chnls_line.Length() != 0) {
//		int pos = chnls_line.Pos("|");
//		if (pos == 0) {
//			condit_chnls->Add(chnls_line);
//		} else {
//			while (chnls_line.Pos("|") != 0) {
//				condit_chnls->Add(chnls_line.SubString(1, pos-1));
//				chnls_line.Delete(1, pos);
//				pos = chnls_line.Pos("|");
//			}
//			condit_chnls->Add(chnls_line);
//		}
//	}
//
//	//������ ������ ������ ������������� �������
//	watched_chnls = new TStringList();
//
//	//������� ������ ���������� � mysql
//	db_conn = new MySQLConn();
//
//	plugin_started = true;
}

//---------------------------------------------------------------------------
BOOL PluginStart(DWORD dwThisPluginID, typeCommFortProcess func1, typeCommFortGetData func2) {
	try {
		dwPluginID = dwThisPluginID;
	//��� ������������� ������� ������������� ���������� �������������
	//��� ���������� ����������� ���������, � ���������
	//� �������� ������� ��������� ��� ������������� �������

		CommFortProcess = func1;
	//��������� ������� ��������� ������,
	//� ������� ������� ������ ������ ������������ �������

		CommFortGetData = func2;
	//��������� ������� ��������� ������,
	//� ������� ������� ����� ����� ����������� ����������� ������ �� ���������
		if (!plugin_started) plug_initialize();

		LoadBasePluginInfo();
		plugin_started = true;

		if (!main_form) {
			main_form = new TMainForm(Application);
		}
		main_form->Show();

		CheckActionOptions();

	} catch (Exception *E) {
		ShowMsg(Application->Handle, Format(e_plug_start, ARRAYOFCONST((E->Message))), 1);
	}

	//������������ ��������:
	//TRUE - ������ ������ �������
	//FALSE - ������ ����������
	return TRUE;
}

//������� ������ ������ �� ��������� � �������
	//���������:
		//dwMessageID - ������������� ����� ������
		//bMessage - ��������� �� ������
		//dwMessageLength - ��������� �� ���������� ���������� ����� ������ � ������

	//������������ ��������:
		//�� ������ ������ �� ������������
VOID PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize) {
	if (sync_enabled) {
		BYTE *pCopy = bInBuffer;
		try {
			switch (dwID) {
				case 1070 : // ���������� � ����� �����.
					OnPublicChannelMessage(pCopy);
				break;

				case 1062: // ����������� � ������ ������������ ������������.
					OnVirtualUserChannelConnect(pCopy);
				break;

				case 1072: // ����������� � ������ ���������� ������������.
					OnUserChannelConnect(pCopy);
				break;

				case 1078: // ���� � ���.
					OnUserConnect(pCopy);
				break;

				case 1073: // ���������� �� ������ ���������� ��� ������������ ������������.
					OnUserChannelDisconnect(pCopy);
				break;

				case 1079: // ����� �� ����.
					OnUserDisconnect(pCopy);
				break;

				case 1077: // ��������� ��������� ������������.
					OnUserStateChanged(pCopy);
				break;

				case 1076: // ��������� ������ ������������.
					OnUserSexChanged(pCopy);
				break;

				case 1071: // ��������� ����.
					OnChannelTopicChanged(pCopy);
				break;

				case 1090: // ����������� ������������ ������������ ����������.
					OnUnsuccessAuth(pCopy);
				break;
				case 1081: // New user registered.
					OnUserRegistered(pCopy);
				break;
					/*case 1062: //����������� � ������ ������������ ������������
					try
					{
						//bot nick
						GetStreamString (&pCopy, &bot);
						//channel
						GetStreamString (&pCopy, &channel);
						//channel theme
						GetStreamString (&pCopy, NULL);
						//channel greeting
						GetStreamString (&pCopy, NULL);

						query = Format(
							"INSERT INTO `%s%s` (`variable`, `nick`, `male`, `channel`, `type`, `datetime`)"
							" VALUES (\"%s\", \"%s\", '%d', \"%s\", '4', '%d')",
							ARRAYOFCONST((tables_prefix, tbl_names.actions, ip, nick, male, channel, timestamp))
						);
						if (mysql_connection->query(query))
							InfoAction(Format(n_user_channel_connect, ARRAYOFCONST((channel, nick, ip))), false, 0);
						}
					}
					catch (Exception *E)
					{
						InfoAction(Format(e_user_channel_connect, ARRAYOFCONST((E->Message))), false, 2);
					}
				break; */
			}
		} catch (String error) {
			InfoAction(error, false, 2, 1);
		}
	}
}
DWORD PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize)
{
	//������� �������� ������ ���������
	//��� �������� dwOutBufferSize ������ ���� ������� ������ ������� ����� ������, ������ �� ���������

	DWORD result = 0;
	switch (dwID) {
		case 2800: //�������������� �������
			result = 4;
			if (dwOutBufferSize != 0) {
				DWORD pr_type = 1;
				memcpy(bOutBuffer, &pr_type, 4);
			}
			break;
		case 2810: //�������� �������
			String name = GetPluginFullName();
			int size = name.Length();
			result = size * 2 + 4;
			if (dwOutBufferSize != 0) {
				memcpy(bOutBuffer, &size, 4);
				memcpy(bOutBuffer + 4, name.c_str(), size * 2);
			}
			break;
	}

	return result;//������������ �������� - ����� ���������� ������
}
VOID PluginShowOptions() {
	if (!settings_form) {
		if (!plugin_started) {
			LoadBasePluginInfo();
		}
		settings_form = new TSettingsForm(Application);
		settings_form->Position = poMainFormCenter;
		settings_form->ShowModal();
		delete settings_form;
		settings_form = NULL;
	} else {
		settings_form->SetFocus();
	}
}
//---------------------------------------------------------------------------
VOID PluginShowAbout() {
	if (!about_form) {
		about_form = new TAboutForm(Application);
		about_form->Position = poMainFormCenter;
		about_form->ShowModal();

		//delete about_form;
		about_form = NULL;
	} else {
		about_form->SetFocus();
	}
}
//---------------------------------------------------------------------------
VOID PluginStop() {
	try {
		FinishSync();

		if (main_form) {
			delete main_form;
			main_form = NULL;
		}
		if (white_ips) {
			delete white_ips;
			white_ips = NULL;
		}
//		if (condit_channels) {
//			delete condit_channels;
//			condit_channels = NULL;
//		}

		plugin_started = false;

		//��� ����������� ���������� ������� ��������� ���� �����
		FileClose(file_pos);
	} catch (Exception *E) {
		ShowMsg(Application->Handle, Format(e_plug_stop, ARRAYOFCONST((E->Message))), 1);
	}
}
//---------------------------------------------------------------------------

