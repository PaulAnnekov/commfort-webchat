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
//---------------------------------------------------------------------------

#define MF_DO_ACTION (WM_APP + 401)

class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *work_time_timer;
	TTrayIcon *tray_icon;
	TPopupMenu *tray_popup;
	TMenuItem *open_window_item;
	TMenuItem *plugin_state_item;
	TMenuItem *plugin_terminate_item;
	TMenuItem *settings_item;
	TMenuItem *about_item;
	TGroupBox *main_box;
	TButton *stop_plugin_but;
	TButton *about_but;
	TButton *settings_but;
	TGroupBox *GroupBox1;
	TLabel *errors_label;
	TLabel *errors_state;
	TLabel *bd_connection_state;
	TLabel *bd_connection_label;
	TLabel *online_users_label;
	TLabel *online_users_state;
	TLabel *plugin_state_label;
	TLabel *plugin_state;
	TLabel *time_plus_lbl;
	TLabel *time_plus_state;
	TLabel *web_users_label;
	TLabel *web_users_state;
	TGroupBox *GroupBox2;
	TImageList *icons_list;
	TButton *plug_sync_but;
	TButton *online_users_but;
	TButton *open_log_but;
	TButton *show_conn_but;
	TLabel *warnings_state;
	TButton *Button2;
	TSpeedButton *SpeedButton1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tray_iconClick(TObject *Sender);
	void __fastcall load_first_config_butClick(TObject *Sender);
	void __fastcall online_users_butClick(TObject *Sender);
	void __fastcall open_log_butClick(TObject *Sender);
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

private:	// User declarations
	int DoAction(TMessage &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(MF_DO_ACTION, TMessage, DoAction)
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
//extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
