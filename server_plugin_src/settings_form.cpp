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

#include "settings_form.h"
#include "global.h"
#include "webchat.h"
#include "notif.h"
#include "l18n.h"
#include <FileCtrl.hpp>
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

void TSettingsForm::add_del_string_list(TButton *btn, TEdit *edit, TListBox *list) {
	if (btn->Caption == "+")
	{
		if (edit->Text.Trim() != "")
		{
			list->Items->Add(edit->Text);
			edit->Text = "";
			edit->SetFocus();
		}
		else MessageBox(Handle, "Сперва надо вписать название канала", "Предупреждение",
		MB_OK + MB_ICONWARNING + MB_APPLMODAL);
	}
	else
	{
		if (list->ItemIndex != -1) {
			list->Items->Delete(list->ItemIndex);
			if (list->Items->Count != 0) {
				list->SetFocus();
				list->ItemIndex = 0;
			}
			else {
				edit->SetFocus();
			}
		}
		else {
			MessageBox(Handle, "Сперва надо выделить канал, который надо удалить из списка", "Предупреждение",
				MB_OK + MB_ICONWARNING + MB_APPLMODAL);
		}
	}
}
//---------------------------------------------------------------------------

void TSettingsForm::SaveDBSettings()
//процедура сохранения конфигурации. производится проверка данных в полях.
{
	db_prop.host  = db_host_edit->Text;
	db_prop.name  = db_name_edit->Text;
	db_prop.user  = db_user_edit->Text;
	db_prop.pass  = db_pass_edit->Text;
	tables_prefix = db_prf_edit->Text;

	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);
		inifile->WriteString("DB Connection Properties", "HostName", db_prop.host);
		inifile->WriteString("DB Connection Properties", "Database", db_prop.name);
		inifile->WriteString("DB Connection Properties", "User_Name", db_prop.user);
		inifile->WriteString("DB Connection Properties", "Password", db_prop.pass);
		inifile->WriteString("DB Connection Properties", "Prefix", tables_prefix);
	}
	__finally
	{
		if (inifile)
			delete inifile;
	}
}
//---------------------------------------------------------------------------

void TSettingsForm::SaveAdditionalSettings() {
	file_log          =  file_log_check->Checked;
	sync_autostart    =  auto_start_check->Checked;
	show_work_time    =  show_work_time_check->Checked;
	auto_update       =  auto_update_check->Checked;
	check_version     =  check_version_check->Checked;
	log_notifications =  log_notifications_check->Checked;

	TIniFile *inifile = NULL;
	try {
		inifile = new TIniFile(config_ini_path);
		inifile->WriteBool("Additional Properties", "FileLogging", file_log);
		inifile->WriteBool("Additional Properties", "AutoStart", sync_autostart);
		inifile->WriteBool("Additional Properties", "ShowWorkTime", show_work_time);
		inifile->WriteBool("Additional Properties", "AutoUpdate", auto_update);
		inifile->WriteBool("Additional Properties", "AutoVersionCheck", check_version);
		inifile->WriteBool("Additional Properties", "LogNotifications", log_notifications);
	} __finally {
		if (inifile) {
			delete inifile;
        }
	}
}
//---------------------------------------------------------------------------

void TSettingsForm::SaveImagesSettings() {
	TIniFile *inifile = NULL;

	try {
		data_storage = image_folder_edit->Text;
		inifile = new TIniFile(config_ini_path);
		inifile->WriteString("Images", "LocalFolder", data_storage);
	} catch(Exception *E) {
    	InfoAction(Format(__("ImagesSettingsSaveError"), ARRAYOFCONST((E->Message))), true, 0, 1);
	}

	if (inifile) {
		delete inifile;
	}
}
//---------------------------------------------------------------------------

