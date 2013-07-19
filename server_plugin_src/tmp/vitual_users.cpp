//---------------------------------------------------------------------------


#pragma hdrstop

#include "vitual_users.h"
#include "port_listener.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

void AddUser(String name, String ip) {
	virtual_users.push_back(VirtualUser(name, ip);
};

// Ищет пользователя или нику.
// Возвращает итератор.
VirtualUsers::iterator SearchUser(String name) {
	VirtualUsers::iterator user = NULL;
	bool is_found = false;

	if (!logical_connections.empty()) {
		for (user = virtual_users.begin(); user != virtual_users.end(); user++) {
			if (user->name == name) {
				is_found = true;
				break;
			}
		}
		if (!is_found) {
			user = NULL;
		}
	}

	return user;
}

void SetUserAuthorizationStatus(String name, int status) {
	VirtualUsers::iterator user = SearchUser(name);

	if (user != NULL) {
		user->auth_status = status;
	}
};
