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


#pragma hdrstop

#include "vector.h"
#include "notif.h"
#include <map.h>
#include <Forms.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

struct TimeMeasure {
	TimeMeasure(FILETIME _time, String _hint) : time(_time), hint(_hint) {};
	FILETIME time;
	String hint;
};

typedef vector<TimeMeasure> TimeMeasures;
TimeMeasures time_measures;

void binary_debug(void *variable, DWORD length) {
	int smth;
	String debug_path = ExtractFilePath(Application->ExeName) + "Plugins\\binary_debug.txt";

	if (FileExists(debug_path)) {
			smth = FileOpen(debug_path, fmOpenReadWrite);
			FileSeek(smth, 0, 2);
	} else smth = FileCreate(debug_path);

	FileWrite(smth, variable, length);
	FileClose(smth);
}

void plain_debug(String text) {
	int smth;
	String debug_path = ExtractFilePath(Application->ExeName) + "Plugins\\plain_debug.txt";
	text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), text)));
	if (FileExists(debug_path))
		{
			smth = FileOpen(debug_path, fmOpenReadWrite);
			FileSeek(smth, 0, 2);
		}
	else smth = FileCreate(debug_path);

	FileWrite(smth, AnsiString(text + "\r\n").c_str(), text.Length() + 2);
	FileClose(smth);
}

void debug(String text, BYTE * var, DWORD Length) {
	int smth;
	String debug_path = ExtractFilePath(Application->ExeName) + "Plugins\\debug.txt";
	text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), text)));
	if (FileExists(debug_path))
		{
			smth = FileOpen(debug_path, fmOpenReadWrite);
			FileSeek(smth, 0, 2);
		}
	else smth = FileCreate(debug_path);

	FileWrite(smth, AnsiString(text + "\r\n").c_str(), text.Length() + 2);
	FileWrite(smth, "\r\n****************\r\n", 20);
	FileWrite(smth, var, Length);
	FileWrite(smth, "\r\n****************\r\n", 20);
	FileClose(smth);
}

void MeasureTime(String hint) {
	FILETIME now;

	GetSystemTimeAsFileTime(&now);

	time_measures.push_back(TimeMeasure(now, hint));
}

void ShowTimeMeasures() {
	TimeMeasures::iterator time_measure;
	String displayed_measures;
	SYSTEMTIME systime;

	for (time_measure = time_measures.begin(); time_measure != time_measures.end(); time_measure++) {
		FileTimeToSystemTime(&time_measure->time, &systime);
		displayed_measures += "[" + String(systime.wHour) + "h:" + String(systime.wMinute) + "m:" + String(systime.wSecond) + "s:" + String(systime.wMilliseconds) + "ms] " + time_measure->hint + "\n";
	}

	ShowMessage(displayed_measures);
}