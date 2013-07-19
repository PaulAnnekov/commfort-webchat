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

#ifndef user_channelH
#define user_channelH

#include "global.h"

using namespace std;

typedef multimap<String, String> UserChannelMap;
extern UserChannelMap user_channel;

void InitializeUserChannelMap();
void DeleteUserChannelMap();

void AddUserChannel(String user, String channel);
StringList GetChannelUsers(String channel_name);
bool IsUserInChannel(String user, String channel);
void DeleteUserChannel(String user, String channel);
void RemoveUserFromChannels(String user);
StringList GetUserChannels(String user);

//---------------------------------------------------------------------------
#endif
