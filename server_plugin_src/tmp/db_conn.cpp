//---------------------------------------------------------------------------
#include <Forms.hpp>
#pragma hdrstop

#include "db_conn.h"
#include "global.h"
#include "cf_functions.h"
#include "notif.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Инициализируем соединение и задаем ему параметр таймаута.
// Если соединение было открыто - закрываем его.
void db_conn_control::ConnectionInit(DBConnection &db_conn) {
	int sec = 1;
	db_conn.Con = mysql_init(NULL);
	if (db_conn.Con != NULL) {
		mysql_options(db_conn.Con, MYSQL_OPT_READ_TIMEOUT, &sec);
	} else {
		throw(Exception(e_sql_on_init));
	}
}

// Устанавливаем стандартные опции соединению - мультизапросы и кодировку.
void db_conn_control::SetConnOptions(DBConnection &db_conn) {
	mysql_set_server_option(db_conn.Con, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	mysql_set_character_set(db_conn.Con, "cp1251");
}

// Устанавливает соединение с БД или выдает ошибку и возвращает результат
// было ли успешно установлено соединение.
String db_conn_control::Сonnect(DBConnection &db_conn, bool test) {
	String result = NULL;

	try {
		// Каждый раз инициализируем соединение заново, чтобы избежать ошибок во время соединения.
		ConnectionInit(db_conn);

		// Используем AnsiString(db_prop.name).c_str() из-за того, что если юзать db_prop.name.t_str(),
		// то t_str() модифицирует переменную и она становится не читабельна.

		// Нельзя делать присваивание Con = mysql_real_connect(Con, ...), так как Con становится при ошибке
		// NULL и из него нельзя вытянуть содержимое ошибки.
		if (!mysql_real_connect(db_conn.Con, AnsiString(db_prop.host).c_str(), AnsiString(db_prop.user).c_str(), AnsiString(db_prop.pass).c_str(), AnsiString(db_prop.name).c_str(), 0, NULL, NULL)) {
			throw(Exception(GetMySQLError(db_conn)));
		} else {
			SetConnOptions(db_conn);

			result = ExecuteQuery(db_conn, "SET group_concat_max_len = 6000");

			if (result != NULL) {
				throw(Exception("Can't set 'group_concat_max_len' variable value: \"" + result + "\""));
			}

			if (db_conn.conn_destination == 0/* && main_form*/) {
				InfoAction(n_connect_success, false, 1, 0);
				//main_form->bd_connection_state->Caption = t_db_connection_yes;
			};
		}
	} catch (Exception *E) {
		Disconnect(db_conn);
		if (result == NULL) {
			result = Format(e_cant_connect, ARRAYOFCONST((db_prop.host, E->Message)));
		}
		if (test) {
			InfoAction(result, true, 1, 1);
		}
	}

	return result;
}

// Если соединение главное (запущено в основном потоке), то после отключения
// надо вывести сообщение в лог и изменить данные на визуальной форме.
void db_conn_control::Disconnect(DBConnection &db_conn) {
	//is_connected = false;
	if (db_conn.Con != NULL) {
		mysql_close(db_conn.Con);
		db_conn.Con = NULL;
		if (db_conn.conn_destination == 0/* && main_form*/) {
			InfoAction(n_disconnect_success, false, 1, 0);
			//main_form->bd_connection_state->Caption = t_db_connection_no;
		}
		//set_user_state(bot_prop.name, "Веб-интерфейс оффлайн");
	}
}

// Выполняет запрос к БД, и записывает ответ в соотв. структуру.
// Если ответ не требуется - структура очищается.
// Возвращает NULL, если запрос прошел успешно или текст ошибки, если возникла ошибка.
String db_conn_control::ExecuteQuery(DBConnection &db_conn, String query) {
	String error = NULL;
	if (db_conn.Con == NULL) {
		error = Сonnect(db_conn, false);
	}
	if (error == NULL) {
		db_conn.last_query = query;
		mysql_real_query(db_conn.Con, AnsiString(query).c_str(), query.Length());
		error = GetQueryError(db_conn);
		if (error == NULL) {
			if (!db_conn.is_result_needed) {
				db_conn.Res = mysql_store_result(db_conn.Con);
				while (mysql_next_result(db_conn.Con) == 0);
				mysql_free_result(db_conn.Res);

				error = GetQueryError(db_conn);
			}
		}
	}

	return error;
}

// Вызывает выполнение запроса и записывает структуру ответа по передаваемой в параметре ссылке.
String db_conn_control::ExecuteQuery(DBConnection &db_conn, String query, MYSQL_RES **result) {
	String error = NULL;

	db_conn.is_result_needed = true;
	error = ExecuteQuery(db_conn, query);
	if (error == NULL) {
		*result = mysql_store_result(db_conn.Con);
		error = GetQueryError(db_conn);
	}
	db_conn.is_result_needed = false;

	return error;
}

// Возвращает экранированную строку из неэкранируемой.
String db_conn_control::GetEscaped(DBConnection &db_conn, String line) {
	String result_line = "";

	if (db_conn.Con == NULL) {
		String error = Сonnect(db_conn, false);
		if (error != NULL) {
			throw(error);
		}
	}

	if (line != "") {
		char *escaped_line = new char[line.Length() * 2 + 1];

		try {
			mysql_real_escape_string(db_conn.Con, escaped_line, AnsiString(line).c_str(), line.Length());
			result_line = String(escaped_line);
		}
		__finally {
			delete []escaped_line;
		}
	}

	return result_line;
}

// Получает ошибки MySQL.
// Возвращает неформатированную строку с ошибкой или NULL.
String db_conn_control::GetMySQLError(DBConnection &db_conn) {
	String error = NULL;
	int err_no = int(mysql_errno(db_conn.Con));

	if (err_no != 0) {
		// Отключаемся от БД, если соединение оборвалось.
		if (err_no == 2006) {
			Disconnect(db_conn);
		}
		error = Format("%s (%d)", ARRAYOFCONST((mysql_error(db_conn.Con), err_no)));
	}

	return error;
}

// Получает ошибки, которые могли возникнуть во время запроса к БД и записывает их в лог.
// Возвращает ответ была ли ошибка или нет.
String db_conn_control::GetQueryError(DBConnection &db_conn) {
	String error = GetMySQLError(db_conn);

	if (error != NULL) {
		String stream_name = a_sql_streams[db_conn.conn_destination];
		error = Format(e_mysql_query, ARRAYOFCONST((stream_name, error)));
	}

	return error;
}
//---------------------------------------------------------------------------
