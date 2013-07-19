//---------------------------------------------------------------------------


#pragma hdrstop

#include "check_conn.h"
#include "connections.h"
#include "connections_form.h"
#include "global.h"
#include "notif.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TConnCheckTimer *conn_check_timer = NULL;

__fastcall TConnCheckTimer::TConnCheckTimer() {
	ConnCheckTimer = new TTimer(Application);
	ConnCheckTimer->Interval = 3000;
	ConnCheckTimer->OnTimer = TConnCheckTimer::ConnCheckTimerOnTimer;
	ConnCheckTimer->Enabled = true;
}

__fastcall TConnCheckTimer::~TConnCheckTimer() {
	delete ConnCheckTimer;
}

void __fastcall TConnCheckTimer::ConnCheckTimerOnTimer(TObject *Sender) {
	int rows = logical_connections.size();
	int i;
	if (rows > 0) {
		if (conn_list_form) {
			LogicalConnections::iterator res = NULL;
			conn_list_form->connections_grid->RowCount = rows + 1;
			i = 1;
			for (res = logical_connections.begin(); res != logical_connections.end(); res++) {
				conn_list_form->connections_grid->Cells[0][i] = res->conn_id;
				if (IsUserIteratorSet(res->virtual_user)) {
					conn_list_form->connections_grid->Cells[1][i] = res->virtual_user->name;
				}
				//conn_list_form->connections_grid->Cells[1][i] = res->nick;
				conn_list_form->connections_grid->Cells[2][i] = res->ip;
				conn_list_form->connections_grid->Cells[3][i] = res->app_name;
				conn_list_form->connections_grid->Cells[4][i] = get_time_offset(res->last_act_t);
				conn_list_form->connections_grid->Cells[5][i] = get_time_offset(res->time);
				i++;
			}
		}
	} else {
		if (conn_list_form) {
			conn_list_form->connections_grid->RowCount = 2;
			for (i = 0; i <= 5; i++) {
				conn_list_form->connections_grid->Cells[i][1] = "";
			}
		}
	}
	//if (main_form) {
	//	main_form->web_users_state->Caption = (rows == 0) ? t_connections_off : String(rows);
	//}
}
