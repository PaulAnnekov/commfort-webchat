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

#include "http_messages_queue.h"
#include "notif.h"
#include "l18n.h"
#include "debug.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION messages_map_cs;
MessagesMap messages_map;

void InitializeMessagesMap() {
	InitializeCriticalSection(&messages_map_cs);
}

void DeleteMessagesMap() {
	DeleteCriticalSection(&messages_map_cs);
	messages_map.clear();
}

void AddMessagesMap(String connection_id, String message) {
	try {
		EnterCriticalSection(&messages_map_cs);

		MessagesMap::iterator item = messages_map.find(connection_id);
		if (item != messages_map.end()) {
			item->second += "," + message;
		} else {
			messages_map.insert(pair<String, String>(connection_id, message));
		}
	} catch (Exception *E) {
		InfoAction("(7000) " + __("HTTPMessagesManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(7000) " + __("HTTPMessagesManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&messages_map_cs);
}

String GetMessagesByConnection(String connection_id) {
	String messages;

	try {
		EnterCriticalSection(&messages_map_cs);

		MessagesMap::iterator item = messages_map.find(connection_id);
		if (item != messages_map.end()) {
			messages = "[" + item->second + "]";
            messages_map.erase(item);
		}
	} catch (Exception *E) {
		InfoAction("(7001) " + __("HTTPMessagesManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(7001) " + __("HTTPMessagesManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&messages_map_cs);

	return messages;
}
