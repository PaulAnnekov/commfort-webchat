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
#include "notif.h"
#include "global.h"
#include "log_form.h"
#include "l18n.h"
#include <StrUtils.hpp>

#pragma package(smart_init)
//---------------------------------------------------------------------------

const String
	e_sql_on_init = "�� ������� ���������������� ����������. ������������ ������",
	e_cant_connect = "�� ���� ������������ � �� �� ���� \"%s\". ������: \"%s\"",
	e_sql_syntax = "%s. �������� ������ � ���������� �������. ������: \"%s\". ������: \"%s\"",
	e_mysql_query = "%s. �� ������� ��������� ������ � ��. ������: \"%s\"",
	e_db_base_create = "�������� ������ ��� �������� ������ ��. ������: \"%s\".\n"
					   "�������� �� ������� ���� ������������. ���������� ��������� ��������� �� � �������",

	e_plug_start = "��������� ������ �� ����� ������ �������. ������: \"%s\"",
	e_plug_stop = "��������� ������ �� ����� ���������� �������. ������: \"%s\"",

	e_event_error                  = "��������� ������ ��� ��������� �������",
	e_user_mess_recieve            = e_event_error + " ��������� ��������������� ���������. ������: \"%s\"",
	e_user_channel_connect         = e_event_error + " ����� ������������ � �����. ������: \"%s\"",
	e_virtual_user_channel_connect = e_event_error + " ����� ������������ ������������ � �����. ������: \"%s\"",
	e_user_connect                 = e_event_error + " ����� ������������ � ���. ������: \"%s\"",
	e_user_channel_disconnect      = e_event_error + " ������ ������������ �� ������. ������: \"%s\"",
	e_user_disconnect              = e_event_error + " ������ ������������ �� ����. ������: \"%s\"",
	e_user_state_changed           = e_event_error + " ��������� ������� ������������. ������: \"%s\"",
	e_user_sex_changed             = e_event_error + " ��������� ������ ������������. ������: \"%s\"",
	e_chnl_theme_changed           = e_event_error + " ��������� ���� ������. ������: \"%s\"",
	e_unsuccess_auth               = e_event_error + " ��������� ����������� ������������. ������: \"%s\"",

	e_bot_auth[9] = {
		"��������� ������������ ����� ������������ �������������. ���� ���������� ����� ��� ����",
		"��� ���� �� ������������� ����������� (�������� ��������� �������, ���� ��������� ������������ ����� � 40 ��������). �������� ��� ���� � ���������� �������",
		"��� ���� ������ � ������� �������� (���). ������� ���",
		"��� ���� �������� ����������� ����� (�� �������� ������ ������ ����). �������� ��� ���� � ���������� �������",
		"������ �� ��������� ��� ��� ����������������. �������� ��� ���� � ���������� �������",
		"��������� ������������ ����� ������� ������� � ������� IP-������. ������� IP � ���������� � ���������� ������� ��� �������� IP � ���������� �������",
		"��� ����� � ������� �� ��������� ������ �� ���������. ����������� ������� ������ ����",
		"�������� ������",
		"������ �� ��������� ���� �� ����������, ���� ���������. ��������� ��������� ������"
	},
	e_bot_base_load = "������ �� ����� �������� ������� ������ ����� ����������� ����",

	e_tcp_addr = "��������� ������ �� ����� ��������� ������� ����������� ��������� ��������. IP-����� ������� �� ������",
	e_tcp_listen= "���������� ������������ � ��������� �����. ������: \"%s\"",
	e_tcp_connection_create = "���������� ������� ����������� ����������. ������: \"%s\"",
	e_tcp_write = "������ ������ � ����� �������. ������: \"%s\"",
	e_tcp_read = "������ ������ �� ������ �������. ������: \"%s\"",
	e_tcp_during_listen = "������ ������� �� ����� ��������� �������� ����������. ������: \"%s\"",
	e_tcp_socket_accept = "���������� ������� �������� �����������. ������: \"%s\"",
	e_tcp_socket_close = "�������� ��� �������� ����������. ������: \"%s\"",
	e_tcp_connections_check = "���������� ��������� �������� �������� ����������. ������: \"%s\"",
	e_tcp_client_during_listen = "������ ������� �� ����� ��������� �������� ����������. ������: \"%s\"",
	e_tcp_client_during_read = "������ ������� �� ����� ������ �������. ������: \"%s\"",

	e_notif_db = "������ ��� ���������� ������� �� ���������� ������� ��� HTTP ��������. ������: \"%s\"",

	e_image_unique = "������ ��� ������� ����������� ����� ��� �����������, ����������� � ���������",
	e_image_open = "������ ��� �������� ����� ����������� �� ������. �������� ������������ ����",
	e_image_write = "������ ��� ������ � ���� �����������. �������� ������������ ����",
	e_image_close = "������ ��� �������� ����� �����������",
	e_image_unexpected = "����������� ������ ��� ���������� �����������",

	n_connect_success = "���������� � �� �����������",
	n_disconnect_success = "���������� � �� ���������",

	n_user_message_recieved = "��������� �� ������������ %s ���������: \"%s\"",
	n_user_channel_connect = "� ����� %s ����� ��������� ������������: %s / %s",
	n_virtual_user_channel_connect = "� ����� %s ����� ����������� ������������: %s",
	n_user_connect = "� ��� ����� ������������: %s",
	n_user_channel_disconnect = "�� ������ %s ����� ������������: %s",
	n_user_disconnect = "�� ���� ����� ������������: %s",
	n_user_state_changed = "������������ %s ������� ������ �� \"%s\"",
	n_user_sex_changed = "������������ %s ������� ������ �� ������: %d",
	n_channel_theme_changed = "���� ������ \"%s\" �������� ��: \"%s\" ������������� \"%s\"",

    n_plugin_sync_started = "������ ������� ������� ��������",
	n_users_list_updated = "������ ������������� ������� ��������",

	a_sql_streams[3] = {"����� �������", "����� http ��������", "����� tcp ��������"},

	// ��������� ��� ���������� ���� ������� MessageBox.
	sm_warning = "��������������",
	sm_error = "������",
	sm_question = "�������� �������",
	sm_asterisk = "����������",

	// ����� ���������� ���������.
	t_show_connection_button = "�������� ����������",
	t_db_connection_yes = "����������",
	t_db_connection_no = "�����������",
	t_plug_off = "��������� ������",
	t_plug_on = "�������� ������",
	t_plug_started = "���������",
	t_plug_offline = "��������",
	t_connections_off = "�����������",
	t_online_users_unknown = "����������",
	t_bot_auth_wait = "����������� ����...",

	// ���������� ����� ��� ���������� ������.
	l_group_prefix[4] = {"�����", "��", "�������", "����������"};

