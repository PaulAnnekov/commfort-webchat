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

// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "first_config_form.h"
#include "webchat.h"
#include "global.h"
#include "notif.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TFastConfigForm *FastConfigForm;

const int nothing_id = 0, begin_id = 1, host_id = 2, bd_name_id = 3, user_name_id = 4,
user_pass_id = 5, charset_id = 6, prefix_id = 7, keep_connect_id = 8, test_and_go_id = 9,
bot_nick_id = 10, bot_pass_id = 11, bot_male_id = 12, bot_ip_id = 13;

String hints [14] = {
	"�������� �� ���� ��� ������, ����� �������� ���������",
	"������� ��� ������, ����� ���������� � ��������� �������",
	"���� \"����� �������\" ������� ����� ������ �������, �� ������� "
			"������������� ���� ������ (��) MySQL. �������: localhost "
			"(���� �� ���������), 111.222.333.444, chat_url",
	"���� \"�������� ��\" ������� ����� �������� ���� ������ "
			"MySQL, � ������� ����� ��������� ������. �������: test, my_base",
	"���� \"��� ������������\" ������� ����� ����� ������������ "
			"��� ����������� � ��. ������������ ������ ����� ����� ��� "
			"�������� � ��������� ������, � ����������. "
			"�������: root, user_1, admin",
	"���� \"������\" ������� ����� ������ ��� ���������� ������������."
			" ��� ������ ������ ������ ���� �������, �� ��� ��� �� ��� �����",
	"���� ������ \"��������� �������\" ������� ����� ��������� ������� MySQL. " "���� �� ����������� ������ WebChat ��� CMS Drupal, �� �������� " "��������� utf8, ��� ��� ��� ���������� ��� Drupal",
	"���� \"������� ������ ��\" ������� ����� ���������, ������� ����������� " "����� ��������� ������, ����������� ��������. ���������� ��� � ��������������� " "��������� � CMS Drupal",
	"���� ������ \"��������� ����������\" ������� ������ ������������ ���������� ��� ���. " "���� �� ���������� �������� � �����������, �� ��������� ��� ���� ��������",
	"������� ��� ������, ����� ����������� ����������� � ��. ���� ���������� �������, ��" "�� �������� � ���������� ����",
	"���� \"��� � ����\" ������� ����� ���� ����, ������� ����� ������������ � ����. " "�� ��� ����� ����� �������� ��������� �� ���-����������",
	"���� \"������\" ������� ����� ������ ��� ����. ����������, ��� ������ ������ ���� ���������� �������, ����� ��� ������ " "���� ������� ������� �������",
	"���� ������ \"���\" ������� ������ ���� ����, �� ���� ������, ������� ����� ������������ � ����",
	"���� \"IP-�����\" ������� ����� IP-������ ����, ������� ����� ������������ � ��� ��������� ������� �� ��� ���� � ����"
};

// ---------------------------------------------------------------------------

