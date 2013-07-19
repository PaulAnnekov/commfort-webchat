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

#ifndef channelsH
#define channelsH

#include <list>
#include "global.h"
using namespace std;

struct Channel {
	Channel(String _name, unsigned int _online_users, String _topic) :
	name(_name), online_users(_online_users), topic(_topic), visibility(1) {};
	String name, topic, topic_editor, greeting;
	unsigned long topic_date, images_type, online_users;
	bool topic_edit, visibility, access;
};

typedef list<Channel> ChannelsList;
extern ChannelsList channels;

void InitializeChannelsList();
void DeleteChannelsList();

void AddChannel(String name, unsigned int online_users, String topic);
void UpdateChannel(ChannelsList::iterator channel, Channel channel_info);
ChannelsList::iterator FindChannelByName(String name);
ChannelsList GetChannels();
void UpdateChannelTopic(String channel_name, String topic, String topic_editor, unsigned int topic_date);
//---------------------------------------------------------------------------
#endif