void TSettingsForm::save_wht_s() {
	wht_nick_list->Items->SaveToFile(plugin_data_path + "white_ips.txt");
	white_ips->Clear();
	white_ips->AddStrings(wht_nick_list->Items);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
	//TranslateFormElements(this);

	// Ставим первой станичку настроек БД + выделяем этот пункт в списке настроек + выставляем индекс выделения
	db_s_page->BringToFront();
	settings_list->ItemIndex = 0;
	s_index = 0;

	//подгрузка в поля значений настроек соединения с БД
	db_host_edit->Text = db_prop.host;
	db_name_edit->Text = db_prop.name;
	db_user_edit->Text = db_prop.user;
	db_pass_edit->Text = db_prop.pass;
	db_prf_edit->Text  = tables_prefix;

	//подгрузка в поля значений настроек изображений
	image_folder_edit->Text = data_storage;
	images_count_lbl->Caption = CountFilesInFolder(data_storage, "jpg");

	//подгрузка в поля значений дополнительных настроек
	file_log_check->Checked           =  file_log;
	auto_start_check->Checked         =  sync_autostart;
	show_work_time_check->Checked     =  show_work_time;
	log_notifications_check->Checked  =  log_notifications;
	check_version_check->Checked      =  check_version;
	if (!check_version) {
		auto_update_check->Enabled = false;
		auto_update_check->Checked = false;
	} else {
		auto_update_check->Checked = auto_update;
    }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::test_conn_butClick(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::load_first_config_butClick(TObject *Sender)
{
	main_form->load_first_config_run_form();
}
//---------------------------------------------------------------------------


void __fastcall TSettingsForm::add_chnl_editEnter(TObject *Sender)
{

}
//---------------------------------------------------------------------------


void __fastcall TSettingsForm::chnl_listClick(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ok_btnClick(TObject *Sender)
{
	save_btn->Click();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::save_btnClick(TObject *Sender)
{
	switch (s_index) {
		case 0:
			SaveDBSettings();
			break;
		case 1:
			SaveImagesSettings();
			break;
		case 2:
			SaveAdditionalSettings();
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::cancel_btnClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::add_del_chnl_btnClick(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::add_del_nick_btnClick(TObject *Sender)
{
	add_del_string_list(add_del_nick_btn, add_nick_edit, wht_nick_list);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::add_nick_editEnter(TObject *Sender)
{
	add_del_nick_btn->Caption = "+";
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::wht_nick_listClick(TObject *Sender)
{
	if (wht_nick_list->ItemIndex != -1)
		add_del_nick_btn->Caption = "-";
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::add_nick_editKeyPress(TObject *Sender, wchar_t &Key)

{
	if (Key == VK_RETURN) {
		add_del_string_list(add_del_nick_btn, add_nick_edit, wht_nick_list);
	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::add_chnl_editKeyPress(TObject *Sender, wchar_t &Key)

{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::chnl_listKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::wht_nick_listKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (wht_nick_list->ItemIndex != -1 && Key == VK_DELETE) {
		add_del_string_list(add_del_nick_btn, add_nick_edit, wht_nick_list);
	}
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::settings_listClick(TObject *Sender)
{
	// Current Index.
	int c_index = settings_list->ItemIndex;
	if (c_index != -1 && s_index != c_index) {
		s_index = c_index;
		if (c_index == 0) {
			db_s_page->BringToFront();
		}
		else if (c_index == 1) {
			images_s_page->BringToFront();
		}
		else if (c_index == 2) {
			add_s_page->BringToFront();
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TSettingsForm::cr_db_struct_btnClick(TObject *Sender)
{

}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::choose_folder_buttonClick(TObject *Sender)
{
	String dir = image_folder_edit->Text;
	SelectDirectory(__("ImageFolderChooseLabel"), "C:", dir, TSelectDirExtOpts() << sdNewUI << sdNewFolder);
	image_folder_edit->Text = dir;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::clear_images_buttonClick(TObject *Sender)
{
	char src[MAX_PATH];
	memset(src, 0, sizeof(src));
	SHFILEOPSTRUCT fos;
	memset(&fos, 0, sizeof(fos));
	fos.hwnd = Handle;
	fos.wFunc = FO_DELETE;
	strcpy(src, AnsiString(data_storage + "\\*.jpg").c_str());
	fos.pFrom = src;
	fos.fFlags = FOF_ALLOWUNDO | FOF_FILESONLY;
	SHFileOperation(&fos);
	images_count_lbl->Caption = CountFilesInFolder(data_storage, "jpg");
}
//---------------------------------------------------------------------------

int TSettingsForm::CountFilesInFolder(String path, String extension) {
	TSearchRec Rec;
	int count = 0;
	extension = "." + extension;

	if (FindFirst(path + "\\*" + extension, faNormal, Rec) == 0) {
		do {
			if (ExtractFileExt(Rec.Name) == extension) {
				count++;
			}
		} while (FindNext(Rec) == 0);
		FindClose(Rec);
	}

	return count;
}
void __fastcall TSettingsForm::check_version_checkClick(TObject *Sender)
{
	if (check_version_check->Checked) {
		auto_update_check->Enabled = true;
	} else {
		auto_update_check->Checked = false;
		auto_update_check->Enabled = false;
    }
}
//---------------------------------------------------------------------------

