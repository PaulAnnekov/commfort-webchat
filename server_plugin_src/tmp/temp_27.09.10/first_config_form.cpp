// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "first_config_form.h"
#include "webchat.h"
#include "global.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TFastConfigForm *FastConfigForm;

const int nothing_id = 0, begin_id = 1, host_id = 2, bd_name_id = 3, user_name_id = 4,
user_pass_id = 5, charset_id = 6, prefix_id = 7, keep_connect_id = 8, test_and_go_id = 9,
bot_nick_id = 10, bot_pass_id = 11, bot_male_id = 12, bot_ip_id = 13;

// ---------------------------------------------------------------------------
__fastcall TFastConfigForm::TFastConfigForm(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void TFastConfigForm::load_hint(int element_id) {
	String hint;
	switch(element_id) {
	case nothing_id:
		hint = "Наведите на поле или кнопку, чтобы получить подсказку";
		break;
	case begin_id:
		hint = "Нажмите эту кнопку, чтобы приступить к настройке плагина";
		break;
	case host_id:
		hint =
			"Поле \"Адрес сервера\" требует ввода адреса сервера, на котором "
			"распологается База Данных (БД) MySQL. Примеры: localhost "
			"(если БД локальная), 111.222.333.444, chat_url";
		break;
	case bd_name_id:
		hint = "Поле \"Название БД\" требует ввода названия Базы Данных "
			"MySQL, в которой будут храниться данные. Примеры: test, my_base";
		break;
	case user_name_id:
		hint = "Поле \"Имя пользователя\" требует ввода имени пользователя "
			"для подлкючения к БД. Пользователь должен иметь права для "
			"создания и изменения таблиц, и переменных. "
			"Примеры: root, user_1, admin";
		break;
	case user_pass_id:
		hint =
			"Поле \"Пароль\" требует ввода пароля для указанного пользователя."
			" Как всегда пароль должен быть сложный, но это уже на Ваш выбор";
		break;
	case charset_id:
		hint =
			"Поле выбора \"Кодировка сервера\" требует ввода кодировки сервера MySQL. " "Если Вы используете плагин WebChat для CMS Drupal, то выберите " "кодировку utf8, так как она стандартна для Drupal";
		break;
	case prefix_id:
		hint =
			"Поле \"Префикс таблиц БД\" требует ввода приставки, которая добавляется " "перед названием таблиц, создаваемых плагином. Установите его в соответствующую " "настройку в CMS Drupal";
		break;
	case keep_connect_id:
		hint =
			"Поле выбора \"Поддержка соединения\" требует выбора поддерживать соединение или нет. " "Если Вы обнаружите проблемы с соединением, то требуется это поле пометить";
		break;
	case test_and_go_id:
		hint =
			"Нажмите эту кнопку, чтобы попробывать соединиться с БД. Если соединение успешно, то" "Вы перейдёте к следующему шагу";
		break;
	case bot_nick_id:
		hint =
			"Поле \"Ник в чате\" требует ввода ника бота, который будет подключаться к чату. " "От его имени будут писаться сообщения из веб-интерфейса";
		break;
	case bot_pass_id:
		hint =
			"Поле \"Пароль\" требует ввода пароля для бота. Напоминаем, что пароль должен быть достаточно сложным, чтобы его нельзя " "было угадать методом подбора";
		break;
	case bot_male_id:
		hint =
			"Поле выбора \"Пол\" требует выбора пола бота, то есть икноки, которая будет отображаться в чате";
		break;
	case bot_ip_id:
		hint =
			"Поле \"IP-адрес\" требует ввода IP-адреса бота, который будет отображаться в при наведении курсора на ник бота в чате";
		break;
	default:
		hint =
			"Тут будет текстовая подсказка при наведении на любое поле настройки";
		break;
	}
	hint_label->Caption = hint;
}

void __fastcall TFastConfigForm::host_addr_editMouseEnter(TObject *Sender) {
	load_hint(host_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bd_name_editMouseEnter(TObject *Sender) {
	load_hint(bd_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::charset_chooseMouseEnter(TObject *Sender) {
	load_hint(charset_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::user_name_editMouseEnter(TObject *Sender) {
	load_hint(user_name_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::pass_editMouseEnter(TObject *Sender) {
	load_hint(user_pass_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::prefix_editMouseEnter(TObject *Sender) {
	load_hint(prefix_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::keep_conn_chooseMouseEnter(TObject *Sender) {
	load_hint(keep_connect_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::test_and_go_butMouseEnter(TObject *Sender) {
	load_hint(test_and_go_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_nick_editMouseEnter(TObject *Sender) {
	load_hint(bot_nick_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_pass_editMouseEnter(TObject *Sender) {
	load_hint(bot_pass_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::bot_male_chooseMouseEnter(TObject *Sender) {
	load_hint(bot_male_id);
}
//---------------------------------------------------------------------------
void __fastcall TFastConfigForm::bot_ip_editMouseEnter(TObject *Sender)
{
	load_hint(bot_ip_id);
}
//---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet1MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet3MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::TabSheet2MouseEnter(TObject *Sender) {
	load_hint(nothing_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::begin_butMouseEnter(TObject *Sender) {
	load_hint(begin_id);
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::begin_butClick(TObject *Sender) {
	pages->TabIndex = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::keep_conn_checkMouseEnter(TObject *Sender) {
	load_hint(keep_connect_id);
}

// ---------------------------------------------------------------------------
void __fastcall TFastConfigForm::test_and_go_butClick(TObject *Sender) {
	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString("BD Connection Properties", "HostName",
		host_addr_edit->Text);
	inifile->WriteString("BD Connection Properties", "Database",
		bd_name_edit->Text);
	inifile->WriteString("BD Connection Properties", "User_Name",
		user_name_edit->Text);
	inifile->WriteString("BD Connection Properties", "Password",
		encode(pass_edit->Text));
	inifile->WriteString("BD Connection Properties", "ServerCharSet",
		charset_choose->Items->Strings[charset_choose->ItemIndex]);
	inifile->WriteString("BD Connection Properties", "Prefix",
		prefix_edit->Text);
	inifile->WriteBool("BD Connection Properties", "KeepConnection",
		keep_conn_check->Checked);
	delete inifile;
	if (main_form->establish_connection(true)) {
		MessageBox(Application->Handle,
			"Тест подключения прошёл успешно. Настройки сохранены. Переходим к следующему шагу"
			, "Успешное соединение", MB_OK + MB_ICONASTERISK + MB_APPLMODAL);
		pages->TabIndex = 2;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFastConfigForm::end_butClick(TObject *Sender) {
	TIniFile *inifile = new TIniFile(config_ini_path);
	inifile->WriteString("Bot Properties", "Nick", bot_nick_edit->Text);
	inifile->WriteString("Bot Properties", "Password", encode(bot_pass_edit->Text));
	inifile->WriteBool("Bot Properties", "IsFemale",bot_male_choose->ItemIndex);
	inifile->WriteString("Bot Properties", "IP", bot_ip_edit->Text);
	//------------------------
	if (main_form->establish_connection(false)) {
		bool is_final = 0;
		try {
			main_form->bd_sql_connection->ExecSQL(
				String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;", tables_prefix.w_str(), table_users_online), NULL, NULL
			);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL auto_increment,"
					"`nick` varchar(50) NOT NULL, "
					// максимальная длина имени - 40 символов
					"`ip` varchar(100) NOT NULL, "
					"`male` int(1) NOT NULL default \"0\", "
					"`state` varchar(70) NOT NULL, "
					// макс длинна статуса - 64 символа
					"PRIMARY KEY  (`id`)) DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(), table_users_online), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_messages_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`body` text NOT NULL," // макс. выбираемая длинна сообщения 40000 символов
					"`datetime` datetime NOT NULL,"
					"`nick` varchar(50) NOT NULL,"
					"`male` int(1) NOT NULL default \"0\", "
					"`channel` varchar(40) NOT NULL,"
					// макс. длина имени канала 32 символа
					"`type` tinyint(1) NOT NULL," "PRIMARY KEY (`id`)"
					") DEFAULT CHARSET=utf8;", tables_prefix.w_str(),
							table_messages_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_mess_to_send_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`body` varchar(10000) NOT NULL,"
					"`datetime` datetime NOT NULL,"
					"`user` varchar(50) NOT NULL,"
					"`channel` varchar(40) NOT NULL,"
					"`type` tinyint(1) NOT NULL," "`ip` varchar(100) NOT NULL,"
					"PRIMARY KEY (`id`)" ") DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(),
							table_mess_to_send_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"DROP TABLE IF EXISTS `%s%s`;",
					tables_prefix.w_str(), table_settings_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL
				(String().sprintf(L"CREATE TABLE IF NOT EXISTS `%s%s` ("
					"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
					"`name` varchar(200) NOT NULL,"
					"`value` varchar(200) NOT NULL,"
					"PRIMARY KEY (`id`)" ") DEFAULT CHARSET=utf8;",
					tables_prefix.w_str(),
							table_settings_name), NULL, NULL);

			main_form->bd_sql_connection->ExecSQL(
				String().sprintf(L"INSERT INTO `%s%s` (`name`, `value`) VALUES ('bot_nick', '%s');",
					tables_prefix.w_str(), table_settings_name, bot_nick_edit->Text
				), NULL, NULL
			);
			// устанавливаем таймаут отключения соединеиния на двое суток

			is_final = 1;
			main_form->bd_sql_connection->ExecSQL
				("SET @@global.wait_timeout=172800;", NULL, NULL);
		}
		catch(Exception * E) {
			if (is_final) {
				inifile->WriteInteger("BD Connection Properties",
					"FirstConfigRun", 0);
				MessageBox(NULL,
					"Настройка таймаута не установилась. Возможно не хватает прав пользователя для установки переменных. " "Вы можете проигнорировать этот случай, так как он не является очень важным.", "Предупреждение", MB_OK + MB_ICONWARNING + MB_TASKMODAL);
			}
			else
				MessageBox(NULL, (
					"Возникла ошибка при создании таблиц БД. Выдана такая ошибка: \""
					+ E->Message + "\". "
					"Возможно не хватает прав пользователя. Пожалуйста проверьте настройки БД и плагина.").t_str(), "Серьёзная ошибка", MB_OK + MB_ICONERROR + MB_TASKMODAL);
		}
		if (is_final) {
			if (MessageBox(Application->Handle,
					"Поздравляем! Настройка плагина успешно завершена. Вы хотите выйти из настройки и запустить" " плагин (Нажмите \"Нет\", чтобы не закрывать окно Быстрой настройки и ещё внести какие-либо изменения)?", "Настройка завершена", MB_YESNO + MB_ICONASTERISK + MB_TASKMODAL) == 6)
			{
				is_plugin_active = true;
				inifile->WriteBool("Additional Properties", "FirstPlugRun", false);
				main_form->FormCreate(this);
				Close();
            }
		}
		// if (test)
		// MessageBox(Application->Handle, "Соединение с БД успешно установлено", "Тест успешен", MB_OK + MB_ICONINFORMATION + MB_TASKMODAL + MB_TOPMOST);
	}
	delete inifile;
}
// ---------------------------------------------------------------------------

void __fastcall TFastConfigForm::FormCreate(TObject *Sender)
{
	String charset;

	TIniFile *inifile = NULL;
	try
	{
		inifile = new TIniFile(config_ini_path);

		// подгрузка в поля значений настройки подключения к серверу MySQL
		host_addr_edit->Text      = inifile->ReadString("BD Connection Properties", "HostName", "localhost");
		bd_name_edit->Text        =  inifile->ReadString("BD Connection Properties","Database", "drupal");
		user_name_edit->Text      =  inifile->ReadString("BD Connection Properties","User_Name", "root");
		pass_edit->Text           =  decode(inifile->ReadString("BD Connection Properties","Password", ""));

		charset                   =  inifile->ReadString("BD Connection Properties", "ServerCharSet", "utf8");
		int i = 0;
		do {
			if (charset == charset_choose->Items->Strings[i]) {
				charset_choose->ItemIndex = i;
				break;
			}
			i += 1;
		}
		while (i > charset_choose->Items->Count - 1);

		prefix_edit->Text         =  tables_prefix;
		keep_conn_check->Checked  =  keep_conn;
	}
	__finally
	{
		if (inifile) {
			delete inifile;
			inifile = NULL;
		}
	}
	// подгрузка в поля значений настроек бота
	bot_nick_edit->Text = bot_name;
	bot_pass_edit->Text = decode(bot_pass);
	bot_ip_edit->Text = bot_ip;
	bot_male_choose->ItemIndex = bot_is_female;

}
//---------------------------------------------------------------------------

