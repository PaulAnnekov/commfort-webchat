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

#include "auth_queue.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION auth_queue_map_cs;
AuthQueueMap auth_queue;

void InitializeAuthQueueMap() {
	InitializeCriticalSection(&auth_queue_map_cs);
}

void DeleteAuthQueueMap() {
	DeleteCriticalSection(&auth_queue_map_cs);
	auth_queue.clear();
}

void AddAuthQueue(LogicalConnections::iterator connection, String user) {
	try {
		EnterCriticalSection(&auth_queue_map_cs);

		auth_queue.insert(pair<String, LogicalConnections::iterator>(user, connection));
	} catch (Exception *E) {
		InfoAction("(5000) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5000) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);
}

bool IsAuthQueueElement(AuthQueueMap::iterator element) {
	bool is_exists = false;

	try {
		EnterCriticalSection(&auth_queue_map_cs);

		if (element != auth_queue.end()) {
        	is_exists = true;
		}
	} catch (Exception *E) {
		InfoAction("(5001) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5001) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);

	return is_exists;
}

AuthQueueMap::iterator FindAuthQueueByUser(String user) {
	AuthQueueMap::iterator result;

	try {
		EnterCriticalSection(&auth_queue_map_cs);

		result = auth_queue.end();
		AuthQueueMap::iterator item = auth_queue.find(user);
		if (item != auth_queue.end()) {
			result = item;
		}
	} catch (Exception *E) {
		InfoAction("(5002) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5002) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);

	return result;
}

LogicalConnections::iterator GetAuthQueueConnection(AuthQueueMap::iterator element) {
	LogicalConnections::iterator result;

	try {
		EnterCriticalSection(&auth_queue_map_cs);

		if (element != auth_queue.end()) {
			result = element->second;
		}
	} catch (Exception *E) {
		InfoAction("(5003) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5003) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);

	return result;
}

void DeleteAuthQueueItem(AuthQueueMap::iterator item) {
	try {
		EnterCriticalSection(&auth_queue_map_cs);

		if (item != auth_queue.end()) {
			auth_queue.erase(item);
		}
	} catch (Exception *E) {
		InfoAction("(5004) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5004) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);
}

void RemoveAuthQueueByUser(String user) {
	try {
		EnterCriticalSection(&auth_queue_map_cs);

		AuthQueueMap::iterator item = auth_queue.find(user);
		if (item != auth_queue.end()) {
			auth_queue.erase(item);
		}
	} catch (Exception *E) {
		InfoAction("(5005) " + __("AuthQueueManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(5005) " + __("AuthQueueManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&auth_queue_map_cs);
}
