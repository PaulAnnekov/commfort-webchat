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

#ifndef settings_formH
#define settings_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
	TButton *load_first_config_but;
	TPanel *pages;
	TPanel *add_s_page;
	TLabel *add_main_lbl;
	TCheckBox *auto_start_check;
	TCheckBox *file_log_check;
	TPanel *images_s_page;
	TLabel *bot_main_lbl;
	TPanel *db_s_page;
	TLabel *db_prf_lbl;
	TLabel *db_pass_lbl;
	TLabel *db_user_lbl;
	TLabel *db_name_lbl;
	TLabel *db_host_lbl;
	TLabel *db_main_lbl;
	TButton *test_conn_but;
	TEdit *db_prf_edit;
	TMaskEdit *db_pass_edit;
	TEdit *db_user_edit;
	TEdit *db_name_edit;
	TEdit *db_host_edit;
	TPanel *wht_s_page;
	TLabel *wht_tip_lbl;
	TLabel *wht_main_lbl;
	TPanel *Panel1;
	TButton *ok_btn;
	TButton *cancel_btn;
	TButton *save_btn;
	TListBox *wht_nick_list;
	TEdit *add_nick_edit;
	TButton *add_del_nick_btn;
	TListBox *settings_list;
	TButton *cr_db_struct_btn;
	TCheckBox *show_work_time_check;
	TButton *choose_folder_button;
	TLabel *images_folder_label;
	TEdit *image_folder_edit;
	TButton *clear_images_button;
	TLabel *images_count_lbl;
	TLabel *total_images_label;
	TCheckBox *auto_update_check;
	TCheckBox *check_version_check;
	TCheckBox *log_notifications_check;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall test_conn_butClick(TObject *Sender);
	void __fastcall load_first_config_butClick(TObject *Sender);
	void __fastcall add_chnl_editEnter(TObject *Sender);
	void __fastcall chnl_listClick(TObject *Sender);
	void __fastcall ok_btnClick(TObject *Sender);
	void __fastcall save_btnClick(TObject *Sender);
	void __fastcall cancel_btnClick(TObject *Sender);
	void __fastcall add_del_chnl_btnClick(TObject *Sender);
	void __fastcall add_del_nick_btnClick(TObject *Sender);
	void __fastcall add_nick_editEnter(TObject *Sender);
	void __fastcall wht_nick_listClick(TObject *Sender);
	void __fastcall add_nick_editKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall add_chnl_editKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall chnl_listKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall wht_nick_listKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall settings_listClick(TObject *Sender);
	void __fastcall cr_db_struct_btnClick(TObject *Sender);
	void __fastcall choose_folder_buttonClick(TObject *Sender);
	void __fastcall clear_images_buttonClick(TObject *Sender);
	void __fastcall check_version_checkClick(TObject *Sender);






private:	// User declarations
	int s_index;
public:		// User declarations
	void add_del_string_list(TButton *btn, TEdit *edit, TListBox *list);
    int CountFilesInFolder(String path, String extension);
	void SaveDBSettings();
	void save_wht_s();
	void SaveImagesSettings();
	void SaveAdditionalSettings();
	__fastcall TSettingsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
