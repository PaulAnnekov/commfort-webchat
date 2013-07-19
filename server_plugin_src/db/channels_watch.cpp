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
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace std;

CRITICAL_SECTION channels_watchers_map_cs;
MapList channels_watchers;

void InitializeChannelsWatchersMap() {
	InitializeCriticalSection(&channels_watchers_map_cs);
}

void DeleteChannelsWatchersMap() {
	DeleteCriticalSection(&channels_watchers_map_cs);
	channels_watchers.clear();
}

// Добавляет ассоциацию виртуального пользователя с каналом.
// Тем самым пользователь становится пользователем, который отслеживает данный канал.
void AddChannelWatcher(String user, String channel) {
	try {
		EnterCriticalSection(&channels_watchers_map_cs);

		channels_watchers.insert(pair<String, String>(channel, user));
	} catch (Exception *E) {
		InfoAction("(6000) " + __("ChannelsWatchersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(6000) " + __("ChannelsWatchersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_watchers_map_cs);
}

// Check channel monitoring state.
bool IsChannelMonitored(String channel) {
	bool result = false;

	try {
		EnterCriticalSection(&channels_watchers_map_cs);

		MapList::iterator item = channels_watchers.find(channel);
		if (item != channels_watchers.end()) {
			result = true;
		}
	} catch (Exception *E) {
		InfoAction("(6001) " + __("ChannelsWatchersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(6001) " + __("ChannelsWatchersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_watchers_map_cs);

	return result;
}

// Check if uch user is monitoring the channel.
bool UserIsMonitoring(String user, String channel) {
	bool user_is_monitor = false;

	try {
		EnterCriticalSection(&channels_watchers_map_cs);

		MapList::iterator item = channels_watchers.find(channel);
		if (item != channels_watchers.end() && item->second == user) {
			user_is_monitor = true;
		}
    } catch (Exception *E) {
		InfoAction("(6002) " + __("ChannelsWatchersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(6002) " + __("ChannelsWatchersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_watchers_map_cs);

	return user_is_monitor;
}

// Check if such user is monitoring the channel.
void RemoveChannelWatcher(String channel) {
	try {
		EnterCriticalSection(&channels_watchers_map_cs);

		MapList::iterator item = channels_watchers.find(channel);
		if (item != channels_watchers.end()) {
			channels_watchers.erase(item);
		}
    } catch (Exception *E) {
		InfoAction("(6003) " + __("ChannelsWatchersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(6003) " + __("ChannelsWatchersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_watchers_map_cs);
}

// Set the first virtual user in this channel as watcher.
// Returns true if channel watcher changed or false if there are not any virtual user in channel.
bool ChangeChannelWatcher(String channel) {
	return false;
}

// Check channels on virtual user disconnect.
void CheckChannelsOnUserDisconnect(String user) {
    try {
		EnterCriticalSection(&channels_watchers_map_cs);

		MapList::iterator item;
		bool is_changed = false;
		for (item = channels_watchers.begin(); item != channels_watchers.end(); item++) {
			if (item->second == user) {
				is_changed = ChangeChannelWatcher(item->first);
                // if new watcher wasn't found then delete this channel from watching.
				if (!is_changed) {
					channels_watchers.erase(item);
				}
			}
		}
    } catch (Exception *E) {
		InfoAction("(6004) " + __("ChannelsWatchersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(6004) " + __("ChannelsWatchersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_watchers_map_cs);
}
