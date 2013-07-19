//---------------------------------------------------------------------------

#ifndef virtual_usersH
#define virtual_usersH
#include <WideStrings.hpp>
#include "global.h"
#include <list>
//---------------------------------------------------------------------------

using namespace std;

// Виртуальный пользователь.
struct VirtualUser {
	VirtualUser(String _name) : name(_name) {};
	String name;
	StringList channels;
};

typedef list<VirtualUser> VirtualUsers;
extern VirtualUsers virtual_users;

// Канал со списком пользователей в нем.
struct Channel {
	Channel(String _name) : name(_name) {};
	String name;
	StringList users;
};

typedef list<Channel> Channels;
extern Channels channels;

extern VirtualUsers::iterator AddUser(String name);
extern VirtualUsers::iterator GetEmptyUser();
extern void DeleteUser(VirtualUsers::iterator user);
extern VirtualUsers::iterator SearchUser(String name);
bool IsUserExists(String name);
bool IsUserIteratorSet(VirtualUsers::iterator user);
bool IsUserInChannel(VirtualUsers::iterator user, String channel);
extern String GetFirstUserInChannel(String channel_name);
extern StringList GetUserChannels(String name);
extern void AddUserToChannel(String name, String channel);
extern void RemoveUserFromChannel(VirtualUsers::iterator user, String channel);
extern void RemoveUserFromChannel(String name, String channel);
extern int GetUserChannelsCount(VirtualUsers::iterator user);
extern String GetUserName(VirtualUsers::iterator user);
extern Channels::iterator SearchChannel(String name);
extern StringList GetUsersOfChannel(String name);
extern void DeleteChannel(String name);
extern void RemoveUserFromChannelsList(String name, String channel_name);
extern void AddUserToChannelsList(String name, String channel);
#endif
