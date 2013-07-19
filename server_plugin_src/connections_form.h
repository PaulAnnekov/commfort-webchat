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

#ifndef connections_formH
#define connections_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include "KControls.hpp"
#include "KGrids.hpp"
//---------------------------------------------------------------------------

#define CL_DO_ACTION (WM_APP + 400)

class TRowInfo : public TObject {
	public:
		int user_state;
		TColor user_color;
		unsigned time, last_action_time;
};

class TConnectionsList : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *conn_actions_pop;
	TMenuItem *DisconnectClientItem;
	TTimer *upd_timer;
	TMenuItem *CopyIDItem;
	TMenuItem *CopyNameItem;
	TMenuItem *CopyIPItem;
	TMenuItem *CopyProgramItem;
	TLabel *no_connections_label;
	TKGrid *connections_grid;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall connections_grid1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall upd_timerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall DisconnectClientItemClick(TObject *Sender);
	void __fastcall CopyIDItemClick(TObject *Sender);
	void __fastcall CopyNameItemClick(TObject *Sender);
	void __fastcall CopyIPItemClick(TObject *Sender);
	void __fastcall CopyProgramItemClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall connections_gridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall connections_gridResize(TObject *Sender);
	void __fastcall connections_gridDrawCell(TObject *Sender, int ACol, int ARow, TRect &R,
          TKGridDrawState State);
	void __fastcall connections_gridMouseCellHint(TObject *Sender, int ACol, int ARow,
          bool AShow);

private:	// User declarations
	void set_col_width();
	bool select_row(int x, int y);
	void UpdateConnections();
	void DeleteRow(int index);
	void SetNoConnections();
	void SetOneConnection();
	void CheckColsWidth();
	void DeleteRowInfo(int row);
	void SetRowInfo(int row, int time);
	void UpdateRowInfo(int row, int state, int time, int last_action_time);
	int GetRowByConnID(String conn_id);
	TColor GetUserColor(int row);
	int GetUserState(int row);
	String GetUserLastActionTime(int row);
	String GetConnectionTime(int row);
	void DeleteConnectionRow(String conn_id);
	int DoAction(TMessage &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(CL_DO_ACTION, TMessage, DoAction)
	END_MESSAGE_MAP(TControl)
public:		// User declarations
	__fastcall TConnectionsList(TComponent* Owner);
	void AddConnection(String conn_id, String ip, String app_name, int time, int last_action_time, int auth_state, String virtual_user);
	void SetConnectionUser(String conn_id, String user);
	void UnSetConnectionUser(String conn_id);
};
//---------------------------------------------------------------------------
extern PACKAGE TConnectionsList *conn_list_form;
//---------------------------------------------------------------------------
#endif
