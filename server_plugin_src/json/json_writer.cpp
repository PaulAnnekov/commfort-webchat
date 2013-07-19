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

#include "json_writer.h"
#include "json_main.h"
#include "notif.h"
#include "methods.h"
#include "debug.h"
#include "l18n.h"
#include "socket_listen_thread.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

String SetJSONValue(String line) {
	bool is_integer;
	String result;

	if (!line.IsEmpty()) {
		try {
			line.ToInt();
		} catch (EConvertError *) {
			is_integer = false;
		}

		if (is_integer || line.LowerCase() == "null") {
			result = line;
		} else {
			result = "\"" + line + "\"";
		}
	} else {
		result = "null";
	}

	return result;
}

String GetEscapedValue(String value) {
	value = StringReplace(value, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\"", "\\\"", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "/", "\\/", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\r", "\\r", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\n", "\\n", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\b", "\\b", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\f", "\\f", TReplaceFlags() << rfReplaceAll);
	value = StringReplace(value, "\t", "\\t", TReplaceFlags() << rfReplaceAll);

	return value;
}

String GetEscapedStringValue(String value) {
	return "\"" + GetEscapedValue(value) + "\"";
}

// Generate error.
String BuildJSONError(int code, String data, String id) {
	String message;

	switch (code) {
		case -32700:
			message = "Parse error";
		break;
		case -32600:
			message = "Invalid Request";
		break;
		case -32601:
			message = "Method not found";
		break;
		case -32602:
			message = "Invalid params";
		break;
		case -32603:
			message = "Internal error";
		break;
		default:
			message = "Server error";
		break;
	}

	id = SetJSONValue(id);
	return Format("{\"jsonrpc\":\"2.0\",\"error\":{\"code\":%d,\"message\":\"%s\",\"data\":\"%s\"},\"id\":%s}", ARRAYOFCONST((code, message, data, id)));
}

void SetJSONResponseObject(String &result_obj, String method, String id = "") {
	if (!id.IsEmpty()) {
		id = ",\"id\":" + SetJSONValue(id);
	}

	result_obj = Format("{\"jsonrpc\":\"%s\",\"method\":\"%s\",\"result\":%s%s}", ARRAYOFCONST((json_rpc_version, method, result_obj, id)));
}

void AddPair(String &line, String name, String value, bool string, bool need_comma) {
	if (string) {
		value = (!value.IsEmpty()) ? GetEscapedStringValue(value) : String("null");
	}
	line += "\"" + name + "\":" + value;
	line += (need_comma) ? "," : "";
}

// Set value from the map list.
String SetParametersObject(MapList params, bool last_param_array) {
    MapList::iterator it;
	MapList::iterator last_param = params.end();
	last_param--;

    String params_object = "";

	for (it = params.begin(); it != params.end(); it++) {
		AddPair(params_object, it->first, it->second, (last_param == it && last_param_array) ? false : true, (last_param != it) ? true : false);
	}

	return params_object;
}

