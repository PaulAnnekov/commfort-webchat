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

#include "channels.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION channels_list_cs;
ChannelsList channels;

void InitializeChannelsList() {
	InitializeCriticalSection(&channels_list_cs);
}

void DeleteChannelsList() {
	DeleteCriticalSection(&channels_list_cs);
	channels.clear();
}

void AddChannel(String name, unsigned int online_users, String topic) {
    try {
		EnterCriticalSection(&channels_list_cs);

		channels.push_back(Channel(name, online_users, topic));
	} catch (Exception *E) {
		InfoAction("(3000) " + __("ChannelsManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(3000) " + __("ChannelsManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_list_cs);
}

void UpdateChannel(ChannelsList::iterator channel, Channel channel_info) {
    try {
		EnterCriticalSection(&channels_list_cs);

		*channel = channel_info;
	} catch (Exception *E) {
		InfoAction("(3001) " + __("ChannelsManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(3001) " + __("ChannelsManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_list_cs);
}

ChannelsList::iterator FindChannelByName(String name) {
	ChannelsList::iterator result;

	try {
		EnterCriticalSection(&channels_list_cs);

		ChannelsList::iterator channel;
		result = channels.end();
		for (channel = channels.begin(); channel != channels.end(); channel++) {
			if (channel->name == name) {
				result = channel;
				break;
			}
		}
	} catch (Exception *E) {
		InfoAction("(3002) " + __("ChannelsManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(3002) " + __("ChannelsManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_list_cs);

	return result;
}

void UpdateChannelTopic(String channel_name, String topic, String topic_editor, unsigned int topic_date) {
	try {
		EnterCriticalSection(&channels_list_cs);

		ChannelsList::iterator channel = FindChannelByName(channel_name);
		if (channel != channels.end()) {
			channel->topic = topic;
			channel->topic_editor = topic_editor;
			channel->topic_date = topic_date;
		}
	} catch (Exception *E) {
		InfoAction("(3003) " + __("ChannelsManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(3003) " + __("ChannelsManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_list_cs);
}

ChannelsList GetChannels() {
	ChannelsList channels_copy;

	try {
		EnterCriticalSection(&channels_list_cs);

		channels_copy = channels;
	} catch (Exception *E) {
		InfoAction("(3004) " + __("ChannelsManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(3004) " + __("ChannelsManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&channels_list_cs);

	return channels_copy;
}
