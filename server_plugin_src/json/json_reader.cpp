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

#include "json_reader.h"
#include "json_writer.h"
#include "debug.h"
#include <SysUtils.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

const char json_white_space[4] = {0x0D, 0x20, 0x09, 0x0A};

// Build summary result of client request.
void BuildJSONResult(map<int, String> objects, String &response) {
	int count = objects.size(), i = 0;

	if (count > 1) {
		map<int, String>::iterator pair;
		response = "[";

		for(pair = objects.begin(); pair != objects.end(); pair++) {
			i++;
			response += pair->second;
			if (i != count) {
				response += ",";
			}
		}

		response += "]";
	} else if (count == 1) {
		response = objects.begin()->second;
	} else {
        response = "";
    }
}

bool JSONWhiteSpace(String req) {
    /*
		 Insignificant whitespace is allowed before or after any of the six
		 structural characters.
		 ws = *(
			 %x20 /              ; Space
			 %x09 /              ; Horizontal tab
			 %x0A /              ; Line feed or New line
			 %x0D                ; Carriage return
		 )
	 */
	bool result = false;

	for (int i = 0; i <= 3; i++) {
		if (json_white_space[i] == req[1]) {
			result = true;
			break;
		}
    }

	return result;
}

void JSONClearWS(String &req) {
	if (JSONWhiteSpace(req[1])) {
		req = req.Delete(1, 1);
	}
}

// Get integer\literal from full request string.
String ParseJSONValue(String *req) {
	bool is_end = false;
	String str = "",
		   err = NULL;
	int i = 0;

	try {
		while (!is_end) {
			// Add symbols to string till one of the special symbols appeared.
			if (!JSONWhiteSpace(*req) && (*req)[1] != ',' && (*req)[1] != '}') {
				str += (*req)[1];
				(*req) = (*req).Delete(1, 1);
				i++;
			} else {
				is_end = true;
			}
			if (i > 10 || (*req) == "") {
				throw(Exception("Integer/literal value must be less then 10 symbols"));
			}
		}
	} catch (Exception *E) {
		throw(JSONError(-32700, "Error while parsing integer/literal value: '" + E->Message + "'"));
	}

	return str;
}

// Get string from full request string.
String ParseJSONString(String *req) {
	bool is_end = false;
	String str = "",
		   err = NULL;
	int i = 0;

	(*req) = (*req).Delete(1, 1);
	try {
		while (!is_end) {
			// Delete escaped symbols.
			if ((*req)[1] == '\\') {
				str += req->SubString(1, 2);
				*req = req->Delete(1, 2);
			} else {
				// End, if close quote was found.
				if ((*req)[1] == '"') {
					is_end = true;
				} else {
					str += (*req)[1];
				}
				*req = req->Delete(1, 1);
			}
			i++;
			if (i > 41000 || (*req) == "") {
				throw(Exception("String value must be less then 41000 symbols. Maybe you forget to close quotes"));
			}
		}
	} catch (Exception *E) {
		throw(JSONError(-32700, "Error while parsing string value: '" + E->Message + "'"));
	}

	return str;
}

// Delete all symbols from the beginning of the request string till the member value.
void ParseJSONToValue(String &req) {
	try {
		JSONClearWS(req);
		if (req[1] == ':') {
			req = req.Delete(1, 1);
			JSONClearWS(req);
			if (JSONWhiteSpace(req[1])) {
				throw(Exception("Can't find member value after colon. Only one whitespace symbol allowed before and after colon"));
			}
		} else {
			throw(Exception("Member value not found. Colon doesn't exist"));
		}
	} catch (Exception *E) {
		throw(JSONError(-32700, "Error while parsing position of the member value: '" + E->Message + "'"));
	}
}

bool IsJSONStructure(String &value) {
	return (value[1] == '{' || value[1] == '[');
}

bool IsJSONString(String &value) {
	return (value[1] == '"');
}

// Get structure(array, object) string from full request string.
String GetJSONStructure(String *req) {
	bool is_end = false,
		 quote_active = false;
	int l_braces = 1,
		r_braces = 0;
	String st_beg_brace = (*req)[1],
		   st_end_brace,
		   st = "",
		   err = NULL;

	if (st_beg_brace == '{') {
		st_end_brace = '}';
	} else if (st_beg_brace == '[') {
		st_end_brace = ']';
	}

    st += (*req)[1];
	(*req) = (*req).Delete(1, 1);

	try {
		while (!is_end) {
			// Check if quote is still opened.
			if (!quote_active) {
				if ((*req)[1] == st_beg_brace[1]) {
					l_braces++;
				}
				if ((*req)[1] == st_end_brace[1]) {
					r_braces++;
				}
			}

			// Check escaped quotes, when quote is opened.
			if (quote_active && (*req)[1] == '\\') {
				st += (*req).SubString(1, 2);
				(*req) = (*req).Delete(1, 2);
			} else {
				// Open of close quote, if it occured.
					if ((*req)[1] == '"') {
						quote_active = (quote_active) ? false : true;
					}
					st += (*req)[1];
					(*req) = (*req).Delete(1, 1);
			}

			// Check if number of left braces is equal to number of right braces.
			if (l_braces == r_braces) {
				is_end = true;
			} else if ((*req) == "") {
				throw(Exception("Can't get structure(array, object) from request string. Left braces number is not equal to right braces"));
			}
		}
	} catch (Exception *E) {
		throw(JSONError(-32700, "Error appeared while getting structure string from full request string: '" + E->Message + "'"));
	}

	return st;
}

