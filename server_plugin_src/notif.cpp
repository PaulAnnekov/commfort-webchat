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
#include "notif.h"
#include "global.h"
#include "log_form.h"
#include "l18n.h"
#include <StrUtils.hpp>

#pragma package(smart_init)
//---------------------------------------------------------------------------

const String
	e_sql_on_init = "Не удалось инициализировать соединение. Недостаточно памяти",
	e_cant_connect = "Не могу подключиться к БД на хост \"%s\". Ошибка: \"%s\"",
	e_sql_syntax = "%s. Возникла ошибка в синтаксисе запроса. Запрос: \"%s\". Ошибка: \"%s\"",
	e_mysql_query = "%s. Не удалось выполнить запрос к БД. Ошибка: \"%s\"",
	e_db_base_create = "Возникла ошибка при создании таблиц БД. Ошибка: \"%s\".\n"
					   "Возможно не хватает прав пользователя. Пожалуйста проверьте настройки БД и плагина",

	e_plug_start = "Произошла ошибка во время старта плагина. Ошибка: \"%s\"",
	e_plug_stop = "Произошла ошибка во время отключения плагина. Ошибка: \"%s\"",

	e_event_error                  = "Произошла ошибка при обработке события",
	e_user_mess_recieve            = e_event_error + " получения опубликованного сообщения. Ошибка: \"%s\"",
	e_user_channel_connect         = e_event_error + " входа пользователя в канал. Ошибка: \"%s\"",
	e_virtual_user_channel_connect = e_event_error + " входа виртуального пользователя в канал. Ошибка: \"%s\"",
	e_user_connect                 = e_event_error + " входа пользователя в чат. Ошибка: \"%s\"",
	e_user_channel_disconnect      = e_event_error + " выхода пользователя из канала. Ошибка: \"%s\"",
	e_user_disconnect              = e_event_error + " выхода пользователя из чата. Ошибка: \"%s\"",
	e_user_state_changed           = e_event_error + " изменения статуса пользователя. Ошибка: \"%s\"",
	e_user_sex_changed             = e_event_error + " изменения иконки пользователя. Ошибка: \"%s\"",
	e_chnl_theme_changed           = e_event_error + " изменения темы канала. Ошибка: \"%s\"",
	e_unsuccess_auth               = e_event_error + " неудачной авторизации пользователя. Ошибка: \"%s\"",

	e_bot_auth[9] = {
		"Превышено максимальное число подключенных пользователей. Надо освободить место для бота",
		"Имя бота не соответствует требованиям (содержит служебные символы, либо превышает максимальную длину в 40 символов). Измените имя бота в настройках плагина",
		"Для бота доступ к серверу запрещен (бан). Снимите бан",
		"Имя бота содержит запрещенные слова (не проходит фильтр плохих слов). Измените имя бота в настройках плагина",
		"Схожее по написанию имя уже зарегистрировано. Измените имя бота в настройках плагина",
		"Превышено максимальное число учетных записей с данного IP-адреса. Внесите IP в исключения в настройках сервера или измените IP в настройках плагина",
		"Бот стоит в очереди на обработку заявки на активацию. Активируйте учётную запись бота",
		"Неверный пароль",
		"Заявка на активацию бота не обработана, либо отклонена. Проверьте состояние заявки"
	},
	e_bot_base_load = "Ошибка во время загрузки базовых данных после подключения бота",

	e_tcp_addr = "Произошла ошибка во время активации функций подключения сторонних клиентов. IP-адрес сервера не верный",
	e_tcp_listen= "Невозможно подключиться к прослушке порта. Ошибка: \"%s\"",
	e_tcp_connection_create = "Невозможно создать принимающее соединение. Ошибка: \"%s\"",
	e_tcp_write = "Ошибка записи в сокет клиента. Ошибка: \"%s\"",
	e_tcp_read = "Ошибка чтения из сокета клиента. Ошибка: \"%s\"",
	e_tcp_during_listen = "Ошибка сервера во время прослушки входящих соединений. Ошибка: \"%s\"",
	e_tcp_socket_accept = "Невозможно принять входящее подключение. Ошибка: \"%s\"",
	e_tcp_socket_close = "Проблемы при закрытии соединения. Ошибка: \"%s\"",
	e_tcp_connections_check = "Невозможно запустить проверку входящих соединений. Ошибка: \"%s\"",
	e_tcp_client_during_listen = "Ошибка клиента во время прослушки входящих соединений. Ошибка: \"%s\"",
	e_tcp_client_during_read = "Ошибка клиента во время чтения запроса. Ошибка: \"%s\"",

	e_notif_db = "Ошибка при выполнении запроса на добавление события для HTTP клиентов. Ошибка: \"%s\"",

	e_image_unique = "Ошибка при подборе уникального имени для изображения, полученного в сообщении",
	e_image_open = "Ошибка при открытии файла изображения на запись. Возможно недостаточно прав",
	e_image_write = "Ошибка при записи в файл изображения. Возможно недостаточно прав",
	e_image_close = "Ошибка при закрытии файла изображения",
	e_image_unexpected = "Неожиданная ошибка при сохранении изображения",

	n_connect_success = "Соединение с БД установлено",
	n_disconnect_success = "Соединение с БД разорвано",

	n_user_message_recieved = "Сообщение от пользователя %s добавлено: \"%s\"",
	n_user_channel_connect = "В канал %s зашёл сторонний пользователь: %s / %s",
	n_virtual_user_channel_connect = "В канал %s зашёл виртуальный пользователь: %s",
	n_user_connect = "В чат вошёл пользователь: %s",
	n_user_channel_disconnect = "Из канала %s вышел пользователь: %s",
	n_user_disconnect = "Из чата вышел пользователь: %s",
	n_user_state_changed = "Пользователь %s изменил статус на \"%s\"",
	n_user_sex_changed = "Пользователь %s изменил иконку на индекс: %d",
	n_channel_theme_changed = "Тема канала \"%s\" изменена на: \"%s\" пользователем \"%s\"",

    n_plugin_sync_started = "Запуск плагина успешно завершен",
	n_users_list_updated = "Список пользователей успешно обновлен",

	a_sql_streams[3] = {"Поток событий", "Поток http клиентов", "Поток tcp клиентов"},

	// Заголовки для диалоговых окон функции MessageBox.
	sm_warning = "Предупреждение",
	sm_error = "Ошибка",
	sm_question = "Принятие решения",
	sm_asterisk = "Информация",

	// Текст визуальных элементов.
	t_show_connection_button = "Активные соединения",
	t_db_connection_yes = "Подключено",
	t_db_connection_no = "Отсутствует",
	t_plug_off = "Выключить плагин",
	t_plug_on = "Включить плагин",
	t_plug_started = "Подключен",
	t_plug_offline = "Отключен",
	t_connections_off = "Отсутствуют",
	t_online_users_unknown = "Неизвестно",
	t_bot_auth_wait = "Авторизация бота...",

	// Префиксный текст для логируемой строки.
	l_group_prefix[4] = {"Общие", "БД", "События", "Соединения"};

