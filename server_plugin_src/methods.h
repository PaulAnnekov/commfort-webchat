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
#ifndef methodsH
#define methodsH

#include <System.hpp>
#include "connections.h"
#include "socket_utils.h"
//---------------------------------------------------------------------------

namespace meth {
	String ConnectionInit(String app_name, Socket—onnections::iterator socket_connection, int transport);
    String CheckConnectionID(String conn_id);
	String ConnectionClose(String conn_id);
	//void ConnectionInit(String user_nick, String user_pass, String user_sex, String pass_encrypt, String app_name, String conn_type, SOCKET sd, String &response);
	String VirtualUserConnect(LogicalConnections::iterator conn, String user_nick, String user_pass);
	String VirtualUserRegister(LogicalConnections::iterator conn, String user_nick, String user_pass, String is_female);
	void VirtualUserDisconnect(LogicalConnections::iterator conn);
	String GetActions(LogicalConnections::iterator conn, SOCKET sd);
	void SendPublicMessage(LogicalConnections::iterator conn, String channel, String mode, String message, String &response);

	void GetAuthState(String conn_id, String &response);
	void GetChannelsList(LogicalConnections::iterator conn, String user_chnls, String &response);
	void GetChannelUsers(LogicalConnections::iterator conn, String channel_name, String &response);
	void GetOnlineUsers(LogicalConnections::iterator conn, String &response);
	void GetUserChannels(LogicalConnections::iterator conn, String &response);
	void GetImage(LogicalConnections::iterator conn, String image_id, String &response);
	void UserChannelConnect(LogicalConnections::iterator conn, String channel, String visibile, String enter_type, String &response);
	void UserChannelDisconnect(LogicalConnections::iterator conn, String channel, String &response);
	void GetOwnUserName(LogicalConnections::iterator conn, String &response);
};
#endif
