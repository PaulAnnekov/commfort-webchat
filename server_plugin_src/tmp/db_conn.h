//---------------------------------------------------------------------------

#ifndef db_connH
#define db_connH

#include <windows.h>
#define __LCC__
#include <mysql.h>

#include <WideStrings.hpp>
//---------------------------------------------------------------------------

// Структура описывающая одно соединение с БД.
struct DBConnection {
	DBConnection(int _conn_destination) : conn_destination(_conn_destination) {
		Con = NULL;
		is_result_needed = false;
	}
	MYSQL *Con;
	MYSQL_RES *Res;
	bool is_result_needed;
	// Назначение соединения:
	// 0 - Запись событий в БД. Соединение основного потока сервера.
	// 1 - Поток прослушки чистого сокета.
	// 2 - Поток прослушки HTTP запросов.
	int conn_destination;
	String last_query;
};

// Набор функций для управления соединением с БД.
namespace db_conn_control {
	void ConnectionInit(DBConnection &db_conn);
	void SetConnOptions(DBConnection &db_conn);
	String Сonnect(DBConnection &db_conn, bool test);
	void Disconnect(DBConnection &db_conn);
	String ExecuteQuery(DBConnection &db_conn, String query);
	String ExecuteQuery(DBConnection &db_conn, String query, MYSQL_RES **result);
	String GetEscaped(DBConnection &db_conn, String line);
    String GetMySQLError(DBConnection &db_conn);
	String GetQueryError(DBConnection &db_conn);
};

#endif
