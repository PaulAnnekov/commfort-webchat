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

#include "users.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CRITICAL_SECTION users_list_cs;
UsersList users;

void InitializeUsersList() {
	InitializeCriticalSection(&users_list_cs);
}

void DeleteUsersList() {
	DeleteCriticalSection(&users_list_cs);
	users.clear();
}

void AddUser(String name, String computer_id, String ip, bool sex, bool hidden_ip) {
    try {
		EnterCriticalSection(&users_list_cs);

		users.push_back(User(name, computer_id, ip, sex, hidden_ip));
	} catch (Exception *E) {
		InfoAction("(1000) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1000) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);
}

void UpdateUser(UsersList::iterator user, String computer_id, String ip, bool sex) {
    try {
		EnterCriticalSection(&users_list_cs);

		if (user != users.end()) {
        	user->computer_id = computer_id;
			user->ip = ip;
			user->sex = sex;
		}
	} catch (Exception *E) {
		InfoAction("(1001) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1001) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);
}

UsersList::iterator FindUserByName(String name) {
	UsersList::iterator result;

	try {
		EnterCriticalSection(&users_list_cs);

		UsersList::iterator user;
		result = users.end();
		for (user = users.begin(); user != users.end(); user++) {
			if (user->name == name) {
				result = user;
				break;
			}
		}
	} catch (Exception *E) {
		InfoAction("(1002) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1002) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);

	return result;
}

String GetUserName(UsersList::iterator user) {
	String name;

	try {
		EnterCriticalSection(&users_list_cs);

		name = user->name;
	} catch (Exception *E) {
		InfoAction("(1003) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1003) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);

	return name;
}

void SetUserSex(UsersList::iterator user, bool sex) {
	try {
		EnterCriticalSection(&users_list_cs);

		user->sex = sex;
	} catch (Exception *E) {
		InfoAction("(1004) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1004) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);
}

User GetUser(UsersList::iterator user) {
	User result_user;

	try {
		EnterCriticalSection(&users_list_cs);

		result_user = *user;
	} catch (Exception *E) {
		InfoAction("(1005) " + __("UsersManipulationWarning") + ": \"" + E->Message + "\"", false, 1, 1);
	} catch (...) {
		InfoAction("(1005) " + __("UsersManipulationError"), false, 1, 2);
	}

	LeaveCriticalSection(&users_list_cs);

	return result_user;
}
