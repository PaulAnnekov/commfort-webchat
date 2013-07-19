//---------------------------------------------------------------------------

#ifndef db_utilsH
#define db_utilsH

#include "db_conn.h"
#include "global.h"
#include "plug_init.h"
//---------------------------------------------------------------------------

//extern bool IsUserRegistred(DBConnection &db_connection, String name);
extern String GetChannelUsersListQuery(StringList users_list, String channel);
extern String UpdateChannelInfoQuery(String channel, String topic, String greeting);
extern String InsertChannelQuery(CFChannelShortInfo channel);
// Возвращает текст запроса для создания базовой структуры БД.
extern bool SetBaseDB();
#endif
