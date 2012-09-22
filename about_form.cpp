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

#include "about_form.h"
#include "global.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

__fastcall TAboutForm::TAboutForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::FormCreate(TObject *Sender)
{
	about_version->Caption = about_version->Caption + " " + plugin_version;
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::sait_link_lablelClick(TObject *Sender)
{
	ShellExecute(0, "open", "http://webcf.ru", "", "", SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::forum_link_labelClick(TObject *Sender)
{
	ShellExecute(0, "open", "http://commfort.com/ru/forum/viewtopic.php?f=28&t=6607&start=0", "", "", SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