__fastcall TFastConfigForm::TFastConfigForm(TComponent* Owner) : TForm(Owner) {
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::FormCreate(TObject *Sender)
{
	//��������� � ���� �������� �������� ���������� � ��
	db_host_edit->Text = db_prop.host;
	db_name_edit->Text = db_prop.name;
	db_user_edit->Text = db_prop.user;
	db_pass_edit->Text = db_prop.pass;
	db_prf_edit->Text  = tables_prefix;

	// ��������� � ���� �������� �������� ����
	bot_nick_edit->Text       = bot_prop.name;
	bot_pass_edit->Text       = bot_prop.pass;
	bot_ip_edit->Text         = bot_prop.ip;
	bot_sex_choose->ItemIndex = bot_prop.is_female;
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::test_and_go_butClick(TObject *Sender) {
	db_prop.host  = db_host_edit->Text;
	db_prop.name  = db_name_edit->Text;
	db_prop.user  = db_user_edit->Text;
	db_prop.pass  = db_pass_edit->Text;
	tables_prefix = db_prf_edit->Text;

	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString("DB Connection Properties", "HostName", db_prop.host);
	inifile->WriteString("DB Connection Properties", "Database", db_prop.name);
	inifile->WriteString("DB Connection Properties", "User_Name", db_prop.user);
	inifile->WriteString("DB Connection Properties", "Password", db_prop.pass);
	inifile->WriteString("DB Connection Properties", "Prefix", tables_prefix);
	delete inifile;

//	if (�onnect(event_db_conn, true) == NULL) {
//		InfoAction("���� ����������� ������ �������. ��������� ���������. ��������� � ���������� ����", true, 0, 0);
//		pages->TabIndex = 2;
//	} else {
//		InfoAction("�������� ������ ��� ����� ����������. ���������� ������ ����� ���������� � �����."
//							 "���������� ��������� ��������� �� � �������",
//							 true, 0, 1
//		 );
//	}
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::end_butClick(TObject *Sender) {
	bot_prop.name       =  bot_nick_edit->Text;
	if (bot_prop.pass != bot_pass_edit->Text)
		bot_prop.pass   = GetMD5(bot_pass_edit->Text);
	bot_prop.ip         =  bot_ip_edit->Text;
	bot_prop.is_female  =  bot_sex_choose->ItemIndex;

	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString ("Bot Properties",        "Nick",         bot_prop.name);
	inifile->WriteString ("Bot Properties",        "Password",     bot_prop.pass);
	inifile->WriteBool   ("Bot Properties",        "IsFemale",     bot_prop.is_female);
	inifile->WriteString ("Bot Properties",        "IP Address",   bot_prop.ip);
	inifile->WriteBool   ("Additional Properties", "FirstPlugRun", false);
	delete inifile;

	// ������� ��������� ��������� �������, � ���� �� ��� false, �� � ����� ������� ������.
	//bool success = SetBaseDB();
	//if (success) {
		if (ShowMsg(Handle, "�����������! ��������� ������� ������� ���������. �� ������ ����� �� ���������"
				" (������� \"���\", ����� �� ��������� ���� ������� ��������� � ��� ������ �����-���� ���������)?", 3) == IDYES) {
			Close();
		}
	//}
}
// ---------------------------------------------------------------------------

void TFastConfigForm::load_hint(int element_id) {
	String hint = hints[element_id];
	hint_label->Caption = hint;
}

void __fastcall TFastConfigForm::db_host_editMouseEnter(TObject *Sender) {
	load_hint(host_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::edit12MouseEnter(TObject *Sender) {
	load_hint(bd_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::charset_chooseMouseEnter(TObject *Sender) {
	load_hint(charset_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::db_user_editMouseEnter(TObject *Sender) {
	load_hint(user_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::pass_editMouseEnter(TObject *Sender) {
	load_hint(user_pass_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::db_prf_editMouseEnter(TObject *Sender) {
	load_hint(prefix_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::keep_conn_chooseMouseEnter(TObject *Sender) {
	load_hint(keep_connect_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::test_and_go_butMouseEnter(TObject *Sender) {
	load_hint(test_and_go_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_nick_editMouseEnter(TObject *Sender) {
	load_hint(bot_nick_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_pass_editMouseEnter(TObject *Sender) {
	load_hint(bot_pass_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_sex_chooseMouseEnter(TObject *Sender) {
	load_hint(bot_male_id);
}
//---------------------------------------------------------------------------
void __fastcall TFastConfigForm::bot_ip_editMouseEnter(TObject *Sender)
{
	load_hint(bot_ip_id);
}
//---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet1MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet3MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet2MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::begin_butMouseEnter(TObject *Sender) {
	load_hint(begin_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::begin_butClick(TObject *Sender) {
	pages->TabIndex = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::keep_conn_checkMouseEnter(TObject *Sender) {
	load_hint(keep_connect_id);
}
// ---------------------------------------------------------------------------



void __fastcall TFastConfigForm::db_name_editMouseEnter(TObject *Sender)
{
	load_hint(bd_name_id);
}
//---------------------------------------------------------------------------


