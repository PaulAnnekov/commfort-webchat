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

#ifndef socket_utilsH
#define socket_utilsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "plug_init.h"
#include <winsock2.h>
#include <list>
//---------------------------------------------------------------------------
// Ïîëíàÿ èíôîğìàöèÿ ïî ôèçè÷åñêîìó ñîåäèíåíèş.
struct SocketÑonnection {
	SocketÑonnection(SOCKET _sd, String _ip) : sd(_sd), ip(_ip), in_buffer(NULL), out_buffer(NULL), in_buf_length(0),
	out_buf_length(0), json_length(0), json_length_received(false) { };
	SOCKET sd;
	char *in_buffer, *out_buffer;
	unsigned int in_buf_length, out_buf_length, json_length;
	bool json_length_received;
	String ip;
};

typedef list<SocketÑonnection> SocketÑonnections;

extern SOCKET SocketListen(int port);
extern void SocketAccept(SOCKET listening_sock, SocketÑonnections &connections, int (*GetSocketMessageLength)(SocketÑonnection &, char *, int), bool is_persistent);
extern void SocketClose(SOCKET sd);
extern String GetSocketError(SOCKET *sock);
extern String GetSocketIP(SOCKET sd);
bool ValidateIP(String ip);
extern volatile long listen_stop;
extern void SocketWrite(SOCKET sd, String response, bool header_needed);
//---------------------------------------------------------------------------
#endif
