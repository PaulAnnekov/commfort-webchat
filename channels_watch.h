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

#ifndef channels_watchH
#define channels_watchH

#include "global.h"

// Ассоциативный массив канал=виртуальный пользователь, который отслеживает этот канал.
extern MapList ChannelsUsers;

extern void AddChannelMonitoring(String channel, String user);
extern void DeleteChannelMonitoring(String channel);
extern bool ChannelExists(String channel);
extern void ReplaceUserInChannel(String channel, String user);
extern bool UserIsMonitoring(String user, String channel);
extern void CheckChannelsOnUserDisconnect(String user);
//---------------------------------------------------------------------------
#endif
