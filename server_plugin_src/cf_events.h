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

#ifndef cf_eventsH
#define cf_eventsH

#include <WideStrings.hpp>

//---------------------------------------------------------------------------

extern void OnUserConnect(BYTE *InBuffer);
extern void OnPublicChannelMessage(BYTE *InBuffer);
extern void OnVirtualUserChannelConnect(BYTE *InBuffer);
extern void OnUserChannelConnect(BYTE *InBuffer);
extern void OnUserChannelDisconnect(BYTE *InBuffer);
extern void OnUserDisconnect(BYTE *InBuffer);
extern void OnUserStateChanged(BYTE *InBuffer);
extern void OnUserSexChanged(BYTE *InBuffer);
extern void OnChannelTopicChanged(BYTE *InBuffer);
extern void OnUnsuccessAuth(BYTE *InBuffer);
extern void OnUserRegistered(BYTE *InBuffer);

#endif
