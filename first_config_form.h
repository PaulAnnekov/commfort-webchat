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

#ifndef first_config_formH
#define first_config_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>
#include <Mask.hpp>
#include <AppEvnts.hpp>
//---------------------------------------------------------------------------
class TFastConfigForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *pages;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet1;
	TEdit *db_host_edit;
	TEdit *db_user_edit;
	TLabel *db_host_lbl;
	TLabel *db_name_lbl;
	TLabel *db_user_lbl;
	TLabel *db_pass_lbl;
	TGroupBox *db_add_group;
	TLabel *db_prf_lbl;
	TEdit *db_prf_edit;
	TButton *test_and_go_but;
	TLabel *bot_nick_lbl;
	TLabel *bot_pass_lbl;
	TLabel *bot_ip_lbl;
	TEdit *bot_nick_edit;
	TPanel *Panel1;
	TImage *hint_img;
	TLabel *hint_label;
	TImage *fast_conf_img;
	TLabel *greating_lbl;
	TLabel *main_tip_lbl;
	TButton *end_but;
	TButton *begin_but;
	TLabel *bot_sex_lbl;
	TComboBox *bot_sex_choose;
	TEdit *bot_ip_edit;
	TMaskEdit *bot_pass_edit;
	TMaskEdit *db_pass_edit;
	TEdit *db_name_edit;
	void __fastcall db_host_editMouseEnter(TObject *Sender);
	void __fastcall edit12MouseEnter(TObject *Sender);
	void __fastcall charset_chooseMouseEnter(TObject *Sender);
	void __fastcall db_user_editMouseEnter(TObject *Sender);
	void __fastcall pass_editMouseEnter(TObject *Sender);
	void __fastcall db_prf_editMouseEnter(TObject *Sender);
	void __fastcall keep_conn_chooseMouseEnter(TObject *Sender);
	void __fastcall test_and_go_butMouseEnter(TObject *Sender);
	void __fastcall bot_nick_editMouseEnter(TObject *Sender);
	void __fastcall bot_pass_editMouseEnter(TObject *Sender);
	void __fastcall bot_sex_chooseMouseEnter(TObject *Sender);
	void __fastcall TabSheet1MouseEnter(TObject *Sender);
	void __fastcall TabSheet3MouseEnter(TObject *Sender);
	void __fastcall TabSheet2MouseEnter(TObject *Sender);
	void __fastcall begin_butMouseEnter(TObject *Sender);
	void __fastcall begin_butClick(TObject *Sender);
	void __fastcall keep_conn_checkMouseEnter(TObject *Sender);
	void __fastcall test_and_go_butClick(TObject *Sender);
	void __fastcall end_butClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bot_ip_editMouseEnter(TObject *Sender);
	void __fastcall db_name_editMouseEnter(TObject *Sender);



private:	// User declarations
public:		// User declarations
	void load_hint(int element_id);
	__fastcall TFastConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFastConfigForm *FastConfigForm;
//---------------------------------------------------------------------------
#endif
