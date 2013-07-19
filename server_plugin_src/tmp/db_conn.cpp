//---------------------------------------------------------------------------
#include <Forms.hpp>
#pragma hdrstop

#include "db_conn.h"
#include "global.h"
#include "cf_functions.h"
#include "notif.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// �������������� ���������� � ������ ��� �������� ��������.
// ���� ���������� ���� ������� - ��������� ���.
void db_conn_control::ConnectionInit(DBConnection &db_conn) {
	int sec = 1;
	db_conn.Con = mysql_init(NULL);
	if (db_conn.Con != NULL) {
		mysql_options(db_conn.Con, MYSQL_OPT_READ_TIMEOUT, &sec);
	} else {
		throw(Exception(e_sql_on_init));
	}
}

// ������������� ����������� ����� ���������� - ������������� � ���������.
void db_conn_control::SetConnOptions(DBConnection &db_conn) {
	mysql_set_server_option(db_conn.Con, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	mysql_set_character_set(db_conn.Con, "cp1251");
}

// ������������� ���������� � �� ��� ������ ������ � ���������� ���������
// ���� �� ������� ����������� ����������.
String db_conn_control::�onnect(DBConnection &db_conn, bool test) {
	String result = NULL;

	try {
		// ������ ��� �������������� ���������� ������, ����� �������� ������ �� ����� ����������.
		ConnectionInit(db_conn);

		// ���������� AnsiString(db_prop.name).c_str() ��-�� ����, ��� ���� ����� db_prop.name.t_str(),
		// �� t_str() ������������ ���������� � ��� ���������� �� ����������.

		// ������ ������ ������������ Con = mysql_real_connect(Con, ...), ��� ��� Con ���������� ��� ������
		// NULL � �� ���� ������ �������� ���������� ������.
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

// ���� ���������� ������� (�������� � �������� ������), �� ����� ����������
// ���� ������� ��������� � ��� � �������� ������ �� ���������� �����.
void db_conn_control::Disconnect(DBConnection &db_conn) {
	//is_connected = false;
	if (db_conn.Con != NULL) {
		mysql_close(db_conn.Con);
		db_conn.Con = NULL;
		if (db_conn.conn_destination == 0/* && main_form*/) {
			InfoAction(n_disconnect_success, false, 1, 0);
			//main_form->bd_connection_state->Caption = t_db_connection_no;
		}
		//set_user_state(bot_prop.name, "���-��������� �������");
	}
}

// ��������� ������ � ��, � ���������� ����� � �����. ���������.
// ���� ����� �� ��������� - ��������� ���������.
// ���������� NULL, ���� ������ ������ ������� ��� ����� ������, ���� �������� ������.
String db_conn_control::ExecuteQuery(DBConnection &db_conn, String query) {
	String error = NULL;
	if (db_conn.Con == NULL) {
		error = �onnect(db_conn, false);
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

// �������� ���������� ������� � ���������� ��������� ������ �� ������������ � ��������� ������.
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

// ���������� �������������� ������ �� ��������������.
String db_conn_control::GetEscaped(DBConnection &db_conn, String line) {
	String result_line = "";

	if (db_conn.Con == NULL) {
		String error = �onnect(db_conn, false);
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

// �������� ������ MySQL.
// ���������� ����������������� ������ � ������� ��� NULL.
String db_conn_control::GetMySQLError(DBConnection &db_conn) {
	String error = NULL;
	int err_no = int(mysql_errno(db_conn.Con));

	if (err_no != 0) {
		// ����������� �� ��, ���� ���������� ����������.
		if (err_no == 2006) {
			Disconnect(db_conn);
		}
		error = Format("%s (%d)", ARRAYOFCONST((mysql_error(db_conn.Con), err_no)));
	}

	return error;
}

// �������� ������, ������� ����� ���������� �� ����� ������� � �� � ���������� �� � ���.
// ���������� ����� ���� �� ������ ��� ���.
String db_conn_control::GetQueryError(DBConnection &db_conn) {
	String error = GetMySQLError(db_conn);

	if (error != NULL) {
		String stream_name = a_sql_streams[db_conn.conn_destination];
		error = Format(e_mysql_query, ARRAYOFCONST((stream_name, error)));
	}

	return error;
}
//---------------------------------------------------------------------------
