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

#ifndef update_formH
#define update_formH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include <ExtCtrls.hpp>

#define UF_CONTROL (WM_APP + 403)

//---------------------------------------------------------------------------
class TUpdateForm : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *download_progress;
	TLabel *downloading_state_label;
	TLabel *checking_state_label;
	TLabel *current_version_label;
	TPanel *update_actions_panel;
	TButton *update_plugin_button;
	TScrollBox *ScrollBox1;
	TLabel *new_version_label;
	TLabel *new_version_info_label;
	//void __fastcall IdHTTP1Work(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	//void __fastcall IdHTTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	//void __fastcall IdHTTP1WorkEnd(TObject *ASender, TWorkMode AWorkMode);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall update_plugin_buttonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);


private:	// User declarations
	int UFControl(TMessage &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(UF_CONTROL, TMessage, UFControl)
	END_MESSAGE_MAP(TControl)
public:		// User declarations
	__fastcall TUpdateForm(TComponent* Owner);
};

extern TUpdateForm *update_form;

void GetVersionInfo();
void GetNewVersion();

#endif