String messages[3]    = {"notice", "warn", "error"}, //Массив сообщений определяющих состояние события в файлах логов
	   last_init_date = NULL;                        //Последняя дата инициализации (используется для сравнения с текущей датой в файле логов и с текущей датой при очистке БД)
int file_pos = NULL;            			 	     //handle для файла логов

ofstream log_file;

void LofFileFlush() {
    // Write all buffered data to assosiated file.
	/* TODO : Предусмотреть потокобезопасность действия */
	log_file.flush();
}

// Загрузка лога из файла в Memo поле.
void LoadLog() {
	TStringList *log_file_list = NULL;
	TFileStream *temp = NULL;

	log_file_list = new TStringList();

	LofFileFlush();

	// Напрямую в log_file не хочет грузить, говорит что файл занят.
	temp = new TFileStream(plugin_data_path + "Logs\\" + last_init_date + ".log", fmOpenRead | fmShareDenyNone);
	try {
		log_file_list->LoadFromStream(temp);

		int pos, int_type, l_count;
		String time, group, line, type;
		l_count = log_file_list->Count;
		int i = (l_count > 200) ? l_count - 201 : 0;

		for (i; i < l_count; i++) {
			line = log_file_list->Strings[i].Trim();

			pos = line.Pos("]");
			time = line.SubString(0, pos);
			line.Delete(1, pos + 1);

			pos = line.Pos("]");
			type = line.SubString(2, pos - 2);
			line.Delete(1, pos + 1);

			if (type == "notice") {
				int_type = 0;
			}
			else if (type == "warn") {
				int_type = 1;
			}
			else if (type == "error") {
				int_type = 2;
			}

			pos = line.Pos("]");
			group = line.SubString(2, pos - 2);
			line.Delete(1, pos + 1);

			RTFInsert(Format("%s. %s\r\n", ARRAYOFCONST((group, line))), int_type);
			RTFInsert(Format("%s ", ARRAYOFCONST((time))), 0);
		}
	}
	__finally {
		if (log_file_list) {
			delete log_file_list;
		}
		if (temp) {
			delete temp;
		}
	}
}

