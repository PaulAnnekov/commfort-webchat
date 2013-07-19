//---------------------------------------------------------------------------
#pragma hdrstop

#include "virtual_users.h"
#include "connections.h"
#include "global.h"
#include "notif.h"
#include "l18n.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

/*
	auth_state:
	6 - пользователь вышел из чата
	7 - от соединения не поступал запрос на подключение виртуального пользователя.
	8 - ожидание ответа от сервера
	9 - уже в чате
	10 - превышено максимальное число подключенных пользователей
	11 - имя не соответствует требованиям (содержит служебные символы, либо превышает максимальную длину в 40 символов)
	12 - для данного пользователя доступ к серверу запрещен (бан)
	13 - имя содержит запрещенные слова (не проходит фильтр плохих слов)
	14 - схожее по написанию имя уже зарегистрировано
	15 - превышено максимальное число учетных записей с данного IP-адреса
	16 - заявка на активацию успешно отправлена
	17 - неверный пароль
	18 - заявка на активацию не обработана, либо отклонена
*/

VirtualUsers virtual_users;
Channels channels;

VirtualUsers::iterator AddUser(String name) {
	VirtualUsers::iterator user;

	try {
		EnterCriticalSection(&users_manipulation_cs);

		user = virtual_users.insert(virtual_users.end(), VirtualUser(name));
    } catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't add user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't add user", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return user;
};

// Возвращает указатель на несуществующего пользователя (итератор после конца).
VirtualUsers::iterator GetEmptyUser() {
	VirtualUsers::iterator user;

	try {
		EnterCriticalSection(&users_manipulation_cs);

		user = virtual_users.end();
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't delete user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't delete user", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return user;
}

void DeleteUser(VirtualUsers::iterator user) {
	try {
		EnterCriticalSection(&users_manipulation_cs);

		if (!user->channels.empty()) {
			StringList::iterator channel = NULL;
			for (channel = user->channels.begin(); channel != user->channels.end(); channel++) {
				RemoveUserFromChannelsList(user->name, *channel);
			}
		}

		virtual_users.erase(user);
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't delete user: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't delete user", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);
};

// Ищет пользователя по нику.
// Возвращает итератор.
VirtualUsers::iterator SearchUser(String name) {
	bool is_found = false;
	VirtualUsers::iterator user;

	try {
		EnterCriticalSection(&users_manipulation_cs);
		user = virtual_users.end();

		if (!virtual_users.empty()) {
			for (user = virtual_users.begin(); user != virtual_users.end(); user++) {
				if (user->name == name) {
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				user = virtual_users.end();
			}
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't search user by name: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't search user by name", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return user;
}

// Ищет пользователя по нику.
// Возвращает итератор.
bool IsUserExists(String name) {
	bool exists = false;
	VirtualUsers::iterator user = SearchUser(name);

	try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			exists = true;
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't check if user exists: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't check if user exists", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return exists;
}

// Ищет итератор в списке пользователей.
// Возвращает true, если найден.
bool IsUserIteratorSet(VirtualUsers::iterator user) {
	bool set = false;

	try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			set = true;
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't check is user iterator set: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't check is user iterator set", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return set;
}

bool IsUserInChannel(VirtualUsers::iterator user, String channel) {
	bool in_channel = false;

    try {
		EnterCriticalSection(&users_manipulation_cs);
		if (ListSearch(user->channels, channel) != NULL) {
			in_channel = true;
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't check is user in channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't check is user in channel", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return in_channel;
}

// Ищет первого пользователя, который находится в указанном канале.
// Возвращает его ник.
String GetFirstUserInChannel(String channel_name) {
	String user = "";

	Channels::iterator channel = SearchChannel(channel_name);

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		if (channel != channels.end()) {
			if (!channel->users.empty()) {
				user = channel->users.front();
			}
		}
    } catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't get first user in channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't get first user in channel", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);

	return user;
}

void AddUserToChannel(String name, String channel) {
	VirtualUsers::iterator user = SearchUser(name);

	try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			user->channels.push_back(channel);
			AddUserToChannelsList(name, channel);
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't add user to channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't add user to channel", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);
};

void RemoveUserFromChannel(VirtualUsers::iterator user, String channel) {
    try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			user->channels.erase(ListSearch(user->channels, channel));
			RemoveUserFromChannelsList(user->name, channel);
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't remove user from channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't remove user from channel", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);
};

void RemoveUserFromChannel(String name, String channel) {
	VirtualUsers::iterator user = SearchUser(name);

	RemoveUserFromChannel(user, channel);
};

// Ищет канал по названию.
// Возвращает итератор.
Channels::iterator SearchChannel(String name) {
	bool is_found = false;
	Channels::iterator channel;

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		channel = channels.end();

		if (!channels.empty()) {
			for (channel = channels.begin(); channel != channels.end(); channel++) {
				if (channel->name == name) {
					is_found = true;
					break;
				}
			}
			if (!is_found) {
				channel = channels.end();
			}
		}
	} catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't search channel by name: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't search channel by name", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);

	return channel;
}

void AddUserToChannelsList(String name, String channel_name) {
	Channels::iterator channel = SearchChannel(channel_name);

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		if (channel != channels.end()) {
			channel->users.push_back(name);
		}
	} catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't add user to channels list: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't add user to channels list", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);
}

void RemoveUserFromChannelsList(String name, String channel_name) {
	Channels::iterator channel = SearchChannel(channel_name);

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		if (channel != channels.end()) {
			StringList::iterator user = ListSearch(channel->users, name);
			if (user != NULL) {
				channel->users.erase(user);
			}
		}
	} catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't remove user from channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't remove user from channel", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);
}

String GetUserName(VirtualUsers::iterator user) {
	String name = "";

	try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			name = user->name;
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't get user name: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't get user name", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return name;
}

StringList GetUsersOfChannel(String name) {
	Channels::iterator channel = SearchChannel(name);
	StringList users;

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		if (channel != channels.end()) {
			users = channel->users;
		}
	} catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't get users of channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't get users of channel", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);

	return users;
}

StringList GetUserChannels(String name) {
	VirtualUsers::iterator user = SearchUser(name);
	StringList channels;

    try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			channels = user->channels;
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't get user channels: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't get user channels", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return channels;
}

int GetUserChannelsCount(VirtualUsers::iterator user) {
	int count = -1;

	try {
		EnterCriticalSection(&users_manipulation_cs);
		if (user != virtual_users.end()) {
			count = user->channels.size();
		}
	} catch(Exception *E) {
		InfoAction(__("UsersManipulationWarning") + ". Can't get channels count: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("UsersManipulationError") + ". Can't get channels count", false, 3, 2);
	}

	LeaveCriticalSection(&users_manipulation_cs);

	return count;
}

void DeleteChannel(String name) {
	Channels::iterator channel = SearchChannel(name);

	try {
		EnterCriticalSection(&channels_manipulation_cs);
		if (channel != channels.end()) {
			channels.erase(channel);
		}
	} catch(Exception *E) {
		InfoAction(__("ChannelsManipulationWarning") + ". Can't delete channel: \"" + E->Message + "\"", false, 3, 1);
	} catch (...) {
		InfoAction(__("ChannelsManipulationError") + ". Can't delete channel", false, 3, 2);
	}

	LeaveCriticalSection(&channels_manipulation_cs);
}
