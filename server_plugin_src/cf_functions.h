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

#ifndef cf_functionsH
#define cf_functionsH

#include "plug_init.h"
#include "channels.h"
#include <WideStrings.hpp>
#include <vector>

//---------------------------------------------------------------------------
struct CFUserInfo {
	String ip, public_ip, computer_id, state, client_version, process;
	DWORD male, window_activity, downtime;
};

struct RequestData {
	RequestData(BYTE *_data, DWORD _len): data(_data), len(_len) {};
	BYTE *data;
	DWORD len;
};

String GetPluginDirectory();
CFUsersList GetChatUsersList();
CFUsersList GetChatOnlineUsersList();
StringList GetChannelUsersList(String user, String channel);
CFChannelsList GetChannelsList();
String GetComputerID(String name);
bool IsHiddenIP(String name);
CFUserInfo GetUserInfo(String nick);
void UserConnect(String name, String ip, bool pass_encrypt, String pass, int is_female);
void UserDisconnect(String name);
bool UserChannelConnect(String user_name, String chnl_name, int visibility, int enter_type);
bool UserChannelDisconnect(String user_name, String chnl_name);
StringList UserChannels(String user_name);
bool GetUserRegState(String nick);
Channel GetChannelInfo(String channel_name);
bool SendChannelMessage(String name, unsigned int rezim, String channel, String message);
void SetUserState(String name, String state);
//---------------------------------------------------------------------------
#endif
