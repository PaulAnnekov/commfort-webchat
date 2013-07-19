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

#ifndef usersH
#define usersH

#include <list>
#include "global.h"
using namespace std;

struct User {
    User(){};
	User(String _name, String _computer_id, String _ip, bool _sex, bool _hidden_ip) : name(_name),
		computer_id(_computer_id), ip(_ip), sex(_sex), hidden_ip(_hidden_ip) { };

	String name, computer_id, ip;
	bool sex, hidden_ip;
};

typedef list<User> UsersList;
extern UsersList users;

void InitializeUsersList();
void DeleteUsersList();

void AddUser(String name, String computer_id, String ip, bool sex, bool hidden_ip);
void UpdateUser(UsersList::iterator user, String computer_id, String ip, bool sex);
UsersList::iterator FindUserByName(String name);
String GetUserName(UsersList::iterator user);
void SetUserSex(UsersList::iterator user, bool sex);
User GetUser(UsersList::iterator user);
//---------------------------------------------------------------------------
#endif