String messages[3]    = {"notice", "warn", "error"}, //������ ��������� ������������ ��������� ������� � ������ �����
	   last_init_date = NULL;                        //��������� ���� ������������� (������������ ��� ��������� � ������� ����� � ����� ����� � � ������� ����� ��� ������� ��)
int file_pos = NULL;            			 	     //handle ��� ����� �����

ofstream log_file;

void LofFileFlush() {
    // Write all buffered data to assosiated file.
	/* TODO : ������������� ������������������ �������� */
	log_file.flush();
}

// �������� ���� �� ����� � Memo ����.
void LoadLog() {
	TStringList *log_file_list = NULL;
	TFileStream *temp = NULL;

	log_file_list = new TStringList();

	LofFileFlush();

	// �������� � log_file �� ����� �������, ������� ��� ���� �����.
	temp = new TFileStream(plugin_data_path + "Logs\\" + last_init_date + ".log", fmOpenRead | fmShareDenyNone);
	try {
		log_file_list->LoadFromStream(temp);

		int pos, int_type, l_count;
		String time, group, line, type;
		l_count = log_file_list->Count;
		int i = (l_count > 200) ? l_count - 201 : 0;

		for (i; i < l_count; i++) {
			line = log_file_list->Strings[i].Trim();

			pos = line.Pos("]");
			time = line.SubString(0, pos);
			line.Delete(1, pos + 1);

			pos = line.Pos("]");
			type = line.SubString(2, pos - 2);
			line.Delete(1, pos + 1);

			if (type == "notice") {
				int_type = 0;
			}
			else if (type == "warn") {
				int_type = 1;
			}
			else if (type == "error") {
				int_type = 2;
			}

			pos = line.Pos("]");
			group = line.SubString(2, pos - 2);
			line.Delete(1, pos + 1);

			RTFInsert(Format("%s. %s\r\n", ARRAYOFCONST((group, line))), int_type);
			RTFInsert(Format("%s ", ARRAYOFCONST((time))), 0);
		}
	}
	__finally {
		if (log_file_list) {
			delete log_file_list;
		}
		if (temp) {
			delete temp;
		}
	}
}

// ����� ���������.
int ShowMsg(HWND handle, String main_t, int type) {
// type - ��� � � InfoAction + 3 - ������
	UINT icon_type, btns_type;
	String caption_t = "";
	btns_type = MB_OK;

	switch (type) {
		case 1:
			icon_type = MB_ICONWARNING;
			caption_t = sm_warning;
			break;
		case 2:
			icon_type = MB_ICONERROR;
			caption_t = sm_error;
			break;
		case 3:
			icon_type = MB_ICONQUESTION;
			caption_t = sm_question;
			btns_type = MB_YESNO;
			break;
		default:
			icon_type = MB_ICONASTERISK;
			caption_t = sm_asterisk;
			break;
	}

	int id = MessageBox(handle, AnsiString(main_t).c_str(), AnsiString(caption_t).c_str(), btns_type + icon_type + MB_TASKMODAL);

	return id;
}

