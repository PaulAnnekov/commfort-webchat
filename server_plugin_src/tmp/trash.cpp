			/*case PM_PLUGIN_REGISTERED_LIST :
				query = Format("INSERT INTO `%s%s` (`nick`, `ip`) VALUES ",
						ARRAYOFCONST((tables_prefix, table_users_name)));
//	  msg = TStringStream->Create(String(bMessage));
//	  msg->Size = dwMessageLength;
//	  msg->Position = 0;
//	  msg->Read(N,4);
				p = (char*)bMessage;

                n = *((DWORD*)p);
				num = n;
				p+=4;


				for (j=1; j<=num; j++)
				{
                	n = *((DWORD*)p);
					p+=4;
					name = new char[n+1];
					memcpy(name,p,n);
					name[n] = 0;
					p+=n;

					n = *((DWORD*)p);
					p+=4;
					ip = new char[n+1];
					memcpy(ip,p,n);
					ip[n] = 0;
					p+=n;

					query += Format("(\"%s\", \"%s\")",
							 ARRAYOFCONST((name, ip)));
					if (j == num)
						query += ";";
					else query += ", ";
				}
//		  msg->Read(I,4);
//		  name = msg->ReadString(I);
//		  msg->Read(I,4);
//		  ip = msg->ReadString(I);
//		  query = query + '("' + name + '", "' + ip + '")';

          //CForm->Memo1->Lines->Add(name + ' / ' + ip);
//		end;
        //CForm->Memo2->Lines->Add(query);
        //Try
				main_form->progress_bar_change(5, 0, "Удаление старой таблицы");
				main_form->bd_sql_connection->ExecuteDirect(
					Format("DROP TABLE IF EXISTS `%s%s`;",
							ARRAYOFCONST((tables_prefix, table_users_name)))
				);
				main_form->progress_bar_change(10, 0, "Создание новой таблицы");
				main_form->bd_sql_connection->ExecuteDirect(
					Format(
						"CREATE TABLE IF NOT EXISTS `%s%s` ("
						"`id` bigint(20) unsigned NOT NULL auto_increment,"
						"`nick` varchar(50) NOT NULL, "   //максимальная длина имени - 40 символов
						"`ip` varchar(100) NOT NULL, "
						"`male` int(1) NOT NULL default \"0\", "
						"`state` varchar(70) NOT NULL, "   //макс-> длина статуса - 64 символа
						"`online` int(1) NOT NULL default \"0\", "
						"`activity_time` datetime NOT NULL, "
						"PRIMARY KEY  (`id`)) DEFAULT CHARSET=utf8;",
						ARRAYOFCONST((tables_prefix, table_users_name))
					)
				);
//			'CREATE TABLE IF NOT EXISTS `' + tables_prefix + table_users_name + '` (' +
//			'`id` bigint(20) unsigned NOT NULL auto_increment,' +
//			'`nick` varchar(50) NOT NULL, ' +  //максимальная длина имени - 40 символов
//			'`ip` varchar(100) NOT NULL, ' +
//			'`male` int(1) NOT NULL default "0", ' +
//			'`state` varchar(70) NOT NULL, ' +  //макс-> длина статуса - 64 символа
//			'`online` int(1) NOT NULL default "0", ' +
//			'`activity_time` datetime NOT NULL, ' +
//			'PRIMARY KEY  (`id`)) DEFAULT CHARSET=utf8;'
//		  );
				main_form->progress_bar_change(60, 0, "Вставка списка зарегистрированных пользователей");
				main_form->bd_sql_connection->ExecuteDirect(query);
				main_form->add_new_action_list("Список зарегистрированных пользователей обновлен", "");
        //except
         // on E : Exception do
            //MessageBox(0, PChar('ошибка - ' + E->Message) , PChar('класс - ' + E->ClassName), 0);
         //   ShowMessage(E->Message + '/n' + E->StackTrace + '/n' + E->ClassName
         //      + '/n' + E->GetBaseException->Message + '/n' + E->GetBaseException->ClassName);
        //end;
			break;*/

			/*case PM_PLUGIN_USER_REGISTERED: begin
	  msg = TStringStream->Create(String(bMessage));
	  msg->Size = dwMessageLength;
	  msg->Position = 0;
	  msg->Read(I,4);
	  name = msg->ReadString(I);
	  msg->Read(I,4);
	  ip = msg->ReadString(I);
	  query = 'INSERT INTO `' + tables_prefix + table_users_name +
				'` (`nick`, `ip`) VALUES ("' + name + '", "' + ip + '");';
	  CForm->add_new_action_list('Зарегистрирован новый пользователь: ' + name + ' / ' + ip, query);
	  //Try
		if (CForm->establish_connection(false) = true) then
		  CForm->bd_sql_connection->ExecuteDirect(query);
//        except
//          on E : Exception do
//            MessageBox(0, PChar('ошибка - ' + E->Message) , PChar('класс - ' + E->ClassName), 0);
//        end;
  end;*/

/*Обработка символов ' в строке посылаемой запросу
проверка текста сообщения на содержание спец-> символов и замена их на
html-кодовый эквивалент*/


				//i = 1;
//				num = 1;
				//new_text = text;
//				text_str = new_text;
//				StrPos(

//				while (i != 0)
//				{
//					i = new_text.Pos("'");
//					if (i != 0 && new_text[i-1] != "\\")
////					{
//						new_text.Insert("\\", i-1);
//						//text_str.Delete(1, i);
////						num += i - 1;
////						new_text.Delete(num, 1);
////						new_text.Insert("&#034", num);
////						num += 5;
////					}
//				}

//				while (i!=NULL)
//				{
//					i = StrPos(text, "\"");
//					if (i != NULL) {
//						text += i;
//
//					}
//				}