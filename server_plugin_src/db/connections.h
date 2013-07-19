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

#ifndef connectionsH
#define connectionsH
#include <list>
#include "online_users.h"
#include <winsock2.h>
//---------------------------------------------------------------------------

// Логическое соединение (пользователь).
struct LogicalConnection {
    LogicalConnection(){};
	LogicalConnection(String _conn_id, String _app_name, int _time, String _ip, SOCKET _sock, bool _auto_actions_get, unsigned int _transport) :
	conn_id(_conn_id), app_name(_app_name), auth_status(7), time(_time), last_act_t(_time), ip(_ip), sock(_sock),
	user(GetEmptyOnlineUser()), auto_actions_get(_auto_actions_get), transport(_transport) {};
	SOCKET sock;
	String conn_id, app_name, ip;
	OnlineUsersList::iterator user;
	unsigned int time, last_act_t, auth_status, transport;
	bool auto_actions_get;
};

typedef list<LogicalConnection> LogicalConnections;
typedef vector<LogicalConnections::iterator> LogicalConnectionsRef;
extern LogicalConnections logical_connections;

extern LogicalConnections::iterator SearchConnection(String conn_id);
extern LogicalConnections::iterator SearchBySocket(SOCKET &sock);
extern LogicalConnections::iterator SearchByIP(String ip);
extern LogicalConnections::iterator FindConnectionByUser(String user);
extern LogicalConnectionsRef GetChannelConnections(String channel);
LogicalConnectionsRef GetConnectionsRef(bool only_online_users);
LogicalConnections GetConnections();
extern String AddConnection(String app_name, String ip, SOCKET sock, bool auto_actions_get, unsigned int transport);
extern void SetConnectionUser(LogicalConnections::iterator conn, OnlineUsersList::iterator user);
extern void UnsetConnectionUser(String user);
extern void UnsetConnectionUser(LogicalConnections::iterator conn);
extern void ConnectionUserDisconnect(LogicalConnections::iterator conn);
extern void FullUserConnectionDelete(LogicalConnections::iterator conn);
extern void DeleteConnection(String conn_id);
extern void DeleteConnection(LogicalConnections::iterator conn);
extern void DeleteExpiredConnections();
extern void UpdateConnectionLastAction(LogicalConnections::iterator conn);
extern void SetConnectionActionsGet(LogicalConnections::iterator connection, bool auto_actions_get);
extern void SetConnectionSocket(LogicalConnections::iterator connection, SOCKET sd);
extern void SetAuthorizationStatus(String conn_id, int authorization_status);
void SetAuthorizationStatus(LogicalConnections::iterator connection, int authorization_status);
extern int GetAuthorizationStatus(LogicalConnections::iterator connection);
extern int GetAuthorizationStatus(String conn_id);
extern bool IsHTTPConnection(LogicalConnections::iterator connection);
extern bool ConnectionExists(String conn_id);
extern bool ConnectionExists(LogicalConnections::iterator connection);
extern bool GetUserOnlineStatus(LogicalConnections::iterator conn);
extern OnlineUsersList::iterator GetConnectionUser(LogicalConnections::iterator connection);
extern int GetConnectionUserChannelsNumber(LogicalConnections::iterator conn);
extern String GetConnectionID(LogicalConnections::iterator connection);
extern String GetConnectionUserName(LogicalConnections::iterator conn);
String GetConnectionIP(LogicalConnections::iterator connection);
SOCKET GetConnectionSocket(LogicalConnections::iterator connection);
LogicalConnection GetConnection(LogicalConnections::iterator connection_ref);

void clear();
//---------------------------------------------------------------------------

#endif
