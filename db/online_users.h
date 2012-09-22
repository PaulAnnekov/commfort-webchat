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

#ifndef online_usersH
#define online_usersH

#include <list>
#include "global.h"
#include "users.h"
using namespace std;

struct OnlineUser {
	OnlineUser(UsersList::iterator _user_ref, String _state, String _client_version, String _process, bool _active, int _delay) :
	user_ref(_user_ref), state(_state), client_version(_client_version), process(_process), active(_active),
	delay(_delay) { };
	UsersList::iterator user_ref;
	String state, client_version, process;
	bool active;
	int delay;
};

typedef list<OnlineUser> OnlineUsersList;
extern OnlineUsersList online_users;

void InitializeOnlineUsersList();
void DeleteOnlineUsersList();

OnlineUsersList::iterator AddOnlineUser(UsersList::iterator user_ref, String state, String client_version, String process, bool active, int delay);
OnlineUsersList::iterator FindOnlineUserByName(String name);
OnlineUsersList::iterator GetEmptyOnlineUser();
bool IsOnlineUserSet(OnlineUsersList::iterator user);
UsersList::iterator GetOnlineUserReference(OnlineUsersList::iterator online_user);
String GetOnlineUserName(OnlineUsersList::iterator name);
void RemoveOnlineUser(String name);
void UpdateOnlineUserState(String name, String state);
void UpdateOnlineUserSex(String name, unsigned int sex);
OnlineUsersList GetOnlineUsers();
//---------------------------------------------------------------------------
#endif
