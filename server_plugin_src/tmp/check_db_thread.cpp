//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "check_db_thread.h"
#include "cf_functions.h"
#include "notif.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TGetDBUpdateThread::TGetDBUpdateThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
    String d_curr_date;
	DateTimeToString (d_curr_date, "yyyy.mm.dd", Date());
	d_file_pos = FileCreate(plugin_data_path + "Logs\\debug_" + d_curr_date + "_" + FormatDateTime("hh.nn.ss", Time()) + ".log", fmShareDenyNone, NULL);
	FreeOnTerminate = true;
	stop = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGetDBUpdateThread::Execute()
{
	int u_timer = 300, db_timer = 3600, error_count = 0;
	bool sucess;

	String dbg_msg;

	while (!stop) {
		try {
		u_timer++;
		db_timer++;
		sucess = true;
		if (u_timer >= 300) {
			// Проверяем пользователей, которые долго не пинговали БД.
			sucess = check_old_users();
			u_timer = 0;

			dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " check_old_users\r\n";
			FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());
		}
		if (auto_db_clear && db_timer >= 3600 && sucess) {
			// Проверяем таблицу действий на превышения лимита строк. Чистим при превышении.
			sucess = check_rows_limit();
			db_timer = 0;
		}
		if (sucess) {
			//проверяем новые сообщения
			if ((sucess = check_new_mess()) == true) {
				//проверяем новых пользователей
				if ((sucess = check_new_users()) == true) {
					//добавляем текущее время в БД
					sucess = ping();
                }
			}
		}

		if (sucess) { //если запрос успешен - обнулям счетчик подряд неудачных запросов
			error_count = 0;
			Sleep(1000);
		}
		else if (!sucess) {
			if (error_count >= 2) { //если произошло хотя бы 2 ошибки подряд - останавливаем запросы на 2 минуты
				error_count = 0;

				dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " error_2_errors_repeats\r\n";
				FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());

				InfoAction("Произошло 2 или более подряд ошибок во время синхронизации с БД. Новая попытка подключения будет выполнена через 2 минуты", false, 1, 1);
				Sleep(120000);
			}
			else {
				error_count++;
				Sleep(1000);
			}
		}
		} catch (Exception *E) {
			dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " catch_error: '" + E->Message + "'\r\n";
			FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());
		}
	}

	dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " stop == true\r\n";
	FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());



	stop = 0;
}
//---------------------------------------------------------------------------

void __fastcall TGetDBUpdateThread::GentleStop()
{
  /* Set the flag to 1 */
  String dbg_msg;

  dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " GentleStop\r\n";
  FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());
  InterlockedIncrement(&stop);

  /* Wait for this thread to finish executing */
  //WaitFor();
}
//---------------------------------------------------------------------------

