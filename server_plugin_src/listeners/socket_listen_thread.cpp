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

#include "socket_listen_thread.h"

#include "notif.h"
#include "l18n.h"
#include "json_reader.h"
#include "debug.h"
#include "main_form.h"
#include "connections.h"
#include <process.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

HANDLE SocketThread;
SOCKET simple_listening_socket;
CRITICAL_SECTION socket_write_cs;
Socket—onnections socket_connections;

unsigned __stdcall SocketListenLoop(void* pArguments) {
	try {
		InitializeCriticalSection(&socket_write_cs);
		SocketAccept(simple_listening_socket, socket_connections, GetCleanMessageLength, true);
		SocketClose(simple_listening_socket);
	} __finally {
		DeleteCriticalSection(&socket_write_cs);
		_endthreadex(0);
	}

	return 0;
}

bool SocketThreadStart() {
	//socket_listen_stop = 0;
	bool success = true;

	try {
		simple_listening_socket = SocketListen(9751);
		SocketThread = (HANDLE)_beginthreadex( NULL, 0, &SocketListenLoop, NULL, 0, NULL);
		if (SocketThread == 0) {
			throw Exception(__("ThreadMemoryError"));
		}
	} catch (Exception *E) {
		success = false;
		InfoAction(__("SocketThreadStartError") + ". " + E->Message, false, 3, 2);
	}

	return success;
}

// Gets the JSON request length from the first 4 bytes received.
// Returns the number of copied bytes from the received data.
int GetCleanMessageLength(Socket—onnection &conn, char *received_data, int len) {
	int copy_to_end = 4 - conn.in_buf_length;

	if (len < copy_to_end) {
		copy_to_end = len;
	} else {
		conn.json_length_received = true;
	}

	char *tmp = new char[conn.in_buf_length + copy_to_end];
	memcpy(tmp, conn.in_buffer, conn.in_buf_length);
	memcpy(tmp + conn.in_buf_length, received_data, copy_to_end);

	if (conn.json_length_received) {
		/* TODO : —‰ÂÎ‡Ú¸ Ó·‡·ÓÚÍÛ Ó¯Ë·ÍË ÔË‚Â‰ÂÌËˇ ÚËÔ‡ */
		conn.json_length = *((int *)tmp);
		delete [] tmp;
	} else {
		delete [] conn.in_buffer;
		conn.in_buffer = NULL;
		conn.in_buffer = tmp;
		conn.in_buf_length += copy_to_end;
	}

	return copy_to_end;
}
