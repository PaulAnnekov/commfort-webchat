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

#ifndef http_listen_threadH
#define http_listen_threadH

#include "socket_utils.h"
#include <list>
//---------------------------------------------------------------------------

using namespace std;

//extern volatile long http_listen_stop;
extern SOCKET http_listening_socket;
extern Socket—onnections http_connections;
extern HANDLE HTTPThread;
//void SetupHTTPFDSet(fd_set& ReadFDs);
//void AddNewHTTPConnection(SOCKET sd);
//void CheckHTTPSockets(fd_set &ReadFDs, fd_set &ExceptFDs);

extern int GetHTTPMessageLength(Socket—onnection &conn, char *received_data, int len);

extern bool HTTPThreadStart();
//extern void HTTPThreadStop();
//extern void HTTPAccept(SOCKET listening_sock);

// HTTP ÒÓÂ‰ËÌÂÌËÂ.
/*
    struct HTTPConnection {
    	HTTPConnection(SOCKET sd_) : sd(sd_), buffer(NULL), buffer_length(0), headers_count(0), content_length(0), headers_received(false) { };
    	SOCKET sd;
    	char *buffer;
    	int buffer_length, headers_count, content_length;
    	bool headers_received;
    };

    typedef list<HTTPConnection> HTTPConnections;
    extern HTTPConnections http_connections;
*/

#endif
