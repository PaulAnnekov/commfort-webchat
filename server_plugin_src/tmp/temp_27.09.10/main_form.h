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
#include <DB.hpp>
#include <DBXMySql.hpp>
#include <ExtCtrls.hpp>
#include <FMTBcd.hpp>
#include <SqlExpr.hpp>
#include <WideStrings.hpp>
#include <Menus.hpp>
#include <winsock2.h>
#include "DBAccess.hpp"
#include "MyAccess.hpp"
#include <Sockets.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"

//---------------------------------------------------------------------------
class TConnectThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TConnectThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *full_refresh_timer;
	TSQLConnection *bd_sql_connection1;
	TTrayIcon *tray_icon;
	TTimer *send_from_web_timer;
	TSQLDataSet *result_set1;
	TRichEdit *states_list_rich;
	TPanel *info_panel;
	TLabel *errors_label;
	TLabel *warnings_label;
	TLabel *online_users_label;
	TLabel *bd_connection_label;
	TLabel *plugin_state_label;
	TLabel *web_public_state_label;
	TLabel *next_web_public_label;
	TLabel *errors_state;
	TLabel *warnings_state;
	TLabel *online_users_state;
	TLabel *bd_connection_state;
	TLabel *plugin_state;
	TLabel *web_public_state;
	TLabel *next_web_public;
	TButton *open_log_but;
	TButton *online_users_but;
	TButton *bd_connection_but;
	TButton *plugin_state_but;
	TButton *web_public_state_but;
	TButton *clear_logs_but;
	TGroupBox *main_box;
	TButton *stop_plugin_but;
	TButton *about_but;
	TButton *settings_but;
	TPopupMenu *tray_popup;
	TMenuItem *open_window_item;
	TMenuItem *plugin_state_item;
	TMenuItem *plugin_terminate_item;
	TMenuItem *settings_item;
	TMenuItem *about_item;
	TMyConnection *bd_sql_connection;
	TIdTCPClient *IdTCPClient1;
	void __fastcall bd_sql_connection1AfterConnect(TObject *Sender);
	void __fastcall bd_conn_change_butClick(TObject *Sender);
	void __fastcall bd_sql_connection1AfterDisconnect(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tray_iconClick(TObject *Sender);
	void __fastcall clear_logs_butClick(TObject *Sender);
	void __fastcall send_from_web_timerTimer(TObject *Sender);
	void __fastcall load_first_config_butClick(TObject *Sender);
	void __fastcall online_users_butClick(TObject *Sender);
	void __fastcall bd_connection_butClick(TObject *Sender);
	void __fastcall open_log_butClick(TObject *Sender);
	void __fastcall web_public_state_butClick(TObject *Sender);
	void __fastcall plugin_state_butClick(TObject *Sender);
	void __fastcall stop_plugin_butClick(TObject *Sender);
	void __fastcall about_butClick(TObject *Sender);
	void __fastcall settings_butClick(TObject *Sender);
	void __fastcall Button123Click(TObject *Sender);
	void __fastcall open_window_itemClick(TObject *Sender);
	void __fastcall settings_itemClick(TObject *Sender);
	void __fastcall about_itemClick(TObject *Sender);
	void __fastcall plugin_terminate_itemClick(TObject *Sender);
	void __fastcall plugin_state_itemClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bd_sql_connectionConnectionLost(TObject *Sender, TComponent *Component,
          TConnLostCause ConnLostCause, TRetryMode &RetryMode);
	void __fastcall IdTCPClient1Connected(TObject *Sender);


private:	// User declarations
public:

	void debug(String text, BYTE *var, DWORD Length);
	void get_users_list(String channel);
	void virt_user_conn (String name, String ip, String pass, bool is_female);
	void virt_user_disconn (String name);
	void virt_user_init (bool is_connected, int error_code);
	void set_user_state(String state);
	void add_new_action_list(String text, String query, bool show_message, int type);
	void save_config();
	void __fastcall TMainForm::exception_handler(TObject *Sender, Exception *E);

	void apply_config(byte action);
	void run_refresh_timer(byte action, int interval, TTime time);
	void progress_bar_change(Byte position, Byte state, String text);
	void log_file_init(bool isfirstrun);
	void load_first_config_run_form();
	bool establish_connection(bool test);
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
