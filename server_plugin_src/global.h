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

#ifndef globalH
#define globalH

#include <vector>
#include <map>
#include <WideStrings.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include "IdCoderMIME.hpp"
#include "main_form.h"


// Объявления переменных.

// Структура с названиями таблиц.
extern struct TABLE_NAMES_ST {
	String users_online; //Название таблицы для хранения списка пользователей онлайн
	String actions;      //Название таблицы для хранения всех действий в чате
	//String mess_to_send; //Название таблицы для сообщений требующих отправки
	//String settings;     //Название таблицы для настроек сервера
	//String web_users;    //Название таблицы для списка пользователей с веба
	String channels;     //Название со списком отслеживаемых каналов
	String users;        //Название таблицы для списка зарегистрированных пользователей
	String user_channel; //Название таблицы для сопоставления пользователя и каналов в которых он находится
} tbl_names;

// Структура с данными для соединения с БД.
extern struct DB_PROP_ST {
	String host; //Адрес хоста БД
	String user; //Имя пользователя БД
	String pass; //Пароль БД
	String name; //Имя БД
} db_prop;

// Структура с данными для подключения бота.
extern struct BOT_PROP_ST {
	String name;    //ник бота
	String ip;      //ip бота
	String pass;    //пароль бота
	bool is_female; //Бот женского пола?
} bot_prop;

extern int work_time_begin;  // Время начала работы плагина.

extern String tables_prefix,       // Префикс таблиц
              chnls_line, 	       // Список каналовв строку.
			  plugin_data_path,    // Путь к папке с файлами плагина
			  data_storage,       // Путь к папке с файловым хранилищем
			  web_storage,         // Путь к папке в вебе с файловым хранилищем
              language,            // Язык программы.
			  config_ini_path;     // Путь к ini файлу конфига

extern const COLORREF color[3];

extern const String plugin_name,      // Название плагина.
					plugin_version,       // Версия плагина.
					json_rpc_version; // Версия JSON-RPC.

extern bool visual_log,         //Визуальное логирование в окне включено?
			file_log,           //Логирование в файлы включено?
			sync_enabled,       //плагин работает?
			auto_db_clear,      //автоматически чистить БД?
			auto_log_clear,     //автоматически чистить визуальный лог?
            show_work_time,     // Показывать время работы плагина?
			plugin_started,     //был ли инициализирован плагин? (подгружаются настройки и т.п.)
			sync_autostart,     //стартует ли плагин при включении?
			isfirstrun,         //впервые ли запущен плагин?
			channels_policy,    //политика отслеживания каналов - 0. Все, кроме из списка, 1. Только из списка
			info_action_using,  //флаг, указывающий на занятость функции Лога. Используется для синхронного доступа нескольких процессов к функции Логирования
			no_white_ips,       // Указывает как определять скрытие IP: через запрос к серверу или через белый список в настройках
			log_notifications,  // Add notifications messages to log.
			check_version,      // Auto version check.
			auto_update;        // Auto update to newest version.

extern int remaining_time,      //промежуток времени до обновления
		   elapsed_time,        //промежуток прошедшего времени до обновления
		   errors_c,            // Счетчик ошибок.
		   warnings_c;          // Счетчик предупреждений.

extern unsigned main_thread_id; // Main thread ID.

extern TStringList *white_ips;     //Список ников пользователей, IP которых не показывается в вебе
				   //*condit_channels; //Список каналов которые исключены\включены для отслеживания ботом
				   //*watched_chnls; //Список отслеживаемых ботом каналов, уже полностью сформированный список

extern TMainForm *main_form;

// DLL Handle;
extern HINSTANCE Instance;

extern CRITICAL_SECTION connections_manipulation_cs;
extern CRITICAL_SECTION users_manipulation_cs;
extern CRITICAL_SECTION channels_manipulation_cs;

// Список строк.
typedef std::vector<String> StringList;

// Список переменная=значение.
typedef std::map<String, String> MapList;

extern String SaveData(char *data, int data_length);
extern String LoadImage(String file_name);

extern String Base64BinaryEncode(void* databuf, unsigned int bufsize);

// Извлечение данных из потока.
extern int GetStreamDword(BYTE **p, DWORD *len);
extern int GetStreamBinary(BYTE **p, char &data, int len);
extern int GetStreamString(BYTE **p, UnicodeString *var);
extern int GetStreamTime(BYTE **p, double &time);
extern int ShiftBytes(char **ptr, int len, int offset);

extern String implode(StringList array);

// Функция вычисления значений из строки.
extern void SetStreamString(BYTE *p, DWORD * offset, UnicodeString var, DWORD len);

// Поиск строки в списке строк.
extern int ListSearch(TStringList *list, String line);
extern int ListSearch(String *list, String line, int count);
extern StringList::iterator ListSearch(StringList &list, String line);

extern String GetPluginFullName(String version = "");

extern String FormatError(int error_code);
extern String FormatError(int error_code, char *library);
// Функции для кодирования и декодирования строк.
//extern String Base64Encode(String line);
//extern String Base64Decode(String line);
extern String GetMD5(String line);

extern String get_ip(HWND hWindow);

// Получаем время, прошедшее от заданного времени до текущего в формате hh:mm:ss.
extern String get_time_offset(int beg_time);
//---------------------------------------------------------------------------
#endif

