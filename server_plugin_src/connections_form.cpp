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

#include "connections_form.h"
#include "connections.h"
#include "global.h"
#include "notif.h"
#include "l18n.h"
#include <Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "KControls"
#pragma link "KGrids"
#pragma link "KControls"
#pragma link "KGrids"
#pragma resource "*.dfm"
TConnectionsList *conn_list_form;
//int fixed_cols_width = 0;

//---------------------------------------------------------------------------
__fastcall TConnectionsList::TConnectionsList(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TConnectionsList::set_col_width() {

}
//---------------------------------------------------------------------------

void TConnectionsList::DeleteRow(int index) {

}
//---------------------------------------------------------------------------

void TConnectionsList::AddConnection(String conn_id, String ip, String app_name, int time, int last_action_time = 0, int auth_state = 0, String virtual_user = "") {
	int i = connections_grid->RowCount;
	connections_grid->InsertRow(i);

	if (connections_grid->Visible == false) {
		SetOneConnection();
	}

	String str_time = get_time_offset(time);

	connections_grid->Cells[0][i] = conn_id;
	if (!virtual_user.IsEmpty()) {
		connections_grid->Cells[1][i] = virtual_user;
	}
	connections_grid->Cells[2][i] = ip;
	connections_grid->Cells[3][i] = app_name;
	connections_grid->Cells[4][i] = str_time;

	if (last_action_time) {
		connections_grid->Cells[5][i] = get_time_offset(last_action_time);
	} else {
		connections_grid->Cells[5][i] = str_time;
    }

	SetRowInfo(i, time);
	UpdateRowInfo(i, auth_state, time, last_action_time);

	CheckColsWidth();
}
//---------------------------------------------------------------------------

int TConnectionsList::DoAction(TMessage &Message) {
	try {
		LogicalConnection *new_connection = (LogicalConnection *) Message.LParam;

        int row;
		switch (Message.WParam) {
			// AddConnection
			case 0:
				AddConnection(new_connection->conn_id, new_connection->ip, new_connection->app_name, new_connection->time);
			break;
			// DeleteConnection
			case 1:
				DeleteConnectionRow(new_connection->conn_id);
			break;
			// SetConnectionUser
			case 2:
				SetConnectionUser(new_connection->conn_id, GetOnlineUserName(new_connection->user));
			break;
			// UnSetConnectionUser
			case 3:
				UnSetConnectionUser(new_connection->conn_id);
			break;
			// UpdateUserState
			case 4:
				row = GetRowByConnID(new_connection->conn_id);
				if (row) {
					UpdateRowInfo(row, new_connection->auth_status, 0, 0);
				}
			break;
			// UpdateLastActionTime
			case 5:
				row = GetRowByConnID(new_connection->conn_id);
				if (row) {
					UpdateRowInfo(row, 0, 0, new_connection->last_act_t);
				}
			break;
		}
	} catch(Exception *E) {
		InfoAction("Error during Connections list form synchronization: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during Connections list form synchronization", false, 3, 2);
	}

	return 1;
}
//---------------------------------------------------------------------------

void TConnectionsList::DeleteRowInfo(int row) {
	try {
		if (connections_grid->Rows[row]->Tag) {
			delete connections_grid->Rows[row]->Tag;
		}
	} catch(Exception *E) {
		InfoAction("Error during unset Connection info: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during unset Connection info", false, 3, 2);
	}
}
//---------------------------------------------------------------------------

void TConnectionsList::SetRowInfo(int row, int time) {
	try {
		TRowInfo *row_info = new TRowInfo();
		row_info->time = time;
		row_info->last_action_time = time;
		connections_grid->Rows[row]->Tag = row_info;
	} catch(Exception *E) {
		InfoAction("Error during set Connection info: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during set Connection info", false, 3, 2);
	}
}
//---------------------------------------------------------------------------

TColor TConnectionsList::GetUserColor(int row) {
	TColor user_color = (TColor) RGB(0, 0, 0);
	try {
		if (connections_grid->Rows[row]->Tag) {
			user_color = ((TRowInfo *) connections_grid->Rows[row]->Tag)->user_color;
		}
	} catch(Exception *E) {
		InfoAction("Error during get Connection user color: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during get Connection user color", false, 3, 2);
	}

	return user_color;
}
//---------------------------------------------------------------------------

int TConnectionsList::GetUserState(int row) {
	int state = 8;
	try {
		if (connections_grid->Rows[row]->Tag) {
			state = ((TRowInfo *) connections_grid->Rows[row]->Tag)->user_state;
		}
	} catch(Exception *E) {
		InfoAction("Error during get Connection user state: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during get Connection user state", false, 3, 2);
	}

	return state;
}
//---------------------------------------------------------------------------

String TConnectionsList::GetUserLastActionTime(int row) {
	String time = "00:00:00";
	try {
		if (connections_grid->Rows[row]->Tag) {
			time = get_time_offset(((TRowInfo *) connections_grid->Rows[row]->Tag)->last_action_time);
		}
	} catch(Exception *E) {
		InfoAction("Error during get Connection user state: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during get Connection user state", false, 3, 2);
	}

	return time;
}
//---------------------------------------------------------------------------

String TConnectionsList::GetConnectionTime(int row) {
	String time = "00:00:00";
	try {
		if (connections_grid->Rows[row]->Tag) {
			time = get_time_offset(((TRowInfo *) connections_grid->Rows[row]->Tag)->time);
		}
	} catch(Exception *E) {
		InfoAction("Error during get Connection user state: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during get Connection user state", false, 3, 2);
	}

	return time;
}
//---------------------------------------------------------------------------

void TConnectionsList::UpdateRowInfo(int row, int state, int time = 0, int last_action_time = 0) {
	try {
		if (connections_grid->Rows[row]->Tag) {
			TRowInfo *row_info = (TRowInfo *) connections_grid->Rows[row]->Tag;
			if (state) {
            	row_info->user_state = state;
				if (state == 9) {
					row_info->user_color = (TColor) RGB(0, 153, 26);
				} else if(state == 16 || state == 8) {
					row_info->user_color = (TColor) RGB(171, 162, 16);
				} else {
					row_info->user_color = (TColor) RGB(255, 0, 0);
				}
			}
			if (time) {
				row_info->time = time;
			}
			if (last_action_time) {
				row_info->last_action_time = last_action_time;
			}
			connections_grid->Rows[row]->Tag = row_info;
			connections_grid->Repaint();
		}
	} catch(Exception *E) {
		InfoAction("Error during update Connection user state: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during update Connection user state", false, 3, 2);
	}
}
//---------------------------------------------------------------------------

int TConnectionsList::GetRowByConnID(String conn_id) {
	int rows_count = connections_grid->RowCount;
	int row = 0;

	for (int i = 1; i < rows_count; i++) {
		if (connections_grid->Cells[0][i] == conn_id) {
			row = i;
			break;
		}
	}

	return row;
}
//---------------------------------------------------------------------------

void TConnectionsList::DeleteConnectionRow(String conn_id) {
	int row = GetRowByConnID(conn_id);

	if (row) {
		DeleteRowInfo(row);
		connections_grid->DeleteRow(row);
		CheckColsWidth();

		if (connections_grid->RowCount == 1) {
			SetNoConnections();
		}
	}
}
//---------------------------------------------------------------------------

void TConnectionsList::SetConnectionUser(String conn_id, String user) {
	int row = GetRowByConnID(conn_id);

	if (row) {
		connections_grid->Cells[1][row] = user;
		CheckColsWidth();
	}
}
//---------------------------------------------------------------------------

void TConnectionsList::UnSetConnectionUser(String conn_id) {
	int row = GetRowByConnID(conn_id);

	if (row) {
		connections_grid->Cells[1][row] = "";
		CheckColsWidth();
    }
}
//---------------------------------------------------------------------------

void TConnectionsList::UpdateConnections() {
	if (sync_enabled) {
		try {
			LogicalConnections connections = GetConnections();

			int rows = connections.size();
			if (rows > 0) {
				LogicalConnections::iterator res;
				int auth_status;
				String virtual_user;
				for (res = connections.begin(); res != connections.end(); res++) {
					auth_status = 0;
					virtual_user = "";
					if (IsOnlineUserSet(res->user)) {
						virtual_user = GetOnlineUserName(res->user);
						auth_status = res->auth_status;
					}
					AddConnection(res->conn_id, res->ip, res->app_name, res->time, res->last_act_t, auth_status, virtual_user);
				}
			}
		} catch (Exception *E) {
			InfoAction(Format(__("ConnectionsUpdateWarning"), ARRAYOFCONST((E->Message))), false, 3, 1);
		} catch (...) {
			InfoAction(__("ConnectionsUpdateError"), false, 3, 2);
		}
	}
}
//---------------------------------------------------------------------------

void TConnectionsList::SetNoConnections() {
	connections_grid->Visible = false;
	connections_grid->RowCount = 1;
}
//---------------------------------------------------------------------------

void TConnectionsList::SetOneConnection() {
    connections_grid->FixedRows = 1;
	connections_grid->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::FormShow(TObject *Sender)
{
	UpdateConnections();
	CheckColsWidth();
}
//---------------------------------------------------------------------------


void __fastcall TConnectionsList::FormResize(TObject *Sender)
{
	//set_col_width();
}
//---------------------------------------------------------------------------

bool TConnectionsList::select_row(int x, int y) {
	int cell_x, cell_y;

	bool is_connection_row = false;
	is_connection_row = connections_grid->MouseToCell(x, y, cell_x, cell_y);
	if (is_connection_row/*cell_y != 0*/) {
		connections_grid->SelectRow(cell_y);
	}

	return is_connection_row;
}

void __fastcall TConnectionsList::connections_grid1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y) {
	if (Button == mbRight) {
		if (select_row(X, Y)) {
			TPoint point = ClientToScreen(Point(X, Y));
			connections_grid->PopupMenu->Popup(point.x, point.y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::upd_timerTimer(TObject *Sender) {
	int count = connections_grid->RowCount;

	for (int i = 1; i < count; i++) {
		connections_grid->Cells[4][i] = GetUserLastActionTime(i);
		connections_grid->Cells[5][i] = GetConnectionTime(i);
	}
}
//---------------------------------------------------------------------------


void __fastcall TConnectionsList::FormClose(TObject *Sender, TCloseAction &Action) {
	Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::FormDestroy(TObject *Sender) {
	conn_list_form = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::DisconnectClientItemClick(TObject *Sender)
{
	String conn_id = connections_grid->Cells[0][connections_grid->Row];

	int id = ShowMsg(Handle, __("DisconnectConfirm") + " " + conn_id + "?", 3);

	if (id == IDYES) {
		DeleteConnection(connections_grid->Cells[0][connections_grid->Row]);
	}
}
//---------------------------------------------------------------------------


void __fastcall TConnectionsList::CopyIDItemClick(TObject *Sender)
{
	Clipboard()->SetTextBuf(connections_grid->Cells[0][connections_grid->Row].c_str());
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::CopyNameItemClick(TObject *Sender)
{
	Clipboard()->SetTextBuf(connections_grid->Cells[1][connections_grid->Row].c_str());
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::CopyIPItemClick(TObject *Sender)
{
	Clipboard()->SetTextBuf(connections_grid->Cells[2][connections_grid->Row].c_str());
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::CopyProgramItemClick(TObject *Sender)
{
	Clipboard()->SetTextBuf(connections_grid->Cells[3][connections_grid->Row].c_str());
}
//---------------------------------------------------------------------------

void __fastcall TConnectionsList::FormCreate(TObject *Sender)
{
	connections_grid->Cells[0][0] = __("ConnectionID");
	connections_grid->Cells[1][0] = __("Nick");
	connections_grid->Cells[2][0] = __("IP");
	connections_grid->Cells[3][0] = __("AppName");
	connections_grid->Cells[4][0] = __("LastAction");
	connections_grid->Cells[5][0] = __("ConnectionTime");

	this->Caption = __("ActiveConnections");
	no_connections_label->Caption = __("NoActiveConnections");
}
//---------------------------------------------------------------------------





void __fastcall TConnectionsList::connections_gridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (Button == mbRight) {
		if (select_row(X, Y)) {
			TPoint point = ClientToScreen(Point(X, Y));
			connections_grid->PopupMenu->Popup(point.x, point.y);
		}
	}
}
//---------------------------------------------------------------------------




void __fastcall TConnectionsList::connections_gridResize(TObject *Sender)
{
	CheckColsWidth();
}
//---------------------------------------------------------------------------

void TConnectionsList::CheckColsWidth() {
	connections_grid->AutoSizeGrid(mpColWidth, true);
}

void __fastcall TConnectionsList::connections_gridDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &R, TKGridDrawState State)
{
	connections_grid->Cell[ACol][ARow]->ApplyDrawProperties();
	connections_grid->CellPainter->DefaultDraw();
	if (ACol == 1 && ARow > 0 && !connections_grid->Cells[1][ARow].IsEmpty()) {
		connections_grid->CellPainter->Canvas->Font->Color = GetUserColor(ARow);
		connections_grid->CellPainter->DrawCellText(R);
	}
}
//---------------------------------------------------------------------------


void __fastcall TConnectionsList::connections_gridMouseCellHint(TObject *Sender, int ACol,
		  int ARow, bool AShow)
{
	if (AShow && ACol == 1 && ARow > 0 && !connections_grid->Cells[1][ARow].IsEmpty()) {
		connections_grid->Hint = __("AuthState" + String(GetUserState(ARow)));
		Application->ActivateHint(Mouse->CursorPos);
	} else {
		connections_grid->Hint = "";
		Application->CancelHint();
    }
}
//---------------------------------------------------------------------------





