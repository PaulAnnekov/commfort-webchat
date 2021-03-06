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
#ifndef json_readerH
#define json_readerH

#include <System.hpp>
#include "json_main.h"
using namespace std;
//---------------------------------------------------------------------------


String BuildJSONError(int code, String data);

String ParseJSONValue(String *req);
String ParseJSONString(String *req);
void ParseJSONToValue(String *req);
String GetJSONStructure(String *req);
Params ParseJSONObject(String req_obj);
bool IsJSONStructure(String &value);
bool IsJSONString(String &value);
String ParseJSONRequest(String request, SocketСonnections::iterator socket_connection, int transport);
String BuildJSONResult(map<int, String>);

#endif
