//---------------------------------------------------------------------------


#pragma hdrstop

#include "db_utils.h"
#include "json_main.h"
#include "cf_functions.h"
#include "notif.h"
#include "l18n.h"
#include "webchat.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace db_conn_control;

//bool IsUserRegistred(DBConnection &db_connection, String name) {
///*
//	 1 - registred
//	 0 - not registred
//*/
//	MYSQL_RES *res;
//
//	name = GetEscaped(db_connection, name);
//
//	String query = Format (
//		"SELECT COUNT(*) FROM `%s%s` WHERE name = '%s'",
//		ARRAYOFCONST((tables_prefix, tbl_names.users, name))
//	);
//
//	String error = ExecuteQuery(db_connection, query, &res);
//	if (error != NULL) {
//		throw(JSONError(-1000, e_json_db_query));
//	}
//
//	MYSQL_ROW row;
//
//	row = mysql_fetch_row(res);
//
//	bool is_exists = (!strcmp(row[0], "0")) ? false : true;
//
//	return is_exists;
//}

// Get channel users list DB query.
String GetChannelUsersListQuery(StringList users_list, String channel) {
	String query = Format(
		"DELETE FROM `%s%s` WHERE `channel` = \"%s\";",
		ARRAYOFCONST((tables_prefix, tbl_names.user_channel, channel))
	);

	if (!users_list.empty()) {
		query = Format(
			"INSERT INTO `%s%s` (`user`, `channel`) VALUES ",
			ARRAYOFCONST((tables_prefix, tbl_names.user_channel))
		);

		StringList::iterator user = NULL;
		StringList::iterator user_end = users_list.end();
		user_end--;
		for (user = users_list.begin(); user != users_list.end(); user++) {
			query += Format(
				"(\"%s\", \"%s\")",
				ARRAYOFCONST((*user, channel))
			);
			if (user != user_end) {
				query += ", ";
			}
		}
		query += ";";
	}

	return query;
}

// Query for channel info update.
//String UpdateChannelInfoQuery(String channel, String topic, String greeting) {
//	String name, topic_editor, topic_edit_time;
//	ChannelInfo channel_info = GetChannelInfo(channel, true, true);
//
//	name = GetEscaped(event_db_conn, channel);
//	topic = (topic.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, topic) + "\"";
//	greeting = (greeting.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, greeting) + "\"";
//	topic_editor = (channel_info.topic_editor.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, channel_info.topic_editor) + "\"";
//	topic_edit_time = (channel_info.topic_edit_time != 0) ? String(channel_info.topic_edit_time) : String("NULL");
//
//	String query = Format(
//		"UPDATE `%s%s` SET `topic` = %s, `last_topic_editor` = %s, "
//			"`topic_edit_time` = %s, `topic_edit_rights` = %d, `greeting` = %s, `image_type` = %d, "
//			"`visibility` = %d, `access` = %d WHERE `name` = \"%s\";",
//		ARRAYOFCONST((tables_prefix, tbl_names.channels, topic, topic_editor, topic_edit_time,
//		channel_info.topic_edit_rights, greeting, channel_info.image_type, channel_info.visibility,
//		channel_info.access, name))
//	);
//
//	return query;
//}

// Query for channel insert.
//String InsertChannelQuery(ChannelShortInfo channel) {
//	String name, topic, greeting, topic_editor, topic_edit_time;
//	ChannelInfo channel_info = GetChannelInfo(channel.name);
//
//	name = GetEscaped(event_db_conn, channel.name);
//	topic = (channel.topic.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, channel.topic) + "\"";
//	greeting = (channel_info.greeting.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, channel_info.greeting) + "\"";
//	topic_editor = (channel_info.topic_editor.IsEmpty()) ? String("NULL") : "\"" + GetEscaped(event_db_conn, channel_info.topic_editor) + "\"";
//	topic_edit_time = (channel_info.topic_edit_time != 0) ? String(channel_info.topic_edit_time) : String("NULL");
//
//	String query = Format(
//		"(NULL, \"%s\", %s, %s, %s, %d, %s, %d, %d, %d, %d)",
//		ARRAYOFCONST((name, topic, topic_editor, topic_edit_time, channel_info.topic_edit_rights, greeting,
//		channel_info.image_type, channel_info.visibility, channel_info.access, channel.users_online))
//	);
//
//	return query;
//}

// Set default DB structure.
bool SetBaseDB() {
	bool success = true;
	TStringList *mysql_dump = new TStringList();
	try {
		mysql_dump->LoadFromFile(plugin_data_path + "db_predump.sql");
		String query = StringReplace(mysql_dump->Text, "%s", tables_prefix, TReplaceFlags() << rfReplaceAll);

		String error = ExecuteQuery(event_db_conn, query);

		if (error != NULL) {
			InfoAction(Format(__("DBDumpExecuteError"), ARRAYOFCONST((error))), true, 1, 1);
			success = false;
		}
	} catch (Exception *E) {
		success = false;
		InfoAction(Format(__("DBDumpLoadError"), ARRAYOFCONST((E->Message))), true, 0, 2);
	}

	delete mysql_dump;

	return success;
}