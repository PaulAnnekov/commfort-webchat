//---------------------------------------------------------------------------

#pragma hdrstop
#include "web_users.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

USER::USER(String c_name) {
	name = c_name;
	chnl = NULL;
	/*for (int i = 0; i < 16; i++) {
		chnls[i] = NULL;
	} */
}
//---------------------------------------------------------------------------

WebUsers::WebUsers() {
// Инициализируем список.

}
//---------------------------------------------------------------------------

WebUsers::~WebUsers(void) {

}
//---------------------------------------------------------------------------

bool WebUsers::is_list_empty() {
// Проверяем список на пустоту.
	return true;
}
//---------------------------------------------------------------------------

void WebUsers::add_user(String name) {
	users.push_back(USER(name));
}
//---------------------------------------------------------------------------

vector<USER>::iterator WebUsers::search_user(String name) {
	vector<USER>::iterator res = NULL;
	bool is_found = false;
	if (!users.empty()) {
		for (res = users.begin(); res != users.end(); res++) {
			if (res->name == name) {
				is_found = true;
				break;
			}
		}
		if (!is_found) {
			res = NULL;
		}
	}

	return res;
}
//---------------------------------------------------------------------------

void WebUsers::change_user_chnl(String name, String chnl) {
	vector<USER>::iterator user = search_user(name);
	user->chnl = chnl;
}

//---------------------------------------------------------------------------

/*bool WebUsers::add_user_chnl(String name, String chnl) {
	vector<USER>::iterator user = search_user(name);
	bool result = false;

	if (user != NULL) {
		int i = 0;
		while (i < 16) {
			if (user->chnls[i] == NULL) {
				break;
			}
			i++;
		}
		if (i != 16) {
			user->chnls[i] = chnl;
			result = true;
		}
	}

	return result;
}   */
//---------------------------------------------------------------------------

void WebUsers::del_user(String name) {
	vector<USER>::iterator del = search_user(name);
	if (del != NULL) {
		users.erase(search_user(name));
	}
}
//---------------------------------------------------------------------------

/*bool WebUsers::del_user_chnl(String name, String chnl) {
	vector<USER>::iterator user = search_user(name);
	bool result = false;

	if (user != NULL) {
		int i = 0;
		while (i < 16) {
			if (user->chnls[i] == chnl) {
				user->chnls[i] = NULL;
				result = true;
				break;
			}
			i++;
		}
	}

	return result;
}  */
//---------------------------------------------------------------------------

bool WebUsers::is_user_in_chnl(String name, String chnl) {
	vector<USER>::iterator user = search_user(name);
	bool result = false;

	if (user != NULL && user->chnl == chnl) {
		result = true;
		/*int i = 0;
		while (i < 16) {
			if (user->chnls[i] == chnl) {
				result = true;
				break;
			}
			i++;
		} */
	}

	return result;
}
//---------------------------------------------------------------------------

bool WebUsers::is_user_exists(String name) {
	bool result = false;
	if (search_user(name) != NULL) {
		result = true;
	}

	return result;
}
//---------------------------------------------------------------------------

void WebUsers::clear() {
    users.clear();
}
//---------------------------------------------------------------------------

