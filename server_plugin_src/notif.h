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

#ifndef notifH
#define notifH

#include <WideStrings.hpp>
#include <Dialogs.hpp>
#include <fstream>
//---------------------------------------------------------------------------

//handle для файла логов
extern int file_pos;

extern ofstream log_file;

extern const String
	// Сообщения об ошибках.
    e_sql_on_init,
	e_cant_connect,
	e_sql_syntax,
	e_mysql_query,
	e_db_base_create,
	e_plug_start,
	e_user_mess_recieve,
	e_user_channel_connect,
	e_virtual_user_channel_connect,
	e_user_connect,
	e_user_channel_disconnect,
	e_user_disconnect,
	e_user_state_changed,
	e_user_sex_changed,
	e_chnl_theme_changed,
	e_unsuccess_auth,
	e_plug_stop,
	e_bot_auth[9],
	e_tcp_addr,
	e_tcp_listen,
	e_tcp_connection_create,
	e_tcp_during_listen,
	e_tcp_socket_accept,
	e_tcp_write,
	e_tcp_read,
	e_tcp_socket_close,
	e_tcp_connections_check,
	e_tcp_client_during_listen,
	e_tcp_client_during_read,
	e_notif_db,
	e_image_unique,
	e_image_open,
	e_image_write,
	e_image_close,
	e_image_unexpected,
	e_bot_base_load,

	// Уведомления.
	n_connect_success,
	n_disconnect_success,
	n_plugin_sync_started,
	n_users_list_updated,
	n_user_message_recieved,
	n_user_channel_connect,
	n_virtual_user_channel_connect,
	n_user_connect,
	n_user_channel_disconnect,
	n_user_disconnect,
	n_user_state_changed,
	n_user_sex_changed,
	n_channel_theme_changed,

	// Дополнительный текст.
	a_sql_streams[3],

	// Заголовки для окон уведомлений.
	sm_warning,
	sm_error,
	sm_question,
	sm_asterisk,

	// Текст элементов форм.
	t_show_connection_button,
	t_db_connection_yes,
	t_db_connection_no,
	t_plug_off,
	t_plug_on,
	t_plug_started,
	t_plug_offline,
	t_connections_off,
	t_online_users_unknown,
	t_bot_auth_wait;

// Инициализация файла логов.
void LogFileInit();

// Загрузка лога из файла в Memo поле.
void LoadLog();

// Функция для упрощения вывода предупреждения от сокетов.
void SocketWarn(String text);

// Функция для упрощения вывода общих логов.
void MainEvent(String text, int type);

// Файловое \ Визуальное логирование.
void InfoAction(String text, bool show_message, int group, int type);

// Показ сообщения.
int ShowMsg(HWND handle, String main_t, int type);

// Вывод сообщения в Memo поле.
void RTFInsert(String text, int type);
//---------------------------------------------------------------------------
#endif
