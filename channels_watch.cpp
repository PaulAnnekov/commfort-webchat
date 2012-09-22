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

#include "channels_watch.h"
#include "virtual_users.h"
#include "notif.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace std;

MapList ChannelsUsers;

// ƒобавл€ет ассоциацию виртуального пользовател€ с каналом.
// “ем самым пользователь становитс€ пользователем, который отслеживает данный канал.
void AddChannelMonitoring(String channel, String user) {
	ChannelsUsers.insert(pair<String, String>(channel, user));
	channels.push_back(Channel(channel));
}

// ”дал€ет канал из списка отслеживаемых.
void DeleteChannelMonitoring(String channel) {
	MapList::iterator it = ChannelsUsers.find(channel);
	if (it != ChannelsUsers.end()) {
		ChannelsUsers.erase(it);
		DeleteChannel(channel);
	}
}

// ѕровер€ет отслеживаетс€ ли канал.
bool ChannelExists(String channel) {
	bool result = false;

	MapList::iterator it = ChannelsUsers.find(channel);
	if (it != ChannelsUsers.end()) {
		result = true;
	}

	return result;
}

// ѕровер€ет отслеживает ли данный пользователь указанный канал.
bool UserIsMonitoring(String user, String channel) {
	bool user_is_monitor = false;

	MapList::iterator it = ChannelsUsers.find(channel);
	if (it != ChannelsUsers.end()) {
		if (it->second == user) {
			user_is_monitor = true;
		}
	}

	return user_is_monitor;
}

// «амен€ет пользовател€, который отслеживает данный канал, другим пользователем.
void ReplaceUserInChannel(String channel, String user) {
	MapList::iterator it = ChannelsUsers.find(channel);

	if (it != NULL) {
        it->second = user;
	}
}

// ѕровер€ет список ассоциаций, когда виртуальный пользователь отключаетс€.
void CheckChannelsOnUserDisconnect(String user) {
	MapList::iterator it;

	for (it = ChannelsUsers.begin(); it != ChannelsUsers.end(); it++) {
		if (it->second == user) {
			String new_user = GetFirstUserInChannel(it->first);
			if (!new_user.IsEmpty()) {
				it->second = new_user;
			} else {
				ChannelsUsers.erase(it);
				DeleteChannel(it->first);
            }
		}
	}
}