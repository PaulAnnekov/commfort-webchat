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

#ifndef auth_queueH
#define auth_queueH

#include <map>
#include "connections.h"

using namespace std;

typedef map<String, LogicalConnections::iterator> AuthQueueMap;
extern AuthQueueMap auth_queue;

void InitializeAuthQueueMap();
void DeleteAuthQueueMap();

void AddAuthQueue(LogicalConnections::iterator connection, String user);
AuthQueueMap::iterator FindAuthQueueByUser(String user);
bool IsAuthQueueElement(AuthQueueMap::iterator element);
LogicalConnections::iterator GetAuthQueueConnection(AuthQueueMap::iterator element);
void DeleteAuthQueueItem(AuthQueueMap::iterator item);
void RemoveAuthQueueByUser(String user);
//---------------------------------------------------------------------------
#endif
