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

#ifndef webchatH
#define webchatH

#include "about_form.h"
#include "settings_form.h"

typedef VOID __stdcall (*typeCommFortProcess)(DWORD dwPluginID, DWORD dID, BYTE * bOutBuffer, DWORD dwOutBufferSize);
typedef DWORD __stdcall (*typeCommFortGetData)(DWORD dwPluginID, DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize);

extern typeCommFortProcess CommFortProcess;
extern typeCommFortGetData CommFortGetData;

extern "C" __declspec(dllexport) VOID PluginShowOptions();
extern "C" __declspec(dllexport) VOID PluginShowAbout();

extern DWORD dwPluginID;

//---------------------------------------------------------------------------
#endif
