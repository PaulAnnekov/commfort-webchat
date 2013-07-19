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

#ifndef main_formH
#define main_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <WideStrings.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <pngimage.hpp>
#include <Graphics.hpp>
#include <IdHTTP.hpp>
//---------------------------------------------------------------------------

#define MF_DO_ACTION (WM_APP + 401)
#define MF_CF_PROCESS (WM_APP + 402)

class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTimer *work_time_timer;
	TTrayIcon *tray_icon;
	TPopupMenu *tray_popup;
	TMenuItem *open_window_item;
	TMenuItem *plugin_state_item;
	TMenuItem *plugin_terminate_item;
	TMenuItem *settings_item;
	TMenuItem *about_item;
	TMemo *last_log_memo;
	TSpeedButton *open_log_but;
	TSpeedButton *show_conn_but;
	TLabel *time_plus_state;
	TLabel *time_plus_lbl;
	TLabel *web_users_state;
	TLabel *web_users_label;
	TLabel *errors_label;
	TLabel *errors_state;
	TLabel *version_label;
	TTimer *cron_timer;
	TSpeedButton *plug_sync_but;
	TSpeedButton *settings_button;
	TButton *Button1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tray_iconClick(TObject *Sender);
	void __fastcall load_first_config_butClick(TObject *Sender);
	void __fastcall online_users_butClick(TObject *Sender);
	void __fastcall plug_sync_butClick(TObject *Sender);
	void __fastcall stop_plugin_butClick(TObject *Sender);
	void __fastcall about_butClick(TObject *Sender);
	void __fastcall settings_butClick(TObject *Sender);
	void __fastcall open_window_itemClick(TObject *Sender);
	void __fastcall settings_itemClick(TObject *Sender);
	void __fastcall about_itemClick(TObject *Sender);
	void __fastcall plugin_terminate_itemClick(TObject *Sender);
	void __fastcall plugin_state_itemClick(TObject *Sender);
	void __fastcall work_time_timerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall show_conn_butClick(TObject *Sender);
	void __fastcall open_log_butClick(TObject *Sender);
	void __fastcall version_labelClick(TObject *Sender);
	void __fastcall cron_timerTimer(TObject *Sender);
	void __fastcall settings_buttonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
	int DoAction(TMessage &Message);
	int CFProcessRequest(TMessage &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(MF_DO_ACTION, TMessage, DoAction)
		MESSAGE_HANDLER(MF_CF_PROCESS, TMessage, CFProcessRequest)
	END_MESSAGE_MAP(TControl)
public:
	__fastcall TMainForm(TComponent* Owner);
	void save_config();
	void check_new_mess();
	void check_new_users();
	void check_old_users();
	void bot_connected();
	void bot_denied (int error_code);
	//void update_users_list();
	void __fastcall exception_handler(TObject *Sender, Exception *E);
	void apply_config(byte action);
	void run_refresh_timer(byte action, int interval, TTime time);
	void progress_bar_change(Byte position, Byte state, String text);
	void load_first_config_run_form();
	void set_window_height();
	void errors_warnings_update();
};
//---------------------------------------------------------------------------
#endif
