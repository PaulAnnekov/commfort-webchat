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

#ifndef json_writerH
#define json_writerH

#include <System.hpp>
#include "global.h"
//#include <windows.h>
//#include <mysql.h>

extern void SetJSONResponseObject(String &result_obj, String method, String id);
extern String BuildJSONError(int code, String data, String id);
extern String SetParametersObject(MapList params, bool last_param_array = false);
extern void AddPair(String &line, String name, String value, bool string = true, bool need_comma = true);
//---------------------------------------------------------------------------
#endif
