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

#pragma hdrstop

#include "main_form.h"
#include "webchat.h"
#include "global.h"
#include "first_config_form.h"
#include "connections_form.h"
#include "connections.h"
#include "cf_functions.h"
#include <ShellApi.h>
#include <DateUtils.hpp>
#include <stdio.h>
#include "notif.h"
#include "debug.h"
#include "log_form.h"
#include "plug_init.h"
#include "update_form.h"
#include "l18n.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFastConfigForm *first_config_form;
// Set initial time to new version check (24 hours in minutes);
int time_to_version_check = 1440;

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

int TMainForm::DoAction(TMessage &Message) {
	try {
		if (Message.WParam == 0) {
			web_users_state->Caption = (Message.LParam > 0) ? String(Message.LParam) : __("NoConnections");
		} else if (Message.WParam == 1) {
        	void (*callback)() = (void(*)())Message.LParam;
			callback();
		}
	} catch(Exception *E) {
		InfoAction("Error during Main form connections synchronization: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during Main form connections synchronization", false, 3, 2);
	}

	return 1;
}
//---------------------------------------------------------------------------

int TMainForm::CFProcessRequest(TMessage &Message) {
	try {
		RequestData *request_data = (RequestData *) Message.LParam;
		(*CommFortProcess)(dwPluginID, Message.WParam, request_data->data, request_data->len);
	} catch(Exception *E) {
		InfoAction("Error during main thread messages synchronization: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during main thread messages synchronization", false, 3, 2);
	}

	return 1;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::exception_handler(TObject *Sender, Exception *E) {
  MainEvent("Неизвестная ошибка: \"" + E->Message + "\" с пометкой \"" + E->ClassName() + "\" в элементе " + Sender->ClassName(), 2);
}
//---------------------------------------------------------------------------
void TMainForm::load_first_config_run_form() {
	first_config_form = new TFastConfigForm(NULL);
	first_config_form->pages->TabIndex = 0;
	first_config_form->ShowModal();
	delete first_config_form;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action) {
	open_window_item->Caption = "Открыть окно";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::tray_iconClick(TObject *Sender) {
	open_window_item->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::load_first_config_butClick(TObject *Sender) {
	sync_enabled = false;
	load_first_config_run_form();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::online_users_butClick(TObject *Sender) {
	try {
    	// Полностью обновляем список пользователей в БД и таблиц соответствий пользователь=канал.
		UpdateUsersList();
		MainEvent(n_users_list_updated, 0);    
	} catch (String error) {
		MainEvent(error, 1);
	}
}
//---------------------------------------------------------------------------
void TMainForm::set_window_height() {
	// Устанавливаем статическую высоту окна учитывая высоту верхнего бара, которая может быть везде разная.
	int bar_height = Height - ClientHeight;
	Constraints->MaxHeight = 170 + bar_height;
	Constraints->MinHeight = 170 + bar_height;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plug_sync_butClick(TObject *Sender) {
	if (sync_enabled) {
		FinishSync();
	} else {
		StartSync();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::stop_plugin_butClick(TObject *Sender) {
	int id = ShowMsg(Handle, "Вы действительно хотите завершить работу плагина?"
				" Это приведёт к полной остановке его работы.", 3);
	if (id == IDYES) {
		(*CommFortProcess)(dwPluginID, 2100, NULL, NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::about_butClick(TObject *Sender) {
	PluginShowAbout();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::settings_butClick(TObject *Sender) {
	PluginShowOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::open_window_itemClick(TObject *Sender) {
	if (Visible) {
		Close();
		open_window_item->Caption = "Открыть окно";
	}
	else {
		Show();
		open_window_item->Caption = "Свернуть окно";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::settings_itemClick(TObject *Sender) {
	PluginShowOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::about_itemClick(TObject *Sender) {
	PluginShowAbout();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plugin_terminate_itemClick(TObject *Sender) {
	//stop_plugin_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plugin_state_itemClick(TObject *Sender) {
	plug_sync_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::work_time_timerTimer(TObject *Sender) {
	work_time_timer->Enabled = false;

	// Обновляем время.
	time_plus_state->Caption = get_time_offset(work_time_begin);

	work_time_timer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender){
	if (isfirstrun) {
		int id = ShowMsg(Handle, "Плагин запущен впервые и требует обязательной настройки. Внесите настройки во всех разделах", 4);

		TIniFile *inifile = NULL;
		try {
			inifile = new TIniFile(config_ini_path);
			inifile->WriteBool("Additional Properties", "FirstPlugRun", false);
		}
		__finally {
			if (inifile) {
				delete inifile;
			}
		}

		PluginShowOptions();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	last_log_memo->Hint = __("LastLogHint");

	// Запускаем таймер показа времени работы плагина и включаем показ времени, если пользователь выбирал эту опцию.
	if (show_work_time) {
		time_plus_lbl->Visible = true;
		time_plus_state->Visible = true;
		work_time_timer->Enabled = true;
	}

	if (check_version) {
        cron_timer->Enabled = true;
	}

	Application->OnException = exception_handler;

	Caption = GetPluginFullName();
	version_label->Caption = plugin_version;
	tray_icon->Hint = Caption;

	errors_warnings_update();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::show_conn_butClick(TObject *Sender)
{
	if (!conn_list_form) {
		conn_list_form = new TConnectionsList(Application);
		conn_list_form->Position = poMainFormCenter;
		conn_list_form->Show();
	} else {
		conn_list_form->BringToFront();
	}
}
//---------------------------------------------------------------------------

void TMainForm::errors_warnings_update() {
	errors_state->Caption = String(errors_c) + " / " + String(warnings_c);
}

void __fastcall TMainForm::open_log_butClick(TObject *Sender)
{
	errors_c = 0;
	warnings_c = 0;
	errors_warnings_update();

	if (!log_form) {
		log_form = new TLogForm(Application);
		log_form->Position = poMainFormCenter;
		log_form->Show();
	} else {
		log_form->BringToFront();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::version_labelClick(TObject *Sender)
{
	if (!update_form) {
		update_form = new TUpdateForm(NULL);
	}
	update_form->ShowModal();
	delete update_form;
	update_form = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cron_timerTimer(TObject *Sender)
{
	DeleteExpiredConnections();
	time_to_version_check -= 5;
	if (time_to_version_check == 0) {
		time_to_version_check = 1440;
		GetVersionInfo();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::settings_buttonClick(TObject *Sender)
{
	PluginShowOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	ShowTimeMeasures();
}
//---------------------------------------------------------------------------

