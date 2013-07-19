// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "first_config_form.h"
#include "webchat.h"
#include "global.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TFastConfigForm *FastConfigForm;

const int nothing_id = 0, begin_id = 1, host_id = 2, bd_name_id = 3, user_name_id = 4,
user_pass_id = 5, charset_id = 6, prefix_id = 7, keep_connect_id = 8, test_and_go_id = 9,
bot_nick_id = 10, bot_pass_id = 11, bot_male_id = 12, bot_ip_id = 13;

// ---------------------------------------------------------------------------
__fastcall TFastConfigForm::TFastConfigForm(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void TFastConfigForm::load_hint(int element_id) {
	String hint;
	switch(element_id) {
	case nothing_id:
		hint = "�������� �� ���� ��� ������, ����� �������� ���������";
		break;
	case begin_id:
		hint = "������� ��� ������, ����� ���������� � ��������� �������";
		break;
	case host_id:
		hint =
			"���� \"����� �������\" ������� ����� ������ �������, �� ������� "
			"������������� ���� ������ (��) MySQL. �������: localhost "
			"(���� �� ���������), 111.222.333.444, chat_url";
		break;
	case bd_name_id:
		hint = "���� \"�������� ��\" ������� ����� �������� ���� ������ "
			"MySQL, � ������� ����� ��������� ������. �������: test, my_base";
		break;
	case user_name_id:
		hint = "���� \"��� ������������\" ������� ����� ����� ������������ "
			"��� ����������� � ��. ������������ ������ ����� ����� ��� "
			"�������� � ��������� ������, � ����������. "
			"�������: root, user_1, admin";
		break;
	case user_pass_id:
		hint =
			"���� \"������\" ������� ����� ������ ��� ���������� ������������."
			" ��� ������ ������ ������ ���� �������, �� ��� ��� �� ��� �����";
		break;
	case charset_id:
		hint =
			"���� ������ \"��������� �������\" ������� ����� ��������� ������� MySQL. " "���� �� ����������� ������ WebChat ��� CMS Drupal, �� �������� " "��������� utf8, ��� ��� ��� ���������� ��� Drupal";
		break;
	case prefix_id:
		hint =
			"���� \"������� ������ ��\" ������� ����� ���������, ������� ����������� " "����� ��������� ������, ����������� ��������. ���������� ��� � ��������������� " "��������� � CMS Drupal";
		break;
	case keep_connect_id:
		hint =
			"���� ������ \"��������� ����������\" ������� ������ ������������ ���������� ��� ���. " "���� �� ���������� �������� � �����������, �� ��������� ��� ���� ��������";
		break;
	case test_and_go_id:
		hint =
			"������� ��� ������, ����� ����������� ����������� � ��. ���� ���������� �������, ��" "�� �������� � ���������� ����";
		break;
	case bot_nick_id:
		hint =
			"���� \"��� � ����\" ������� ����� ���� ����, ������� ����� ������������ � ����. " "�� ��� ����� ����� �������� ��������� �� ���-����������";
		break;
	case bot_pass_id:
		hint =
			"���� \"������\" ������� ����� ������ ��� ����. ����������, ��� ������ ������ ���� ���������� �������, ����� ��� ������ " "���� ������� ������� �������";
		break;
	case bot_male_id:
		hint =
			"���� ������ \"���\" ������� ������ ���� ����, �� ���� ������, ������� ����� ������������ � ����";
		break;
	case bot_ip_id:
		hint =
			"���� \"IP-�����\" ������� ����� IP-������ ����, ������� ����� ������������ � ��� ��������� ������� �� ��� ���� � ����";
		break;
	default:
		hint =
			"��� ����� ��������� ��������� ��� ��������� �� ����� ���� ���������";
		break;
	}
	hint_label->Caption = hint;
}

void __fastcall TFastConfigForm::host_addr_editMouseEnter(TObject *Sender) {
	load_hint(host_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bd_name_editMouseEnter(TObject *Sender) {
	load_hint(bd_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::charset_chooseMouseEnter(TObject *Sender) {
	load_hint(charset_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::user_name_editMouseEnter(TObject *Sender) {
	load_hint(user_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::pass_editMouseEnter(TObject *Sender) {
	load_hint(user_pass_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::prefix_editMouseEnter(TObject *Sender) {
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

void __fastcall TFastConfigForm::bot_male_chooseMouseEnter(TObject *Sender) {
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
void __fastcall TFastConfigForm::test_and_go_butClick(TObject *Sender) {
	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString("BD Connection Properties", "HostName",
		host_addr_edit->Text);
	inifile->WriteString("BD Connection Properties", "Database",
		bd_name_edit->Text);
	inifile->WriteString("BD Connection Properties", "User_Name",
		user_name_edit->Text);
	inifile->WriteString("BD Connection Properties", "Password",
		encode(pass_edit->Text));
	inifile->WriteString("BD Connection Properties", "ServerCharSet",
		charset_choose->Items->Strings[charset_choose->ItemIndex]);
	inifile->WriteString("BD Connection Properties", "Prefix",
		prefix_edit->Text);
	inifile->WriteBool("BD Connection Properties", "KeepConnection",
		keep_conn_check->Checked);
	delete inifile;
	if (main_form->establish_connection(true)) {
		MessageBox(Application->Handle,
			"���� ����������� ������ �������. ��������� ���������. ��������� � ���������� ����"
			, "�������� ����������", MB_OK + MB_ICONASTERISK + MB_APPLMODAL);
		pages->TabIndex = 2;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFastConfigForm::end_butClick(TObject *Sender) {
	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString("Bot Properties", "Nick", bot_nick_edit->Text);
	inifile->WriteString("Bot Properties", "Password", encode(bot_pass_edit->Text));
	inifile->WriteBool("Bot Properties", "IsFemale",bot_male_choose->ItemIndex);
	inifile->WriteString("Bot Properties", "IP", bot_ip_edit->Text);
	//------------------------
	if (main_form->establish_connection(false)) {
		bool is_final = 0;
		try {
			main_form->bd_sql_connection->ExecSQL(
				String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;", tables_prefix.w_str(), table_users_online), NULL, NULL
			);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL auto_increment,"
					"`nick` varchar(50) NOT NULL, "
					// ������������ ����� ����� - 40 ��������
					"`ip` varchar(100) NOT NULL, "
					"`male` int(1) NOT NULL default \"0\", "
					"`state` varchar(70) NOT NULL, "
					// ���� ������ ������� - 64 �������
					"PRIMARY KEY  (`id`)) DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(), table_users_online), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_messages_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`body` text NOT NULL," // ����. ���������� ������ ��������� 40000 ��������
					"`datetime` datetime NOT NULL,"
					"`nick` varchar(50) NOT NULL,"
					"`male` int(1) NOT NULL default \"0\", "
					"`channel` varchar(40) NOT NULL,"
					// ����. ����� ����� ������ 32 �������
					"`type` tinyint(1) NOT NULL," "PRIMARY KEY (`id`)"
					") DEFAULT CHARSET=utf8;", tables_prefix.w_str(),
							table_messages_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_mess_to_send_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`body` varchar(10000) NOT NULL,"
					"`datetime` datetime NOT NULL,"
					"`user` varchar(50) NOT NULL,"
					"`channel` varchar(40) NOT NULL,"
					"`type` tinyint(1) NOT NULL," "`ip` varchar(100) NOT NULL,"
					"PRIMARY KEY (`id`)" ") DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(),
							table_mess_to_send_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_settings_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`name` varchar(200) NOT NULL,"
					"`value` varchar(200) NOT NULL,"
					"PRIMARY KEY (`id`)" ") DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(),
							table_settings_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL(
				String().sprintf(L"INSERT INTO `%s%s` (`name`, `value`) VALUES ('bot_nick', '%s');",
					tables_prefix.w_str(), table_settings_name, bot_nick_edit->Text
				), NULL, NULL
			);
			// ������������� ������� ���������� ����������� �� ���� �����

			is_final = 1;
			main_form->bd_sql_connection->ExecSQL
				("SET @@global.wait_timeout=172800;", NULL, NULL);
		}
		catch(Exception * E) {
			if (is_final) {
				inifile->WriteInteger("BD Connection Properties",
					"FirstConfigRun", 0);
				MessageBox(NULL,
					"��������� �������� �� ������������. �������� �� ������� ���� ������������ ��� ��������� ����������. " "�� ������ ��������������� ���� ������, ��� ��� �� �� �������� ����� ������.", "��������������", MB_OK + MB_ICONWARNING + MB_TASKMODAL);
			}
			else
				MessageBox(NULL, (
					"�������� ������ ��� �������� ������ ��. ������ ����� ������: \""
					+ E->Message + "\". "
					"�������� �� ������� ���� ������������. ���������� ��������� ��������� �� � �������.").t_str(), "��������� ������", MB_OK + MB_ICONERROR + MB_TASKMODAL);
		}
		if (is_final) {
			if (MessageBox(Application->Handle,
					"�����������! ��������� ������� ������� ���������. �� ������ ����� �� ��������� � ���������" " ������ (������� \"���\", ����� �� ��������� ���� ������� ��������� � ��� ������ �����-���� ���������)?", "��������� ���������", MB_YESNO + MB_ICONASTERISK + MB_TASKMODAL) == 6)
			{
				is_plugin_active = true;
				inifile->WriteBool("Additional Properties", "FirstPlugRun", false);
				main_form->FormCreate(this);
				Close();
            }
		}
		// if (test)
		// MessageBox(Application->Handle, "���������� � �� ������� �����������", "���� �������", MB_OK + MB_ICONINFORMATION + MB_TASKMODAL + MB_TOPMOST);
	}
	delete inifile;
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::FormCreate(TObject *Sender)
{
	String charset;

	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);

		// ��������� � ���� �������� ��������� ����������� � ������� MySQL
		host_addr_edit->Text      = inifile->ReadString("BD Connection Properties", "HostName", "localhost");
		bd_name_edit->Text        =  inifile->ReadString("BD Connection Properties","Database", "drupal");
		user_name_edit->Text      =  inifile->ReadString("BD Connection Properties","User_Name", "root");
		pass_edit->Text           =  decode(inifile->ReadString("BD Connection Properties","Password", ""));

		charset                   =  inifile->ReadString("BD Connection Properties", "ServerCharSet", "utf8");
		int i = 0;
		do {
			if (charset == charset_choose->Items->Strings[i]) {
				charset_choose->ItemIndex = i;
				break;
			}
			i += 1;
		}
		while (i > charset_choose->Items->Count - 1);

		prefix_edit->Text         =  tables_prefix;
		keep_conn_check->Checked  =  keep_conn;
	}
	__finally
	{
		if (inifile) {
			delete inifile;
			inifile = NULL;
		}
	}
	// ��������� � ���� �������� �������� ����
	bot_nick_edit->Text = bot_name;
	bot_pass_edit->Text = decode(bot_pass);
	bot_ip_edit->Text = bot_ip;
	bot_male_choose->ItemIndex = bot_is_female;

}
//---------------------------------------------------------------------------

