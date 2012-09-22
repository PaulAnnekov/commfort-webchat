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

#include "online_users.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION online_users_list_cs;
OnlineUsersList online_users;

void InitializeOnlineUsersList() {
	InitializeCriticalSection(&online_users_list_cs);
}

void DeleteOnlineUsersList() {
	DeleteCriticalSection(&online_users_list_cs);
	online_users.clear();
}

OnlineUsersList::iterator AddOnlineUser(UsersList::iterator user_ref, String state, String client_version, String process, bool active, int delay) {
	OnlineUsersList::iterator user;

	try {
		EnterCriticalSection(&online_users_list_cs);

		user = online_users.insert(online_users.end(), OnlineUser(user_ref, state, client_version, process, active, delay));
	} catch (Exception *E) {
		InfoAction("(2000) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2000) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return user;
}

OnlineUsersList::iterator FindOnlineUserByName(String name) {
	OnlineUsersList::iterator result;

	try {
		EnterCriticalSection(&online_users_list_cs);

		result = online_users.end();
		if (!online_users.empty()) {
        	OnlineUsersList::iterator user;
			for (user = online_users.begin(); user != online_users.end(); user++) {
				if (GetUserName(user->user_ref) == name) {
					result = user;
					break;
				}
			}
		}
	} catch (Exception *E) {
		InfoAction("(2001) " + __("OnlineUsersManipulationError") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2001) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return result;
}

OnlineUsersList::iterator GetEmptyOnlineUser() {
	OnlineUsersList::iterator user;

	try {
		EnterCriticalSection(&online_users_list_cs);

		user = online_users.end();
	} catch (Exception *E) {
		InfoAction("(2002) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2002) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return user;
}

UsersList::iterator GetOnlineUserReference(OnlineUsersList::iterator online_user) {
	UsersList::iterator user;

	try {
		EnterCriticalSection(&online_users_list_cs);

		if (online_user != online_users.end()) {
			user = online_user->user_ref;
        }
	} catch (Exception *E) {
		InfoAction("(2003) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2003) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return user;
}

String GetOnlineUserName(OnlineUsersList::iterator user) {
	String name = "";

	try {
		EnterCriticalSection(&online_users_list_cs);

		if (user != online_users.end()) {
			name = GetUserName(user->user_ref);
		}
	} catch (Exception *E) {
		InfoAction("(2004) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2004) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return name;
}

void RemoveOnlineUser(String name) {
	try {
		EnterCriticalSection(&online_users_list_cs);

		OnlineUsersList::iterator user = FindOnlineUserByName(name);
		if (user != online_users.end()) {
			online_users.erase(user);
		}
	} catch (Exception *E) {
		InfoAction("(2005) " + __("OnlineUsersManipulationError") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2005) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);
}

void UpdateOnlineUserState(String name, String state) {
	try {
		EnterCriticalSection(&online_users_list_cs);

		OnlineUsersList::iterator user = FindOnlineUserByName(name);
		if (user != online_users.end()) {
        	user->state = state;
		}
	} catch (Exception *E) {
		InfoAction("(2006) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2006) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);
}

void UpdateOnlineUserSex(String name, unsigned int sex) {
	try {
		EnterCriticalSection(&online_users_list_cs);

		OnlineUsersList::iterator user = FindOnlineUserByName(name);
		if (user != online_users.end()) {
			SetUserSex(user->user_ref, sex);
		}
	} catch (Exception *E) {
		InfoAction("(2007) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2007) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);
}

bool IsOnlineUserSet(OnlineUsersList::iterator user) {
	bool is_set = false;

	try {
		EnterCriticalSection(&online_users_list_cs);

		if (user != online_users.end()) {
			is_set = true;
		}
	} catch (Exception *E) {
		InfoAction("(2008) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2008) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return is_set;
}

OnlineUsersList GetOnlineUsers() {
	OnlineUsersList online_users_copy;

	try {
		EnterCriticalSection(&online_users_list_cs);

		online_users_copy = online_users;
	} catch (Exception *E) {
		InfoAction("(2009) " + __("OnlineUsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(2009) " + __("OnlineUsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&online_users_list_cs);

	return online_users_copy;
}