// Показ сообщения.
int ShowMsg(HWND handle, String main_t, int type) {
// type - как и в InfoAction + 3 - вопрос
	UINT icon_type, btns_type;
	String caption_t = "";
	btns_type = MB_OK;

	switch (type) {
		case 1:
			icon_type = MB_ICONWARNING;
			caption_t = sm_warning;
			break;
		case 2:
			icon_type = MB_ICONERROR;
			caption_t = sm_error;
			break;
		case 3:
			icon_type = MB_ICONQUESTION;
			caption_t = sm_question;
			btns_type = MB_YESNO;
			break;
		default:
			icon_type = MB_ICONASTERISK;
			caption_t = sm_asterisk;
			break;
	}

	int id = MessageBox(handle, AnsiString(main_t).c_str(), AnsiString(caption_t).c_str(), btns_type + icon_type + MB_TASKMODAL);

	return id;
}

// Вывод сообщения в Memo поле.
void RTFInsert(String text, int type) {
	HWND__ *h_Edit = log_form->states_list_rich->Handle;

	// Устанавливаем позицию вставки текста. cpMin = cpMax - значит это единичная точка, а не границы.
	CHARRANGE cr;
	cr.cpMin = 0;
	cr.cpMax = 0;
	SendMessage(h_Edit, EM_EXSETSEL, 0, (LPARAM)&cr);

	// Устанавливаем цвет из массива цветов.
	CHARFORMAT cf;
	cf.cbSize = sizeof cf;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = color[type];

	// Применяем выбранные параметры форматирования ко всему тексту после текущей позиции.
	SendMessage(h_Edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);

	// Insert text to current position (beggining). Using inline UnicodeString convertion to fix bug with data copy to new pointer.
	SendMessage(h_Edit, EM_REPLACESEL, 0, (LPARAM)(AnsiString(text).c_str()));
}

// Инициализация файла логов.
void LogFileInit() {
	String full_log_path;
	String curr_date;
	DateTimeToString (curr_date, "yyyy.mm.dd", Date());
	if (curr_date != last_init_date) {
		if (log_file.is_open()) {
			log_file.close();
			if (log_file.fail()) {
				throw Exception(__("LogFileCloseError"));
			}
		}
		full_log_path = plugin_data_path + "logs\\" + curr_date + ".log";
		log_file.open(AnsiString(full_log_path).c_str(), ios::out | ios::app | ios::binary);
		if (!log_file.is_open()) {
			throw Exception(__("LogFileOpenError"));
		}

		last_init_date = curr_date;
	}
}

// Функция для упрощения вывода предупреждения от сокетов.
void SocketWarn(String text) {
	InfoAction(text, false, 3, 1);
}

// Функция для упрощения вывода общих логов.
void MainEvent(String text, int type) {
	InfoAction(text, false, 0, type);
}

// Файловое \ Визуальное логирование.
void InfoAction(String text, bool show_message, int group, int type) {
/*
	group - группа уведомления:
		0 - Общие
		1 - БД
		2 - События
		3 - Соединения
	type - тип уведомления:
		0 - обычное уведомление
		1 - предупреждение
		2 - критическая ошибка
*/
	String text_log, text_vis;

	// Удаляем символы нового абзаца и переноса каретки, чтобы в логах не было лишних переносов строк.
	text = ReplaceStr(text, "\n", " ");
	text = ReplaceStr(text, "\r", " ");

	if (file_log || visual_log) {
		String time = FormatDateTime("hh:nn:ss", Time());
		text_log = Format("[%s] [%s] [%s] %s.", ARRAYOFCONST((time, messages[type], l_group_prefix[group], text)));

		if (file_log) {
			try {
				LogFileInit();
				log_file.write(AnsiString(text_log + "\r\n").c_str(), text_log.Length() + 2);
				if (log_file.bad()) {
					throw Exception(__("LogFileWriteError"));
				}
				if (type != 0) {
					LofFileFlush();
				}
			} catch (Exception *E) {
				RTFInsert(Format("%s. " + __("LogFileError") + ".\r\n", ARRAYOFCONST((l_group_prefix[0], E->Message))), 1);
			}
			//FileWrite(file_pos, AnsiString(text_log + "\r\n").c_str(), text_log.Length() + 2);
		}

		if (log_form) {
			RTFInsert(Format("%s. %s.\r\n", ARRAYOFCONST((l_group_prefix[group], text))), type);
			RTFInsert("[" + time + "] ", 0);
		}

		// Проверяем обычное ли это уведомление и если нет, то счётчик ошибок\педупреждений пересчитываем и выводим новые значения.
		if (main_form) {
			main_form->last_log_memo->Text = Format("[%s] %s. %s.", ARRAYOFCONST((time, l_group_prefix[group], text)));
			main_form->last_log_memo->Font->Color = (TColor)color[type];
			if (type != 0) {
				(type == 1) ? warnings_c++ : errors_c++;
				/* TODO : Сделать потокобезопасный вызов функции обновления счетчика ошибок и предупреждений */
				main_form->errors_warnings_update();
			}
		}
	}

	if (show_message) {
		ShowMsg(Application->Handle, text, type);
    }
}
