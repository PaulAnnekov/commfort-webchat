//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "settings_form.h"
#include "global.h"
#include "webchat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TSettingsForm *SettingsForm;

//---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TSettingsForm::save_bd_config()
//процедура сохранения конфигурации. производится проверка данных в полях.
{
	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);
		inifile->WriteString("BD Connection Properties", "HostName", Edit1->Text);
		inifile->WriteString("BD Connection Properties", "Database", Edit2->Text);
		inifile->WriteString("BD Connection Properties", "User_Name", Edit3->Text);
		inifile->WriteString("BD Connection Properties", "Password", encode(Edit4->Text));
		inifile->WriteString("BD Connection Properties", "ServerCharSet", ComboBox1->Items->Strings[ComboBox1->ItemIndex]);
		inifile->WriteString("BD Connection Properties", "Prefix", Edit6->Text);
		inifile->WriteInteger("BD Connection Properties", "KeepConnection", keep_conn_edit->ItemIndex);
	}
	__finally
	{
		if (inifile)
			delete inifile;
	}

	keep_conn = keep_conn_edit->ItemIndex;
	tables_prefix = Edit6->Text;
}
void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);
		int i;
		String charset;

		//подгрузка в поля значений настройки подключения к серверу MySQL
		Edit1->Text = inifile->ReadString("BD Connection Properties", "HostName", "localhost");
		Edit2->Text = inifile->ReadString("BD Connection Properties", "Database", "drupal");
		Edit3->Text = inifile->ReadString("BD Connection Properties", "User_Name", "root");
		Edit4->Text = inifile->ReadString("BD Connection Properties", "Password", "");
		Edit6->Text = inifile->ReadString("BD Connection Properties", "Prefix", "");
		keep_conn_edit->ItemIndex = keep_conn;
		charset = inifile->ReadString("BD Connection Properties", "ServerCharSet", "не указано");
		i = 0;
		do
		{
			if (charset == ComboBox1->Items->Strings[i])
			{
				ComboBox1->ItemIndex = i;
				break;
			}
			i+=1;
		}
		while (i > ComboBox1->Items->Count - 1);
		/*full_refresh_set_choose->ItemIndex = inifile->ReadInteger("Additional Properties", "FullAutoRefresh", 0);
		full_refresh_set_choose->OnSelect(Sender);
		time_interval_set_edit->Text = inifile->ReadString("Additional Properties", "RefreshInterval", "0");
		time_set_pick->Time = inifile->ReadTime("Additional Properties", "RefreshTime", Now());  */
	}
	__finally
	{
		if (inifile) {
			delete inifile;
			inifile = NULL;
		}
	}

    //подгрузка в поля значений настроек бота
	chat_nick_edit->Text           =  bot_name;
	bot_pass_edit->Text            =  bot_pass;
	bot_ip_edit->Text              =  bot_ip;
	bot_male_choose->ItemIndex     =  bot_is_female;
	active_channel_edit->Text      =  active_channel;

	//подгрузка в поля белого списка IP
	white_ip_memo->Lines           =  white_ips;

	//подгрузка в поля значений дополнительных настроек
	visual_log_check->Checked      =  visual_log;
	file_log_check->Checked        =  file_log;
	auto_bd_clear_check->Checked   =  auto_bd_clear;
	auto_log_clear_check->Checked  =  auto_log_clear;
	auto_start_check->Checked      =  auto_start;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::bot_sett_butClick(TObject *Sender)
{
	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);
		inifile->WriteString("Bot Properties", "Nick", chat_nick_edit->Text);
		inifile->WriteString("Bot Properties", "Password", encode(bot_pass_edit->Text));
		inifile->WriteBool("Bot Properties", "IsFemale", bot_male_choose->ItemIndex);
		inifile->WriteString("Bot Properties", "IP", bot_ip_edit->Text);
		inifile->WriteString("Bot Properties", "ActiveChannel", active_channel_edit->Text);
	}
	__finally
	{
		if (inifile)
			delete inifile;
	}
	bot_name = chat_nick_edit->Text;
	active_channel = active_channel_edit->Text;
	//сохраняем настройки ника бота
	if (main_form->establish_connection(false))
		main_form->bd_sql_connection->ExecSQL(
			String().sprintf(L"UPDATE `%s%s` SET `value` = '%s' WHERE `name` = 'bot_nick';",
				tables_prefix.w_str(), table_settings_name, bot_name
			), NULL, NULL
		);
		main_form->bd_sql_connection->ExecSQL(
			String().sprintf(L"UPDATE `%s%s` SET `value` = '%s' WHERE `name` = 'active_channel';",
				tables_prefix.w_str(), table_settings_name, active_channel
			), NULL, NULL
		);

	main_form->add_new_action_list("Настройки бота сохранены и будут ПРИМЕНИНЫ после перезагрузки плагина", "", true, 0);
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::save_add_set_butClick(TObject *Sender)
{
	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);
		inifile->WriteBool("Additional Properties", "VisualLogging", visual_log_check->Checked);
		inifile->WriteBool("Additional Properties", "FileLogging", file_log_check->Checked);
		inifile->WriteBool("Additional Properties", "AutoLogClear", auto_log_clear_check->Checked);
		inifile->WriteBool("Additional Properties", "AutoBDClear", auto_bd_clear_check->Checked);
		inifile->WriteBool("Additional Properties", "AutoStart", auto_start_check->Checked);
		/*inifile->WriteInteger("Additional Properties", "FullAutoRefresh", full_refresh_set_choose->ItemIndex);
		inifile->WriteInteger("Additional Properties", "RefreshInterval", StrToInt(time_interval_set_edit->Text));
		inifile->WriteTime("Additional Properties", "RefreshTime", time_set_pick->Time);*/
	}
	__finally
	{
		if (inifile)
			delete inifile;
	}

	visual_log      =  visual_log_check->Checked;
	file_log        =  file_log_check->Checked;
	auto_log_clear  =  auto_log_clear_check->Checked;
	auto_bd_clear   =  auto_bd_clear_check->Checked;

	main_form->add_new_action_list("Дополнительные настройки успешно сохранены", "", true, 0);
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::save_white_ipsClick(TObject *Sender)
{
	white_ip_memo->Lines->SaveToFile(plugin_data_path + "white_ips.txt");
	main_form->add_new_action_list("Настройки фильтрации IP сохранены", "", true, 0);
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::save_set_butClick(TObject *Sender)
{
	save_bd_config();
	main_form->add_new_action_list("Настройки подключения к MySQL серверу сохранены", "", true, 0);
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::test_conn_butClick(TObject *Sender)
{
	save_bd_config();
	if (main_form->establish_connection(true))
		MessageBox(Application->Handle,
			"Тест подключения прошёл успешно. Настройки сохранены."
			, "Успешное соединение", MB_OK + MB_ICONASTERISK + MB_APPLMODAL);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::load_first_config_butClick(TObject *Sender)
{
	main_form->load_first_config_run_form();
}
//---------------------------------------------------------------------------
void __fastcall TSettingsForm::full_refresh_set_chooseSelect(TObject *Sender)
{
	switch (full_refresh_set_choose->ItemIndex)
	{
		case 0:
			time_interval_set_edit->Enabled = false;
			time_set_pick->Enabled = false;
		break;
		case 1:
			time_interval_set_edit->Enabled = true;
			time_interval_set_label->Caption  = "Интервал времени в Минутах";
			time_set_pick->Enabled = false;
		break;
		case 2:
			time_interval_set_edit->Enabled = true;
			time_interval_set_label->Caption  = "Интервал времени в Сутках (24 часа)";
			time_set_pick->Enabled = true;
		break;
	}
}
//---------------------------------------------------------------------------


