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

#ifndef json_mainH
#define json_mainH
#include <System.hpp>
#include <map>
#include "global.h"
#include "db\connections.h"
#include "socket_utils.h"

using namespace std;
//---------------------------------------------------------------------------

const int meth_n = 17;
const String methods[meth_n] = {
	"conn_init",
	"check_conn_id",
	"conn_close",
	"user_connect",
	"user_reg",
	"get_auth_state",
	"get_actions",
	"send_public_mess",
	"get_chnls_list",
	"get_chnl_users",
	"get_online_users",
	"get_user_channels",
	"user_disconnect",
	"get_image",
	"channel_connect",
	"channel_disconnect",
	"get_own_user_name"
};

struct JSONERROR {
	JSONERROR(int e_type, String e_data, String e_id = NULL);
	int type;
	String data, id;
};

typedef map<String, String> Params;

struct PREQUEST {
	PREQUEST(String _id, int _meth_id, String _meth_name, Params _params) : id(_id), meth_id(_meth_id), meth_name(_meth_name), params(_params) { };
	int meth_id;
	String meth_name, id;
	Params params;
};

typedef PREQUEST pRequest;


JSONERROR JSONError(int type, String data);
JSONERROR JSONError(int type, String data, String id);
int GetMethodID(String method);
String GetMemberValue(map<String, String> *method_obj, String member, bool is_req, String def, int type);
String GetMemberValue(Params *method_obj, String member);
String CallMethod(String id, int meth_id, Params params, Socket—onnections::iterator socket_connection, int transport);
pRequest CheckObject(map<String, String> method_obj);
extern void SendNotification(String method, String json_object, LogicalConnections::iterator connection_ref, bool single_value);
extern void SendNotification(String method, String json_object, LogicalConnectionsRef connections);
#endif
