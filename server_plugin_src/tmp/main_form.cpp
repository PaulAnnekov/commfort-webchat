//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "main_form.h"
#include "webchat.h"
#include "global.h"
#include "first_config_form.h"
#include "connections_form.h"
#include "cf_functions.h"
#include <ShellApi.h>
#include <DateUtils.hpp>
#include <stdio.h>
#include "notif.h"
#include "check_conn.h"
#include "log_form.h"
#include "plug_init.h"
#include "l18n.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFastConfigForm *first_config_form;

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

int TMainForm::DoAction(TMessage &Message) {
	try {
		web_users_state->Caption = (Message.LParam > 0) ? String(Message.LParam) : __("NoConnections");
	} catch(Exception *E) {
		InfoAction("Error during Main form connections synchronization: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during Main form connections synchronization", false, 3, 2);
	}

	return 1;
}
//---------------------------------------------------------------------------

void TMainForm::bot_denied(int error_code) {
//������� ����������� ���� ����������� ��������
	sync_enabled = false;
	plugin_state->Caption = "��������";
	plug_sync_but->Caption = "��������";
	plugin_state_item->Caption = "�������� ������";

	ShowMsg(Handle, "��� �� ���� ����� � ��� �� ��������� �������: \n\"" + e_bot_auth[error_code] + "\"", 1);
}
//---------------------------------------------------------------------------
void TMainForm::bot_connected() {


//	// ��������� ������ ������� � ������ ���� � ��� ������.
//	get_channels_list();
//
//	bool sucess = true;
//	String query;
//	int i, chnls_count = watched_chnls->Count;
//
//	// ��������� ������ ������������� ������� �� �� �������.
//	if (chnls_count != 0) {
//		// �.�. ��� ����� ��������� �������� � 16-�� ������� - ������� ����������� ������ � ������� ���������� ������.
//		if (chnls_count > 16) {
//			MainEvent("���������� ������������� ������� ����� 16-��. ��� ����� ��������� � ������� ��� 16-�� �������. "
//				"������������� ����� ������ 16 ������� �� ������. �� ������ ������� ������ ������ ��� ������ �� ������ "
//				"� ����������", 0);
//			for (i = 16; i < chnls_count; i++) {
//				watched_chnls->Delete(i);
//			}
//			chnls_count = 16;
//		}
//
//		query = Format(
//			"TRUNCATE TABLE `%s%s`;"
//			"INSERT INTO `%s%s` (`name`, `topic`) VALUES ",
//			ARRAYOFCONST((tables_prefix, tbl_names.channels, tables_prefix, tbl_names.channels))
//		);
//
//		String *active_chnls;
//		int active_count = virt_user_channels(bot_prop.name, NULL);
//		active_chnls = new String[active_count];
//		virt_user_channels(bot_prop.name, active_chnls);
//
//		int index;
//		// ���� channels != NULL, �� ���� ���� �� �������� ������ ��� ���������� ������� ��������.
//		if (active_chnls != NULL) {
//			for (i = 0; i < chnls_count; i++) {
//				index = ListSearch(active_chnls, watched_chnls->Names[i], active_count);
//				// ���� � ������ �������, � ������� ����� ������������ ������ ������ ����� (�� ���� ��� ��� � ����)
//				// ���������, �� �������� ���� ����� - ������� ����� � ����� �� ���������� ������� ���������
//				// ������������, ������ ��� �� �� ���� �����������.
//				if (index != -1) {
//					active_chnls[index] = NULL;
//				}
//				else {
//					sucess = virt_user_chnl_conn(bot_prop.name, watched_chnls->Names[i]);
//				}
//			}
//
//			// ��������� ���� �� �������, � ������� �� ���������, � �� �� ���� �����������.
//			for (i = 0; i < active_count; i++) {
//				if (active_chnls[i] != NULL) {
//					virt_user_chnl_disconn(bot_prop.name, active_chnls[i]);
//				}
//			}
//
//			// ������� ���������� ������ ���������� ������ � ������� virt_user_channels.
//			delete []active_chnls;
//
//			// ���� ��������� ���������� ������������ � ����� ���� ������� (������ � ������� ���������), �� ��������� ������.
//			if (sucess) {
//				for (int i = 0; i < chnls_count; i++) {
//					query += Format("(\"%s\", \"%s\")", ARRAYOFCONST((watched_chnls->Names[i], db_conn->get_escaped(watched_chnls->ValueFromIndex[i]))));
//					if (i != chnls_count - 1)
//						query += ", ";
//
//					// ������� �������� ��� ������� �� ������� �����=����.
//					watched_chnls->Strings[i] = watched_chnls->Names[i];
//				}
//
//				sucess = db_conn->query(query, 0);
//			}
//		}
//		else {
//			sucess = false;
//		}
//
//		// ���� ��������� ���������� ������������ ���� ��������� ��� ������ ��� ���������, �� ���������� ������.
//		if (!sucess)
//			InfoAction("��������� ������ �� ����� ����������� ���� � �������. �������� ����������� ����������� � �� ��� �� �������� ������������ � ������", true, 0, 1);
//	}
//	else {
//		InfoAction("������ ������������� ������� ����. ��������� ��������� ������ ������� ��� ������������", true, 0, 0);
//	}
//
//	// ��������� ��������� ������ ������������� ��� �����������\�������������� ����������.
//	update_users_list();
//
//	// ��������� ��������� �����.
//	port_listener = new TPortListener(true);
//	port_listener->Start();
//
//	// ��������� ������ �������� ����������.
//	conn_check_timer = new TConnCheckTimer();
//
//	// ������������� ���� ���������.
//	set_user_state(bot_prop.name, "���-��������� ������");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::exception_handler(TObject *Sender, Exception *E) {
  MainEvent("����������� ������: \"" + E->Message + "\" � �������� \"" + E->ClassName() + "\" � �������� " + Sender->ClassName(), 2);
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
	open_window_item->Caption = "������� ����";
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
    	// ��������� ��������� ������ ������������� � �� � ������ ������������ ������������=�����.
		UpdateUsersTable();
		MainEvent(n_users_list_updated, 0);    
	} catch (String error) {
		MainEvent(error, 1);
	}
}
//---------------------------------------------------------------------------
void TMainForm::set_window_height() {
	// ������������� ����������� ������ ���� �������� ������ �������� ����, ������� ����� ���� ����� ������.
	int bar_height = Height - ClientHeight;
	Constraints->MaxHeight = 170 + bar_height;
	Constraints->MinHeight = 170 + bar_height;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::open_log_butClick(TObject *Sender) {
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
void __fastcall TMainForm::plug_sync_butClick(TObject *Sender) {
	if (sync_enabled) {
		FinishSync();
	} else {
		StartSync();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::stop_plugin_butClick(TObject *Sender) {
	int id = ShowMsg(Handle, "�� ������������� ������ ��������� ������ �������?"
				" ��� ������� � ������ ��������� ��� ������.", 3);
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
		open_window_item->Caption = "������� ����";
	}
	else {
		Show();
		open_window_item->Caption = "�������� ����";
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
	stop_plugin_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plugin_state_itemClick(TObject *Sender) {
	plug_sync_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::work_time_timerTimer(TObject *Sender) {
	work_time_timer->Enabled = false;

	// ��������� �����.
	time_plus_state->Caption = get_time_offset(work_time_begin);

	work_time_timer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender){
	if (isfirstrun) {
		int id = ShowMsg(Handle, "������ ������� ������� � ������� ������������ ���������. "
			"������ ��������� ������� ������� ��������� (� ��������� ������ ��������� "
			"���� ������ ���������)?", 3);

		if (id == IDYES) {
			load_first_config_run_form();
		}
		else {
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
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	// ��������� ������ ������ ������� ������ ������� � �������� ����� �������, ���� ������������ ������� ��� �����.
	if (show_work_time) {
		time_plus_lbl->Visible = true;
		time_plus_state->Visible = true;
		work_time_timer->Enabled = true;
	}

	//set_window_height();
	Application->OnException = exception_handler;

	Caption = plu_name + " " + plu_ver;
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




