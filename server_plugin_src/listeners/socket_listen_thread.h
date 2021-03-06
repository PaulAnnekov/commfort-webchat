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

#ifndef socket_listen_threadH
#define socket_listen_threadH

#include "socket_utils.h"

//---------------------------------------------------------------------------

using namespace std;
extern SOCKET simple_listening_socket;
extern CRITICAL_SECTION socket_write_cs;
extern SocketСonnections socket_connections;
extern HANDLE SocketThread;

extern int GetCleanMessageLength(SocketСonnection &conn, char *received_data, int len);
extern bool SocketThreadStart();
#endif