bool TGetDBUpdateThread::check_new_mess()
{
	bool sucess;
	MYSQL_RES *result;
	String query = Format (
		"SELECT * FROM `%s%s` ORDER BY id ASC;" ,
		ARRAYOFCONST((tables_prefix, tbl_names.mess_to_send)));

	try {
		sucess = db_conn->query(query, 1, &result);
		int rows = mysql_num_rows(result);

		if (sucess && rows != 0) {
			String channel, body, name;
			int rezim;

			while ((row = mysql_fetch_row(result)) != NULL) {
				rezim = (int)row[5];
				channel = String(row[4]);

				//поиск ника отправителя в списке виртуальных пользователей
				if (web_users->is_user_exists(String(row[3]))) {
					name = String(row[3]);
					body = String(row[1]);
					if (!web_users->is_user_in_chnl(name, channel)) {
						virt_user_chnl_conn(name, channel);
						web_users->change_user_chnl(name, channel);
                    }
				}
				else {
					name = bot_prop.name;
					body = Format("[url=IP: %s]%s[/url]: %s",
						ARRAYOFCONST((
							String(row[6]),
							String(row[3]),
							String(row[1]))
						)
					);
                }

				cf_mess_send (name, rezim, channel, body);
			}

			sucess = db_conn->query(
				Format("TRUNCATE TABLE `%s%s`;",
					ARRAYOFCONST((tables_prefix, tbl_names.mess_to_send))),
				1
			);

			InfoAction("Добавлены новые сообщения с веба, кол-во: " + IntToStr(rows), false, 0, 0);
		}

		mysql_free_result(result);
	}
	catch(Exception *E) {
		InfoAction("Произошла ошибка во время проверки новых сообщений из веба. Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	return sucess;
}
//---------------------------------------------------------------------------

bool TGetDBUpdateThread::check_new_users()
{
	bool sucess;
	MYSQL_RES *result;
	String query = Format (
			"SELECT * FROM `%s%s` WHERE auth = 0 AND error = 0" ,
			ARRAYOFCONST((tables_prefix, tbl_names.web_users))
	);

	try {
		sucess = db_conn->query(query, 1, &result);
		int rows = mysql_num_rows(result);

		if (sucess && rows != 0) {
			while ((row = mysql_fetch_row(result)) != NULL) {
				/*if (user_registred(Result->Cells[1][i]))
				{  */
					UserConnect (
						String(row[1]),
						String(row[2]),
						String(row[3]),
						(row[4] == "1"),
						1
					);
				/*}*/
			}
			InfoAction("Обработаны новые заявки на авторизацию с веба, кол-во: " + IntToStr(rows), false, 0, 0);
		}
		mysql_free_result(result);
	}
	catch(Exception *E)
	{
		InfoAction("Произошла ошибка во время проверки новых пользователей из веба. Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	return sucess;
}
//---------------------------------------------------------------------------

bool TGetDBUpdateThread::check_old_users()
{
	bool sucess;
	MYSQL_RES *result;
	int timestamp = time(NULL) - 600;

	String query = Format (
			"SELECT nick FROM `%s%s` WHERE ping < '%d'" ,
			ARRAYOFCONST((tables_prefix, tbl_names.web_users, timestamp))
	);

	try {
		sucess = db_conn->query(query, 1, &result);
		int rows = mysql_num_rows(result);
		if (sucess && rows != 0) {
			while ((row = mysql_fetch_row(result)) != NULL) {
				virt_user_disconn(String(row[0]));
			}

			query = Format (
				"DELETE FROM `%s%s` WHERE ping < '%d'" ,
				ARRAYOFCONST((tables_prefix, tbl_names.web_users, timestamp))
			);

			sucess = db_conn->query(query, 1);
			InfoAction("Неактивные веб-пользователи отключены, кол-во: " + IntToStr(rows), false, 0, 0);
		}

		mysql_free_result(result);
	}
    catch(Exception *E)
	{
		InfoAction("Произошла ошибка во время проверки не активных пользователей из веба. Ошибка: \"" + E->Message + "\"", false, 0, 1);
	}

	return sucess;
}

bool TGetDBUpdateThread::ping() {
	bool sucess;
	int timestamp = time(NULL);

	String query = Format (
		"INSERT INTO `%s%s` (`name`, `value`) VALUES ('ping', '%d') ON DUPLICATE KEY UPDATE `value` = '%d'",
			ARRAYOFCONST((tables_prefix, tbl_names.settings, timestamp, timestamp))
	);

	sucess = db_conn->query(query, 1);

	return sucess;
}

bool TGetDBUpdateThread::check_rows_limit() {
	MYSQL_RES *result;
	MYSQL_ROW row;

	bool success = db_conn->query(
		Format("SELECT COUNT(*) FROM `%s%s`", ARRAYOFCONST((tables_prefix, tbl_names.actions))),
		1,
		&result
	);

	if (success) {
		row = mysql_fetch_row(result);
		int rows = atoi(row[0]);
		mysql_free_result(result);

		if (rows >= 100) {
			success = db_conn->query(
				Format("DELETE FROM `%s%s` ORDER BY id ASC LIMIT %d", ARRAYOFCONST((tables_prefix, tbl_names.actions, rows - 10))),
				1
			);
			if (success) {
				InfoAction("Таблица сообщений очищена. Удалено " + IntToStr(rows - 10) + " записей. Следующая проверка через час", false, 0, 0);
			} else {
				InfoAction("Во время попытки очистки таблицы сообщений произошла ошибка. Следующая проверка через час", false, 0, 1);
			}
		}
	}

	return success;
}

void TGetDBUpdateThread::terminate() {
	String dbg_msg;

	dbg_msg = FormatDateTime("hh:nn:ss", Time()) + " OnTerminate\r\n";
	FileWrite(d_file_pos, AnsiString(dbg_msg).c_str(), dbg_msg.Length());

	FileClose(d_file_pos);
}
