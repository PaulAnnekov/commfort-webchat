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

#include "user_channel.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION user_channel_map_cs;
UserChannelMap user_channel;

void InitializeUserChannelMap() {
	InitializeCriticalSection(&user_channel_map_cs);
}

void DeleteUserChannelMap() {
	DeleteCriticalSection(&user_channel_map_cs);
	user_channel.clear();
}

void AddUserChannel(String user, String channel) {
	try {
		EnterCriticalSection(&user_channel_map_cs);

		user_channel.insert(pair<String, String>(user, channel));
	} catch (Exception *E) {
		InfoAction("(4000) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4000) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);
}

// Get list of channel users.
StringList GetChannelUsers(String channel_name) {
	StringList users;

	try {
		EnterCriticalSection(&user_channel_map_cs);

		if (!user_channel.empty()) {
			UserChannelMap::iterator item;
			for (item = user_channel.begin(); item != user_channel.end(); item++) {
				if (item->second == channel_name) {
					users.push_back(item->first);
				}
			}
		}
	} catch (Exception *E) {
		InfoAction("(4001) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4001) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);

	return users;
}

// Check if user in channel.
bool IsUserInChannel(String user, String channel) {
	bool is_user_in_channel = false;

	try {
		EnterCriticalSection(&user_channel_map_cs);

		if (!user_channel.empty()) {
			UserChannelMap::iterator item;
			for (item = user_channel.begin(); item != user_channel.end(); item++) {
				if (item->first == user && item->second == channel) {
					is_user_in_channel = true;
					break;
				}
			}
		}
	} catch (Exception *E) {
		InfoAction("(4002) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4002) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);

	return is_user_in_channel;
}

// Remove user from specified channel.
void DeleteUserChannel(String user, String channel) {
	try {
		EnterCriticalSection(&user_channel_map_cs);

		if (!user_channel.empty()) {
			UserChannelMap::iterator item;
			for (item = user_channel.begin(); item != user_channel.end(); item++) {
				if (item->first == user && item->second == channel) {
					user_channel.erase(item);
					break;
				}
			}
		}
	} catch (Exception *E) {
		InfoAction("(4003) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4003) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);
}

// Remove user from all channels.
void RemoveUserFromChannels(String user) {
	try {
		EnterCriticalSection(&user_channel_map_cs);

		if (!user_channel.empty()) {
			UserChannelMap::iterator item = user_channel.begin(), next_item;
			while (item != user_channel.end()) {
				if (item->first == user) {
                    // Record next item iterator, so after delete current iterator we will now who is next.
					next_item = item;
					next_item++;
					user_channel.erase(item);
					item = next_item;
				} else {
                    item++;
                }
			}
		}
	} catch (Exception *E) {
		InfoAction("(4004) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4004) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);
}

// Get list of user channels.
StringList GetUserChannels(String user) {
	StringList channels;

	try {
		EnterCriticalSection(&user_channel_map_cs);

		if (!user_channel.empty()) {
			UserChannelMap::iterator item;
			for (item = user_channel.begin(); item != user_channel.end(); item++) {
				if (item->first == user) {
					channels.push_back(item->second);
				}
			}
		}
	} catch (Exception *E) {
		InfoAction("(4005) " + __("UserChannelManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(4005) " + __("UserChannelManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&user_channel_map_cs);

	return channels;
}
