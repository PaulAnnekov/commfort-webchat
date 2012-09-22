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

#ifndef plug_initH
#define plug_initH

#include "global.h"
#include <map>
#include <vector>

using namespace std;

struct CFUser {
	CFUser(String _name, String _ip, int _sex) : name(_name), ip(_ip), sex(_sex) { };
	String name, ip;
	bool sex;
};

typedef vector<CFUser> CFUsersList;

struct CFChannelShortInfo {
	CFChannelShortInfo(String _name, int _users_online, String _topic) : name(_name), users_online(_users_online), topic(_topic) { };
	String name, topic;
	int users_online;
};

typedef vector<CFChannelShortInfo> CFChannelsList;

extern MapList watched_channels;

void LoadBasePluginInfo();
void CheckActionOptions();
void ConnectBot();
bool SetBaseInfo();
void StartSync();
void FinishSync();
void DisconnectBot();
void UpdateUsersList();
//---------------------------------------------------------------------------
#endif
