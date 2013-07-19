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

}

//---------------------------------------------------------------------------
BOOL PluginStart(DWORD dwThisPluginID, typeCommFortProcess func1, typeCommFortGetData func2) {
	try {
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

	//Возвращаемые значения:
	//TRUE - запуск прошел успешно
	//FALSE - запуск невозможен
	return TRUE;
}

//Функция приема данных от программы к плагину
	//Параметры:
		//dwMessageID - идентификатор блока данных
		//bMessage - указатель на данные
		//dwMessageLength - указатель на переменную содержащую объем данных в байтах

	//Возвращаемое значение:
		//на данный момент не используется
VOID PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize) {
	if (sync_enabled) {
		BYTE *pCopy = bInBuffer;
		try {
			switch (dwID) {
				case 1070 : // Публикация в общий канал.
					OnPublicChannelMessage(pCopy);
				break;

				case 1062: // Подключение к каналу виртуального пользователя.
					OnVirtualUserChannelConnect(pCopy);
				break;

				case 1072: // Подключение к каналу стороннего пользователя.
					OnUserChannelConnect(pCopy);
				break;

				case 1078: // Вход в чат.
					OnUserConnect(pCopy);
				break;

				case 1073: // Отключение от канала стороннего или виртуального пользователя.
					OnUserChannelDisconnect(pCopy);
				break;

				case 1079: // Выход из чата.
					OnUserDisconnect(pCopy);
				break;

				case 1077: // Изменение состояния пользователя.
					OnUserStateChanged(pCopy);
				break;

				case 1076: // Изменение иконки пользователя.
					OnUserSexChanged(pCopy);
				break;

				case 1071: // Изменение темы.
					OnChannelTopicChanged(pCopy);
				break;

				case 1090: // Авторизация виртуального пользователя невозможна.
					OnUnsuccessAuth(pCopy);
				break;
				case 1081: // New user registered.
					OnUserRegistered(pCopy);
				break;
			}
		} catch (String error) {
			InfoAction(error, false, 2, 1);
		}
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
			String name = GetPluginFullName();
			int size = name.Length();
			result = size * 2 + 4;
			if (dwOutBufferSize != 0) {
				memcpy(bOutBuffer, &size, 4);
				memcpy(bOutBuffer + 4, name.c_str(), size * 2);
			}
			break;
	}

	return result;//возвращаемое значение - объем записанных данных
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
		plugin_started = false;

		//при уничтожении отключение плагина закрываем файл логов
		FileClose(file_pos);
	} catch (Exception *E) {
		ShowMsg(Application->Handle, Format(e_plug_stop, ARRAYOFCONST((E->Message))), 1);
	}
}
//---------------------------------------------------------------------------