// Get name-value pairs from json representation of the object.
Params ParseJSONObject(String req_obj) {
	String t_var,
		   t_value,
		   t_req_obj = req_obj;
	Params value_pairs;
	int pos = t_req_obj.Pos("\"");

	while (pos != 0) {
		t_req_obj = t_req_obj.Delete(1, pos-1);
		t_var = ParseJSONString(&t_req_obj);

		ParseJSONToValue(t_req_obj);

		if (t_req_obj[1] == '"') {
			t_value = ParseJSONString(&t_req_obj);
		} else if (t_req_obj[1] == '{' || t_req_obj[1] == '[') {
			t_value = GetJSONStructure(&t_req_obj);
		} else {
			t_value = ParseJSONValue(&t_req_obj);
		};

		value_pairs.insert(pair<String,String>(t_var, t_value));

		pos = t_req_obj.Pos("\"");
	}

	return value_pairs;
}

// Parse batch(array of objects) request.
/*
    map<int, String> ParseJSONArray(String request, SOCKET sd) {
    	int parsed_n = 0,
    		next_obj;
    	String t_request = request, err = NULL, response;
    	map<int, String> response_arr;

    	try {
    		next_obj = t_request.Pos("{");
    		if (next_obj) {
    			while (next_obj) {
    				// Delete all letters from beggining till the first left brace.
    				t_request = t_request.Delete(1, next_obj - 1);

    				try {
    					// Get string line of the object and send it to object parser.
    					pRequest req = CheckObject(ParseJSONObject(GetJSONStructure(&t_request)));
    					response = CallMethod(req.id, req.meth_id, req.params, sd);
    					SetJSONResponseObject(req.meth_name, req.id, response);
    					response_arr.insert(pair<int, String>(parsed_n, response));
    				} catch (JSONERROR je) {
    					response_arr.insert(pair<int, String>(parsed_n, BuildJSONError(je.type, je.data, je.id)));
    				}

    				parsed_n++;

    				next_obj = t_request.Pos("{");
    			}
    		} else {
    			throw(Exception("Can't find any object in array(batch)"));
    		}
    	} catch(Exception *E) {
    		response_arr.insert(pair<int, String>(0, BuildJSONError(-32600, "Error while parsing array of requests: '" + E->Message + "'", NULL)));
    	} catch(JSONERROR je) {
    		response_arr.insert(pair<int, String>(0, BuildJSONError(je.type, je.data, je.id)));
    	}

    	return response_arr;
    }
*/

// Parse full request string.
// transport - http or clear sockets:
//   0 - clear sockets,
//   1 - http
String ParseJSONRequest(String request, Socket—onnections::iterator socket_connection, int transport) {
	String response;
	map<int, String> response_arr;

	int parsed_n = 0,
		next_obj;
	String err = NULL;

	// Decode HTTP request's JSON string.
	if (transport == 1) {
		request = TIdURI::URLDecode(request);
	}
	int len = request.Length();

	try {
		if (len <= 0) {
			throw JSONERROR(-32600, "JSON request encoding error");
		}

		if ((request[1] != '[' && request[len] != ']') && (request[1] != '{' && request[len] != '}')) {
			throw JSONERROR(-32600, "JSON request must begin with { or [ and end with } or ] respectively");
		}

		next_obj = request.Pos("{");
		if (!next_obj) {
			throw JSONERROR(-32600, "JSON request must have at least one object");
		}

		bool json_array = (request[1] == '[') ? true : false;
		String json_object;
		while (next_obj) {
			// Delete all letters from beggining till the first left brace.
			request = request.Delete(1, next_obj - 1);

			try {
				// Get string line of the object and send it to object parser.
				if (json_array) {
					json_object = GetJSONStructure(&request);
				} else {
					json_object = request;
					request = "";
				}

				pRequest req = CheckObject(ParseJSONObject(json_object));
				response = CallMethod(req.id, req.meth_id, req.params, socket_connection, transport);
				// If id is empty then it is notification from client so do not return response.
				if (!response.IsEmpty()) {
                	SetJSONResponseObject(response, req.meth_name, req.id);
					response_arr.insert(pair<int, String>(parsed_n, response));
				}
			} catch (JSONERROR je) {
				response_arr.insert(pair<int, String>(parsed_n, BuildJSONError(je.type, je.data, je.id)));
			}

			parsed_n++;
			next_obj = request.Pos("{");
		}
	} catch (JSONERROR je) {
		response_arr.insert(pair<int, String>(0, BuildJSONError(je.type, je.data, NULL)));
	} catch (Exception *E) {
		response_arr.insert(pair<int, String>(0, BuildJSONError(-32603, "Unexpected exception during JSON request parse", NULL)));
	} catch (...) {
		response_arr.insert(pair<int, String>(0, BuildJSONError(-32603, "Unexpected exception during JSON request parse", NULL)));
	}

	BuildJSONResult(response_arr, response);

	return response;
}