// ����� ��������� � Memo ����.
void RTFInsert(String text, int type) {
	HWND__ *h_Edit = log_form->states_list_rich->Handle;

	//EnterCriticalSection(&cr_sec);
	// ������������� ������� ������� ������. cpMin = cpMax - ������ ��� ��������� �����, � �� �������.
	CHARRANGE cr;
	cr.cpMin = 0;
	cr.cpMax = 0;
	SendMessage(h_Edit, EM_EXSETSEL, 0, (LPARAM)&cr);

	// ������������� ���� �� ������� ������.
	CHARFORMAT cf;
	cf.cbSize = sizeof cf;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = color[type];

	// ��������� ��������� ��������� �������������� �� ����� ������ ����� ������� �������.
	SendMessage(h_Edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);

	// Insert text to current position (beggining). Using inline UnicodeString convertion to fix bug with data copy to new pointer.
	SendMessage(h_Edit, EM_REPLACESEL, 0, (LPARAM)(AnsiString(text).c_str()));

	//LeaveCriticalSection(&cr_sec);
}

// ������������� ����� �����.
void LogFileInit() {
	String full_log_path;
	String curr_date;
	DateTimeToString (curr_date, "yyyy.mm.dd", Date());
	if (curr_date != last_init_date) {
		if (log_file.is_open()) {
			log_file.close();
			if (log_file.fail()) {
				throw Exception(__("LogFileCloseError"));
			}
		}
		full_log_path = plugin_data_path + "logs\\" + curr_date + ".log";
		log_file.open(AnsiString(full_log_path).c_str(), ios::out | ios::app | ios::binary);
		if (!log_file.is_open()) {
			throw Exception(__("LogFileOpenError"));
		}
//		if (file_pos) {
//			FileClose(file_pos);
//		}
//		full_log_path = plugin_data_path + "logs\\" + curr_date + ".log";
//		if (FileExists(full_log_path)) {
//			file_pos = FileOpen(full_log_path, fmOpenWrite | fmShareDenyNone);
//			FileSeek(file_pos, 0, 2);
//		}
//		else {
//			file_pos = FileCreate(full_log_path, fmShareDenyNone, NULL);
//		}
		last_init_date = curr_date;
	}
}

// ������� ��� ��������� ������ �������������� �� �������.
void SocketWarn(String text) {
	InfoAction(text, false, 3, 1);
}

// ������� ��� ��������� ������ ����� �����.
void MainEvent(String text, int type) {
	InfoAction(text, false, 0, type);
}

// �������� \ ���������� �����������.
void InfoAction(String text, bool show_message, int group, int type) {
/*
	group - ������ �����������:
		0 - �����
		1 - ��
		2 - �������
		3 - ����������
	type - ��� �����������:
		0 - ������� �����������
		1 - ��������������
		2 - ����������� ������
*/
	String text_log, text_vis;

	// ������� ������� ������ ������ � �������� �������, ����� � ����� �� ���� ������ ��������� �����.
	text = ReplaceStr(text, "\n", " ");
	text = ReplaceStr(text, "\r", " ");

	if (file_log || visual_log) {
		String time = FormatDateTime("hh:nn:ss", Time());
		text_log = Format("[%s] [%s] [%s] %s.", ARRAYOFCONST((time, messages[type], l_group_prefix[group], text)));

		if (file_log) {
			try {
				LogFileInit();
				log_file.write(AnsiString(text_log + "\r\n").c_str(), text_log.Length() + 2);
				if (log_file.bad()) {
					throw Exception(__("LogFileWriteError"));
				}
				if (type != 0) {
					LofFileFlush();
				}
			} catch (Exception *E) {
				RTFInsert(Format("%s. " + __("LogFileError") + ".\r\n", ARRAYOFCONST((l_group_prefix[0], E->Message))), 1);
			}
			//FileWrite(file_pos, AnsiString(text_log + "\r\n").c_str(), text_log.Length() + 2);
		}

		if (log_form) {
			RTFInsert(Format("%s. %s.\r\n", ARRAYOFCONST((l_group_prefix[group], text))), type);
			RTFInsert("[" + time + "] ", 0);
		}

		// ��������� ������� �� ��� ����������� � ���� ���, �� ������� ������\������������� ������������� � ������� ����� ��������.
		if (main_form) {
			main_form->last_log_memo->Text = Format("[%s] %s. %s.", ARRAYOFCONST((time, l_group_prefix[group], text)));
			main_form->last_log_memo->Font->Color = (TColor)color[type];
			if (type != 0) {
				(type == 1) ? warnings_c++ : errors_c++;
				/* TODO : ������� ���������������� ����� ������� ���������� �������� ������ � �������������� */
				main_form->errors_warnings_update();
			}
		}
	}

	if (show_message) {
		ShowMsg(Application->Handle, text, type);
    }
}
