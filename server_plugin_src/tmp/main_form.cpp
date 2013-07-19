//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "main_form.h"
#include "webchat.h"
#include "global.h"
#include "first_config_form.h"
#include "connections_form.h"
#include "cf_functions.h"
#include <ShellApi.h>
#include <DateUtils.hpp>
#include <stdio.h>
#include "notif.h"
#include "check_conn.h"
#include "log_form.h"
#include "plug_init.h"
#include "l18n.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFastConfigForm *first_config_form;

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

int TMainForm::DoAction(TMessage &Message) {
	try {
		web_users_state->Caption = (Message.LParam > 0) ? String(Message.LParam) : __("NoConnections");
	} catch(Exception *E) {
		InfoAction("Error during Main form connections synchronization: \"" + E->Message + "\"", false, 3, 1);
	} catch(...) {
		InfoAction("Serious error during Main form connections synchronization", false, 3, 2);
	}

	return 1;
}
//---------------------------------------------------------------------------

void TMainForm::bot_denied(int error_code) {
//попытка подключения бота завершилась неудачей
	sync_enabled = false;
	plugin_state->Caption = "Отключен";
	plug_sync_but->Caption = "Включить";
	plugin_state_item->Caption = "Включить плагин";

	ShowMsg(Handle, "Бот не смог войти в чат по следующей причине: \n\"" + e_bot_auth[error_code] + "\"", 1);
}
//---------------------------------------------------------------------------
void TMainForm::bot_connected() {


//	// Формируем список каналов и вводим бота в эти каналы.
//	get_channels_list();
//
//	bool sucess = true;
//	String query;
//	int i, chnls_count = watched_chnls->Count;
//
//	// Проверяем список отслеживаемых каналов на их наличие.
//	if (chnls_count != 0) {
//		// Т.к. бот может находится максимум в 16-ти каналах - выводим уведомление админу и удаляем оставшиеся каналы.
//		if (chnls_count > 16) {
//			MainEvent("Количество отслеживаемых каналов более 16-ти. Бот может находится в неболее чем 16-ти каналах. "
//				"Отслеживаться будут первые 16 каналов из списка. Вы можете выбрать нужные каналы или убрать не нужные "
//				"в Настройках", 0);
//			for (i = 16; i < chnls_count; i++) {
//				watched_chnls->Delete(i);
//			}
//			chnls_count = 16;
//		}
//
//		query = Format(
//			"TRUNCATE TABLE `%s%s`;"
//			"INSERT INTO `%s%s` (`name`, `topic`) VALUES ",
//			ARRAYOFCONST((tables_prefix, tbl_names.channels, tables_prefix, tbl_names.channels))
//		);
//
//		String *active_chnls;
//		int active_count = virt_user_channels(bot_prop.name, NULL);
//		active_chnls = new String[active_count];
//		virt_user_channels(bot_prop.name, active_chnls);
//
//		int index;
//		// Если channels != NULL, то есть если не возникло ошибки при заполнении массива каналами.
//		if (active_chnls != NULL) {
//			for (i = 0; i < chnls_count; i++) {
//				index = ListSearch(active_chnls, watched_chnls->Names[i], active_count);
//				// Если в списке каналов, в которых сидит пользователь найден нужный канал (то есть бот уже к нему)
//				// подключен, то обнуляем этот канал - ускорим поиск и потом из оставшихся каналов отключаем
//				// пользователя, потому что их не надо отслеживать.
//				if (index != -1) {
//					active_chnls[index] = NULL;
//				}
//				else {
//					sucess = virt_user_chnl_conn(bot_prop.name, watched_chnls->Names[i]);
//				}
//			}
//
//			// Отключаем бота от каналов, в которых он находится, а их не надо отслеживать.
//			for (i = 0; i < active_count; i++) {
//				if (active_chnls[i] != NULL) {
//					virt_user_chnl_disconn(bot_prop.name, active_chnls[i]);
//				}
//			}
//
//			// Удаляем выделенную память экземпляру класса в функции virt_user_channels.
//			delete []active_chnls;
//
//			// Если последнее добавление пользователя в канал было удачным (скобка в запросе закрылась), то выполняем запрос.
//			if (sucess) {
//				for (int i = 0; i < chnls_count; i++) {
//					query += Format("(\"%s\", \"%s\")", ARRAYOFCONST((watched_chnls->Names[i], db_conn->get_escaped(watched_chnls->ValueFromIndex[i]))));
//					if (i != chnls_count - 1)
//						query += ", ";
//
//					// Удаляем названия тем каналов из массива канал=тема.
//					watched_chnls->Strings[i] = watched_chnls->Names[i];
//				}
//
//				sucess = db_conn->query(query, 0);
//			}
//		}
//		else {
//			sucess = false;
//		}
//
//		// Если последнее добавление пользователя было неудачным или запрос был неудачным, то показываем ошибку.
//		if (!sucess)
//			InfoAction("Произошла ошибка во время подключения бота к каналам. Возможно отсутствует подключение к БД или не возможно подключиться к каналу", true, 0, 1);
//	}
//	else {
//		InfoAction("Список отслеживаемых каналов пуст. Проверьте настройки списка каналов для отслеживания", true, 0, 0);
//	}
//
//	// Полностью обновляем список пользователей при подключении\восстановлении соединения.
//	update_users_list();
//
//	// Запускаем прослушку порта.
//	port_listener = new TPortListener(true);
//	port_listener->Start();
//
//	// Запускаем таймер проверки соединений.
//	conn_check_timer = new TConnCheckTimer();
//
//	// Устанавливаем боту состояние.
//	set_user_state(bot_prop.name, "Веб-интерфейс онлайн");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::exception_handler(TObject *Sender, Exception *E) {
  MainEvent("Неизвестная ошибка: \"" + E->Message + "\" с пометкой \"" + E->ClassName() + "\" в элементе " + Sender->ClassName(), 2);
}
//---------------------------------------------------------------------------
void TMainForm::load_first_config_run_form() {
	first_config_form = new TFastConfigForm(NULL);
	first_config_form->pages->TabIndex = 0;
	first_config_form->ShowModal();
	delete first_config_form;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action) {
	open_window_item->Caption = "Открыть окно";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::tray_iconClick(TObject *Sender) {
	open_window_item->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::load_first_config_butClick(TObject *Sender) {
	sync_enabled = false;
	load_first_config_run_form();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::online_users_butClick(TObject *Sender) {
	try {
    	// Полностью обновляем список пользователей в БД и таблиц соответствий пользователь=канал.
		UpdateUsersTable();
		MainEvent(n_users_list_updated, 0);    
	} catch (String error) {
		MainEvent(error, 1);
	}
}
//---------------------------------------------------------------------------
void TMainForm::set_window_height() {
	// Устанавливаем статическую высоту окна учитывая высоту верхнего бара, которая может быть везде разная.
	int bar_height = Height - ClientHeight;
	Constraints->MaxHeight = 170 + bar_height;
	Constraints->MinHeight = 170 + bar_height;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::open_log_butClick(TObject *Sender) {
	errors_c = 0;
	warnings_c = 0;
	errors_warnings_update();

	if (!log_form) {
		log_form = new TLogForm(Application);
		log_form->Position = poMainFormCenter;
		log_form->Show();
	} else {
		log_form->BringToFront();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plug_sync_butClick(TObject *Sender) {
	if (sync_enabled) {
		FinishSync();
	} else {
		StartSync();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::stop_plugin_butClick(TObject *Sender) {
	int id = ShowMsg(Handle, "Вы действительно хотите завершить работу плагина?"
				" Это приведёт к полной остановке его работы.", 3);
	if (id == IDYES) {
		(*CommFortProcess)(dwPluginID, 2100, NULL, NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::about_butClick(TObject *Sender) {
	PluginShowAbout();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::settings_butClick(TObject *Sender) {
	PluginShowOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::open_window_itemClick(TObject *Sender) {
	if (Visible) {
		Close();
		open_window_item->Caption = "Открыть окно";
	}
	else {
		Show();
		open_window_item->Caption = "Свернуть окно";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::settings_itemClick(TObject *Sender) {
	PluginShowOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::about_itemClick(TObject *Sender) {
	PluginShowAbout();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plugin_terminate_itemClick(TObject *Sender) {
	stop_plugin_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::plugin_state_itemClick(TObject *Sender) {
	plug_sync_but->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::work_time_timerTimer(TObject *Sender) {
	work_time_timer->Enabled = false;

	// Обновляем время.
	time_plus_state->Caption = get_time_offset(work_time_begin);

	work_time_timer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender){
	if (isfirstrun) {
		int id = ShowMsg(Handle, "Плагин запущен впервые и требует ОБЯЗАТЕЛЬНОЙ настройки. "
			"Хотите запустить Мастера быстрой настройки (в противном случае откроется "
			"окно полной настройки)?", 3);

		if (id == IDYES) {
			load_first_config_run_form();
		}
		else {
			TIniFile *inifile = NULL;
			try {
				inifile = new TIniFile(config_ini_path);
				inifile->WriteBool("Additional Properties", "FirstPlugRun", false);
			}
			__finally {
				if (inifile) {
					delete inifile;
				}
			}

			PluginShowOptions();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	// Запускаем таймер показа времени работы плагина и включаем показ времени, если пользователь выбирал эту опцию.
	if (show_work_time) {
		time_plus_lbl->Visible = true;
		time_plus_state->Visible = true;
		work_time_timer->Enabled = true;
	}

	//set_window_height();
	Application->OnException = exception_handler;

	Caption = plu_name + " " + plu_ver;
	tray_icon->Hint = Caption;

	errors_warnings_update();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::show_conn_butClick(TObject *Sender)
{
	if (!conn_list_form) {
		conn_list_form = new TConnectionsList(Application);
		conn_list_form->Position = poMainFormCenter;
		conn_list_form->Show();
	} else {
		conn_list_form->BringToFront();
	}
}
//---------------------------------------------------------------------------

void TMainForm::errors_warnings_update() {
	errors_state->Caption = String(errors_c) + " / " + String(warnings_c);
}




