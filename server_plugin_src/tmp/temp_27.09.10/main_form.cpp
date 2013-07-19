//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#pragma comment(lib, "ws2_32.lib")

#include "main_form.h"
#include "global.h"
#include "webchat.h"
#include "first_config_form.h"
#include <ShellApi.h>
//#include "vars.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MyAccess"
#pragma link "DBAccess"
#pragma link "MyAccess"
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma resource "*.dfm"
TConnectThread * ConnectThread;
TFastConfigForm *first_config_form;

int file_pos,         //handle ��� ����� �����
	web_timer = 0,        //������� ������� ��� ������� �������� ��������� � ����
	bd_clear_timer = 0;   //������� ������� ��� ������� ������� ��
	//id;

String last_init_date; //��������� ���� ������������� (������������ ��� ��������� � ������� ����� � ����� ����� � � ������� ����� ��� ������� ��)

SOCKET Client = INVALID_SOCKET;

SOCKADDR_IN rmTCP;

TIMEVAL timeVal;
FD_SET readFDS;

__fastcall TConnectThread::TConnectThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TConnectThread::Execute()
{
	main_form->establish_connection(false);
}

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}

void TMainForm::debug(String text, BYTE * var, DWORD Length)
{
	int smth;
	String debug_path = ExtractFilePath(Application->ExeName) + "Plugins\\debug.txt";
	text.sprintf(L"[%s %s] %s.", TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), text);
	//text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), text)));
	if (FileExists(debug_path))
		{
			smth = FileOpen(debug_path, fmOpenReadWrite);
			FileSeek(smth, 0, 2);
		}
	else smth = FileCreate(debug_path);

	//FileWrite(smth, (text + "\r\n").t_str(), text.Length() + 2);
	FileWrite(smth, var, Length);
	///FileWrite(smth, "\r\n****************\r\n", 20);
	FileClose(smth);
}
//---------------------------------------------------------------------------
void TMainForm::get_users_list(String channel)
{
	BYTE * data = NULL, * bOutBuffer = NULL, *pCopy;
	try {
		DWORD pos_offset = 0, len, name_len, channel_len;
		name_len = bot_name.Length();
		channel_len = channel.Length();
	/*
		ID: 1081
		���� ������ (���������): �����(��� ������������ ������������) + �����(�����)
		len = 4 + name_len * 2 + 4 + channel_len * 2;
		���� ������ (��������): �����(����������) + ������������()*����������
	*/
		len = (name_len + channel_len) * 2 + 8;
		bOutBuffer = new char[len];

		set_stream_str (bOutBuffer, &pos_offset, bot_name, name_len);
		set_stream_str (bOutBuffer, &pos_offset, channel, channel_len);

		DWORD size = (*CommFortGetData)(dwPluginID, 1081, NULL, NULL, bOutBuffer, len);
		data = new BYTE[size];
		pCopy = data;
		(*CommFortGetData)(dwPluginID, 1081, data, size, bOutBuffer, len);

		String nick, ip, query;
		DWORD num, male, i;
		int line;

		query.sprintf(L"INSERT INTO `%s%s` (`nick`, `ip`, `male`) VALUES ",
				tables_prefix.w_str(), table_users_online);
		get_stream_dword (&pCopy, &num);

		online_users_state->Caption = num;
		for (i = 1; i<=num; i++)
		{
			//nick
			get_stream_string (&pCopy, &nick);
			//ip
			get_stream_string (&pCopy, &ip);
			//male
			get_stream_dword (&pCopy, &male);

			for (line = 0; line < white_ips->Count; line++)
				if (nick == white_ips->Strings[line]) {
					ip = "N/A";
					line = white_ips->Count;
				}

			query += String().sprintf(L"(\"%s\", \"%s\", \"%d\")",
					nick, ip, male);
			if (i == num)
				query += ";";
			else query += ", ";
		}

		if (establish_connection(false))
		{			bd_sql_connection->ExecSQL(
				String().sprintf(L"TRUNCATE TABLE `%s%s`", tables_prefix.w_str(), table_users_online), NULL, NULL);
			bd_sql_connection->ExecSQL(query, NULL, NULL);
		}
		add_new_action_list("������� ������������� ������ ���������", query, false, 0);
	}
	catch (Exception *E)
	{
		main_form->add_new_action_list("��������� ������ ��� ��������� ������ �������������. ������: \"" + E->Message + "\"", "", false, 2);
	}
	if (bOutBuffer) {
        delete []bOutBuffer;
		bOutBuffer = NULL;
		if (data) {
			delete []data;
			data = NULL;
			pCopy = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void TMainForm::virt_user_conn (String name, String ip, String pass, bool is_female)
//������� ������������ ������������
{
	DWORD len, pos_offset = 0, name_len, ip_len, pass_len, pass_type = 0;
	name_len = name.Length();
	ip_len = ip.Length();
	pass_len = pass.Length();
	//�����(���) + �����(IP-�����) + �����(��� ������) + �����(������) + �����(������)
	//len = 4 + bot_name.Length() * 2 + 4 + bot_ip.Length() * 2 + 4 + 4 + bot_pass.Length() * 2 + 4;

	len = 20 + (name_len + ip_len + pass_len) * 2;
	BYTE * bOutBuffer = NULL;
	try {
		bOutBuffer = new char[len];

		set_stream_str (bOutBuffer, &pos_offset, name, name_len);

		set_stream_str (bOutBuffer, &pos_offset, ip, ip_len);

		set_stream_str (bOutBuffer, &pos_offset, NULL, pass_type);

		set_stream_str (bOutBuffer, &pos_offset, pass, pass_len);

		set_stream_str (bOutBuffer, &pos_offset, NULL, is_female);

		(*CommFortProcess)(dwPluginID, 1001, bOutBuffer, len);
	}
	__finally
	{
		if (bOutBuffer)
			delete []bOutBuffer;
	}
}
//---------------------------------------------------------------------------
void TMainForm::virt_user_disconn (String name)
//��������� ������������ ������������
{
	DWORD len, pos_offset = 0, name_len;
	name_len = name.Length();
	//�����(���)

	len = 4 + name_len * 2;
	BYTE * bOutBuffer = NULL;
	try
	{
		bOutBuffer = new char[len];

		set_stream_str (bOutBuffer, &pos_offset, name, name_len);

		(*CommFortProcess)(dwPluginID, 1002, bOutBuffer, len);
	}
	__finally
	{
		if (bOutBuffer)
			delete []bOutBuffer;
	}
}
//---------------------------------------------------------------------------
void TMainForm::virt_user_init (bool is_connected, int error_code)
//����� �� ������� ����������� ���� � ����
{
	if (is_connected) {
		web_public_state_label->Enabled = true;
		web_public_state->Enabled = true;
		web_public_state_but->Enabled = true;
		online_users_label->Enabled = true;
		online_users_state->Enabled = true;
		online_users_but->Enabled = true;
		web_public_state_but->Click();
		plugin_state->Caption = "��������";

		//��������� ��������� ������ ������������� ��� �����������\�������������� ����������
		get_users_list(active_channel);
		//������������� ���� ���������
		set_user_state("���-��������� ������");
	}
	else
	{
        is_plugin_active = false;
		String error_message;
		switch (error_code) {
			case 0: error_message = "��������� ������������ ����� ������������ �������������. ���� ���������� ����� ��� ����";
					break;
			case 1: error_message = "��� ���� �� ������������� ����������� (�������� ��������� �������, ���� ��������� ������������ ����� � 40 ��������). �������� ��� ���� � ���������� �������";
					break;
			case 2: error_message = "��� ���� ������ � ������� �������� (���). ������� ���";
					break;
			case 3: error_message = "��� ���� �������� ����������� ����� (�� �������� ������ ������ ����). �������� ��� ���� � ���������� �������";
					break;
			case 4: error_message = "������ �� ��������� ��� ��� ����������������. �������� ��� ���� � ���������� �������";
					break;
			case 5: error_message = "��������� ������������ ����� ������� ������� � ������� IP-������. ������� IP � ���������� � ���������� ������� ��� �������� IP � ���������� �������";
					break;
			case 6: error_message = "��� ����� � ������� �� ��������� ������ �� ���������. ����������� ������� ������ ����";
					break;
			case 7: error_message = "�������� ������";
					break;
			case 8: error_message = "������ �� ��������� ���� �� ����������, ���� ���������. ��������� ��������� ������";
					break;
		}
		MessageBox(Handle, ("��� �� ���� ����� � ��� �� ��������� �������: \n\"" + error_message + "\"").t_str(), "��������������",
			MB_OK + MB_ICONWARNING + MB_APPLMODAL);
		plugin_state->Caption = "��������";
		plugin_state_but->Caption = "��������";
		plugin_state_item->Caption = "�������� ������";
	}
}
//---------------------------------------------------------------------------
void TMainForm::set_user_state(String state)
{
	/*
		ID: 1025
		���� ������: �����(��� ������������ ������������) + �����(����� ���������)
		len = 4 + name_len * 2 + 4 + state_len * 2;
	*/
	DWORD len, state_len, name_len, pos_offset;
	state_len = state.Length();
	name_len = bot_name.Length();
	len = 8 + (name_len + state_len) * 2;
	BYTE * data;
	try
	{
		data = new char[len];
		pos_offset = 0;
		set_stream_str (data, &pos_offset, bot_name, name_len);

		set_stream_str (data, &pos_offset, state, state_len);

		(*CommFortProcess)(dwPluginID, 1025, data, len);
	}
	__finally
	{
		if (data)
			delete []data;
	}
}
//---------------------------------------------------------------------------
void TMainForm::add_new_action_list(String text, String query, bool show_message, int type)
/*
	type - ��� �����������:
		0 - ������� �����������
		1 - ��������������
		2 - ����������� ������
*/
{
	String text_log;
	if (type) //��������� ������� �� ��� ����������� � ���� ���, �� ������� ������\������������� ������������� � ������� ����� ��������
	{
		if (type == 1)
			warnings_state->Caption = warnings_state->Caption.ToInt() + 1;
		else errors_state->Caption = errors_state->Caption.ToInt() + 1;
	}

	if (file_log || visual_log)
	{
		text_log.sprintf(L"[%s %s] %s.", TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), text);
		if (query != "")
			query = "�������� ������: " + query;

		if (file_log) {
			log_file_init(false);
			FileWrite(file_pos, (messages[type] + text_log + "\r\n").t_str(), messages[type].Length() + text_log.Length() + 2);
			if (query != "")
				FileWrite(file_pos, (query + "\r\n").t_str(), query.Length()+2);
			FileWrite(file_pos, "****************\r\n", 18);
		}
		if (visual_log) {
			states_list_rich->SelStart = states_list_rich->Text.Length();
			states_list_rich->SelAttributes->Color = color[type];
			states_list_rich->SelText = text_log;
			states_list_rich->SelAttributes->Color = clBlack;
			if (query != "")
				states_list_rich->Lines->Add(query);
			states_list_rich->Lines->Add("****************\r\n");
			SendMessage(states_list_rich->Handle, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM,0), 0);
		}
	}
	if (show_message)
		MessageBox(Application->Handle, text.t_str(), "���������", MB_OK + MB_ICONASTERISK + MB_TASKMODAL);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::bd_sql_connection1AfterConnect(TObject *Sender)
{
	bd_connection_state->Caption = "�����������";
	bd_connection_but->Caption = "���������";
	add_new_action_list("���������� � �� �����������", "", false, 0);
}
//---------------------------------------------------------------------------

bool TMainForm::establish_connection(bool test)
//��������� ����������� � ��. ���� test = true, ������ ��� �������� ����������
//��� �������� ����������������� ��������
//��������� false, ���� ������ ����������� � true, ���� ���������� �������
{   /*Memo1->Lines->Clear();
	String text;
	text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "������ ��������/������������ ����������")));
	Memo1->Lines->Add(text);
	FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);*/
	bool result = true;
	if (test)
			bd_sql_connection->Connected = false;
	/*if (bd_sql_connection->Connected && keep_conn) //��������� ������ �� ���� ����������, ���� ����� ��������� ��������� ����������
	{
			try
			{   */
				/*text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "���������� ���� � �������������. �������� ������")));
				FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);
				Memo1->Lines->Add(text);  */

				//bd_sql_connection->ExecuteDirect("DO 1;");
				/*text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "�������� ������ ������")));
				FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);
				Memo1->Lines->Add(text);   */
			/*}
			catch (Exception *ex)
			{
				add_new_action_list("���������� ��������� �������� MySQL, ���������������", "", false, 1);
				bd_sql_connection->Connected = false; //����������� � �������� ���������������� � ��������� if-�
			}
	}     */
	//bd_sql_connection->Ping();

	if (!bd_sql_connection->Connected)
	{
		try
		{
			/*text  = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "���������� ���, �����������")));
			FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);
			Memo1->Lines->Add(text);  */
			bd_sql_connection1->Params->LoadFromFile(config_ini_path);

			//���������� ������ � ��
			if (bd_sql_connection1->Params->Values["Password"] != "") {
				bd_sql_connection1->Params->Values["Password"] = decode(bd_sql_connection1->Params->Values["Password"]);
			}
			bd_sql_connection->Password = bd_sql_connection1->Params->Values["Password"];
			bd_sql_connection->Database = bd_sql_connection1->Params->Values["Database"];
			bd_sql_connection->Username = bd_sql_connection1->Params->Values["User_Name"];
			bd_sql_connection->Server = bd_sql_connection1->Params->Values["HostName"];
			bd_sql_connection->Options->Charset = bd_sql_connection1->Params->Values["ServerCharSet"];
			//iSelRet = select(0, &readFDS, NULL, NULL, &timeVal);
			/*Client = WSASocket(AF_INET,
							SOCK_STREAM,
							IPPROTO_TCP,
							NULL,
							0,
							WSA_FLAG_OVERLAPPED);
			DWORD block = 1;

			bind(Client, (SOCKADDR *) &rmTCP, sizeof(rmTCP));
	FD_ZERO(&readFDS);
	FD_SET(Client, &readFDS);
	int iTimeOut = 5000; */
	/* set timeout values */
	/*timeVal.tv_sec  = iTimeOut / 1000;
	timeVal.tv_usec = iTimeOut % 1000;
	ioctlsocket(Client, FIONBIO, &block);
			int iSelRet = select(0, &readFDS, NULL, NULL, &timeVal);
	block = 0;

	if ((iSelRet != SOCKET_ERROR) && (iSelRet != 0)) { */
		/*iSockRet = recvfrom(icmpSock,              //socket
						   (char *)&recvpacket,    //buffer
						   iPacketSize,            //size of buffer
						   0,                      //flags
						   (SOCKADDR *)&source,    //source address
                           &iFromLen);              //pointer to address length
	//if socket timed out
    } else if (iSelRet == 0) {
        printf("timeout\n");
    } else if (iSelRet == SOCKET_ERROR) {
		printf("select() failed : %d\n", WSAGetLastError()); */
	IdTCPClient1->Connect();

	bd_sql_connection->Connect();
	/* if socket timed out */
	/*} else if (iSelRet == 0) {
		add_new_action_list("��������� ������ ��� �����������\\�������� ���������� � MySQL. ������: \"����������� ���������� � �������� ��\"", "", false, 1);
	} else if (iSelRet == SOCKET_ERROR) {
		add_new_action_list("��������� ������ ��� �����������\\�������� ���������� � MySQL. ������: \"����������� ���������� � �������� �� ������ \"", "", false, 1);
	}
	 ioctlsocket(Client, FIONBIO, &block);  */
	//else

			/*if (Client != INVALID_SOCKET) {
				if (connect (Client, (struct sockaddr *)&rmTCP,sizeof(rmTCP)) !=0)
				{
					Client = INVALID_SOCKET;
					add_new_action_list("��������� ������ ��� �����������\\�������� ���������� � MySQL. ������: \"����������� ���������� � �������� ��\"", "", false, 1);
				}
				else
				{
					bd_sql_connection->Connect();
				}
				shutdown(Client,4);
			}  */
			//ShowMessage("asd");
			//bd_sql_connection->Ping();
			//ShowMessage("asd1");
			//bd_sql_connection->ConnectionTimeout = 5;
			//ShowMessage("asd2");
			//bd_sql_connection->Connect();
			//ShowMessage("asd3");
			/*text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "�����������")));
			FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);
			Memo1->Lines->Add(text); */
		}
		catch (Exception *E)
		{
			if (test)
				MessageBox(NULL, ("����������� � �� ����������� ��������. ������ ����� ������: \"" + E->Message + "\". "
				"��������� ��� ��������� ��� ��� � ��������� �������. � ������ ������� ���������� � ������������� (���������� � ��� ��� "
				" � ���� ��������� �� ������ �� ������� \"� ���������\").").t_str(), "�������", MB_OK + MB_ICONERROR + MB_TASKMODAL);
			else
			  add_new_action_list("��������� ������ ��� �����������\\�������� ���������� � MySQL. ������: \"" + E->Message + "\"", "", false, 1);
				//throw E;
			result = false;
				//E->ThrowOuterException(E);
			//main_form->add_new_action_list("������: \"" + E->Message + "\" � �������� \"" + E->ClassName() + "\"", "");
		}
		if (test)
			bd_sql_connection->Connected = false;
	}
   /*	text = Format("[%s %s] %s.", ARRAYOFCONST((TimeToStr(Time()), FormatDateTime("dd.mm.yy", Date()), "���������")));
	FileWrite(id, (text + "\r\n").t_str(), text.Length() + 2);
	Memo1->Lines->Add(text);   */

	return result;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bd_conn_change_butClick(TObject *Sender)
{
  if (bd_sql_connection->Connected == false)
	  establish_connection(false);
  else
		//drop_connection();
		bd_sql_connection->Connected = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bd_sql_connection1AfterDisconnect(TObject *Sender)
{
	bd_connection_state->Caption = "�����������";
	add_new_action_list("���������� � �� ���������", "", false, 0);
	bd_connection_but->Caption = "����������";

	set_user_state("���-��������� �������");
}
void __fastcall TMainForm::exception_handler(TObject *Sender, Exception *E)

{
    //  case StrToInt(E.StackTrace) of
//    10060: begin
//            add_new_action_list('���������� ������������ � ��. �������� ������ �� MySQL �� ��������', sender.ToString);
//            bd_conn_change_but.Down := false;
//
//    end;
//  end;
  //MessageBox(Application.Handle, PChar('[' + TimeToStr(Time) + '] ������: "' + E.Message + '" � �������� "' + E.ClassName + '" '), '�������� ��������� ������', MB_OK + MB_ICONERROR + MB_TASKMODAL + MB_TOPMOST);
  add_new_action_list("��������� ����������� ������: \"" + E->Message + "\" � �������� \"" + E->ClassName() + "\" � �������� " + Sender->ClassName(), "", false, 2);
}
//---------------------------------------------------------------------------
void TMainForm::run_refresh_timer(byte action, int interval, TTime time)

//��������� ������� ������� �� ���������� ���� ���������� ������ �������������
//�������� action:
//0 - ������� �� ��������� ������, ������ ��������������
//1 - �������� � ������� � �����
//2 - �������� �� ���� � ���-�� �����
{
	WORD hh, mm, trash;
	switch (action)
	{
	  case 0:
		  full_refresh_timer->Enabled = false;
		  //StatusBar1->Panels->Items[1]->Text = "���������";
	  break;
	  case 1:
		  remaining_time = interval * 60;
		  elapsed_time = remaining_time;
		  full_refresh_timer->Enabled = true;
	  break;
	  case 2:
		DecodeTime(time, hh, mm, trash, trash);
		remaining_time = (interval-1) * 86400 + (hh * 3600) + (mm * 60);
		elapsed_time = remaining_time;
		full_refresh_timer->Enabled = true;
	  break;
	}
}
//---------------------------------------------------------------------------
void TMainForm::log_file_init(bool isfirstrun)
{
	String full_log_path;
	String curr_date;
	DateTimeToString (curr_date, "dd_mm_yy", Date());
	if (curr_date != last_init_date || isfirstrun) {
		if (!isfirstrun)
			FileClose(file_pos);
		full_log_path = plugin_data_path + "Logs\\monitoring_" + curr_date + ".log";
		if (FileExists(full_log_path))
		{
			file_pos = FileOpen(full_log_path, fmOpenReadWrite);
			FileSeek(file_pos, 0, 2);
		}
		else file_pos = FileCreate(full_log_path);
		last_init_date = curr_date;
	}
}
//---------------------------------------------------------------------------
void TMainForm::apply_config(byte action)
//��������� ���������� ������������-> ������������ ���������� ������������ �� ����� ��������->
//�������� action:
//0 - ������ ���������� �������
//1 - ���������� ������� �������� ������������� ������
//2 - ���������� ������� �������� ��

{
  TIniFile *inifile = new TIniFile(config_ini_path);
  switch (action)
  {
	case 0:
		tables_prefix = inifile->ReadString("BD Connection Properties", "Prefix", "");
		keep_conn = inifile->ReadInteger("BD Connection Properties", "KeepConnection", 0);
		visual_log = inifile->ReadBool("Additional Properties", "VisualLogging", true);
		file_log = inifile->ReadBool("Additional Properties", "FileLogging", true);
		/*full_refresh_timer->Enabled = false;
		run_refresh_timer(
		inifile->ReadInteger("Additional Properties", "FullAutoRefresh", 0),
		inifile->ReadInteger("Additional Properties", "RefreshInterval", 0),
		inifile->ReadTime("Additional Properties", "RefreshTime", Now()));*/
		break;
	case 1:
		tables_prefix = inifile->ReadString("BD Connection Properties", "Prefix", "");
		keep_conn = inifile->ReadInteger("BD Connection Properties", "KeepConnection", 0);
	   break;
	case 2:
		visual_log = inifile->ReadBool("Additional Properties", "VisualLogging", true);
		file_log = inifile->ReadBool("Additional Properties", "FileLogging", true);
		/*full_refresh_timer->Enabled = false;
		run_refresh_timer(
		inifile->ReadInteger("Additional Properties", "FullAutoRefresh", 0),
		inifile->ReadInteger("Additional Properties", "RefreshInterval", 0),
		inifile->ReadTime("Additional Properties", "RefreshTime", Now()));*/
	  break;
  }
  delete inifile;
}
//---------------------------------------------------------------------------

void TMainForm::load_first_config_run_form()
{
	first_config_form = new TFastConfigForm(NULL);
	first_config_form->pages->TabIndex = 0;
	first_config_form->ShowModal();
	delete first_config_form;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::Button7Click(TObject *Sender)
{
//    if (establish_connection(false) = true) then
//	  begin
//		FCommFortProcess(FdwPluginID, 42, nil, 0);
//		FCommFortProcess(FdwPluginID, 41, nil, 0);
//	  end;

}
//---------------------------------------------------------------------------




void TMainForm::progress_bar_change(Byte position, Byte state, String text)
//��������� ��������� �������, ������� � ������� �������� ����
//����������� �������� state:
//0 - pbsNormal
//1 - pbsError
//2 - pbsPaused
{
    //progress_bar_label.Caption := text;
	//showmessage(text);
	//general_progress_bar->Position = position;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	//��� ����������� ����� (���������� �������) ��������� ���� �����
	FileClose(file_pos);
	//FileClose(id);
	//����������� �� ��
	bd_sql_connection->Connected = false;
	//delete ConnectThread;
	WSACleanup();
	//������� ��������� ����������� � ��
	//delete bd_sql_connection;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	//Action = caHide;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::tray_iconClick(TObject *Sender)
{
	open_window_item->Click();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::clear_logs_butClick(TObject *Sender)
{
	states_list_rich->Lines->Clear();
	errors_state->Caption = "0";
	warnings_state->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::send_from_web_timerTimer(TObject *Sender)
{
	send_from_web_timer->Enabled = false;

	if (web_timer == 10) {
		next_web_public->Caption = 0;
		web_timer = 0;
		if (auto_log_clear && states_list_rich->Lines->Count >= 500)
		{
			clear_logs_but->Click();
			add_new_action_list("���������� ��� ������. ���������� ����� ��������� 500 ����", "", false, 0);
		}
		if (establish_connection(false))
		{
			int queries;
			String query;
			query.sprintf(L"SELECT * FROM `%s%s` ORDER BY id ASC;", tables_prefix.w_str(), table_mess_to_send_name);
			BYTE * bOutBuffer = NULL;
			try
			{
				TCustomDADataSet *result_set = bd_sql_connection->CreateDataSet();
				//bd_sql_connection->Execute(query, 0, &result_set);
				//bd_sql_connection->ExecSQL(query, NULL, NULL);
				result_set->SQL->Add(query);
				result_set->Open();
				if (!result_set->Eof)
				{
					DWORD len, pos_offset, rezim, body_len, channel_len, name_len;
					String channel, body;

					queries = 0;
					while (!result_set->Eof)
					{
						queries++;
						rezim = result_set->FieldByName("type")->AsInteger;
						channel = result_set->FieldByName("channel")->AsString;
						body.sprintf(L"[url=IP: %s]%s[/url]: %s",
								result_set->FieldByName("ip")->AsString,
								result_set->FieldByName("user")->AsString,
								result_set->FieldByName("body")->AsString
						);
					/*
						ID: 1020
						���� ������: �����(��� ������������ ������������) + �����(�����) + �����(�����) + �����(���������)
						len = 4 + bot_name_len * 2 + 4 + 4 + channel_len * 2 + 4 + body_len * 2
					*/
						body_len = body.Length();
						name_len = bot_name.Length();
						channel_len = active_channel.Length();
						len = 16 + (name_len + channel_len + body_len) * 2;

						bOutBuffer = new char[len];
						pos_offset = 0;

						set_stream_str (bOutBuffer, &pos_offset, bot_name, name_len);

						set_stream_str (bOutBuffer, &pos_offset, NULL, rezim);

						set_stream_str (bOutBuffer, &pos_offset, active_channel, channel_len);

						set_stream_str (bOutBuffer, &pos_offset, body, body_len);

						(*CommFortProcess)(dwPluginID, 1020, bOutBuffer, len);

						delete []bOutBuffer;
						bOutBuffer = NULL;

						result_set->Next();
					}

					add_new_action_list("�� ���-���������� ��������� � ��� " + IntToStr(queries) + " ���������", "", false, 0);
					bd_sql_connection->ExecSQL(
						String().sprintf(L"TRUNCATE TABLE `%s%s`;",
						tables_prefix.w_str(), table_mess_to_send_name), NULL, NULL
					);
				}
				result_set->Close();
			}
			catch(Exception *E)
			{
				main_form->add_new_action_list("��������� ������ �� ����� �������� ����� ��������� �� ����. ������: \"" + E->Message + "\"", "", false, 2);
			}
			if (bOutBuffer) {
				delete []bOutBuffer;
				bOutBuffer = NULL;
			}
			query.sprintf(L"SELECT * FROM `%s%s` WHERE auth = 0", tables_prefix.w_str(), table_web_users_name);
			/*try
			{
				bd_sql_connection->Execute(query, 0, &result_set);
				result_set->Open();
				if (!result_set->Eof)
				{
					queries = 0;
				   //	int ids[];
					while (!result_set->Eof)
					{
						virt_user_conn (
							result_set->FieldByName("nick")->AsString,
							result_set->FieldByName("ip")->AsString,
							result_set->FieldByName("pass")->AsString,
							(bool)result_set->FieldByName("male")->AsInteger
						);
					 //	ids[] = result_set->FieldByName("id")->AsInteger;
						result_set->Next();
					}
					add_new_action_list("�� ���-���������� ��������� � ��� " + IntToStr(queries) + " �������������", "", false, 0);
				  */	/*bd_sql_connection->ExecuteDirect(
						Format("UPDATE `%s%s` SET `auth`=:state WHERE `nick`=\"%s\";",
						ARRAYOFCONST((tables_prefix, table_web_users_name))
					)
					); */
			   /*	}
				result_set->Close();
			}
			catch(Exception *E)
			{
				main_form->add_new_action_list("��������� ������ �� ����� �������� ����� ������������� �� ����. ������: \"" + E->Message + "\"", "", false, 2);
			}  */
		}
	}
	else {
		next_web_public->Caption = 10 - web_timer;
		web_timer++;
	}
	/*if (auto_bd_clear)
	{
		if (bd_clear_timer >= 86400000) {
        	if (establish_connection(false))
			{
				add_new_action_list("�������������� ������� �� ���������", "", false, 0);
				bd_sql_connection->ExecuteDirect(
						Format("TRUNCATE TABLE `%s%s`;",
						ARRAYOFCONST((tables_prefix, table_mess_to_send_name))
					)
				);

            }
		}
		else bd_clear_timer++;
	} */
	send_from_web_timer->Enabled = true;
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::load_first_config_butClick(TObject *Sender)
{
	is_plugin_active = false;
	load_first_config_run_form();
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::online_users_butClick(TObject *Sender)
{
	get_users_list(active_channel);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bd_connection_butClick(TObject *Sender)
{

	if (!bd_sql_connection->Connected)
		establish_connection(false);
		//ConnectThread->FreeOnTerminate = true;
		//ConnectThread->c
		//ConnectThread->Start();    }
	else
		bd_sql_connection->Connected = false;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::open_log_butClick(TObject *Sender)
{
	if (open_log_but->Tag) {
		int bar_height = Height - ClientHeight;
		open_log_but->Caption = "������� ���";
		Height = 222 + bar_height;
		Constraints->MaxHeight = 222 + bar_height;
		states_list_rich->Visible = false;
		open_log_but->Tag = 0;
	}
	else {
		open_log_but->Caption = "������� ���";
		states_list_rich->Visible = true;
		Constraints->MaxHeight = 0;
		Height = 600;
		open_log_but->Tag = 1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::web_public_state_butClick(TObject *Sender)
//������ ������������ ���\����. ���������� ��������� � ����
{
	if (web_public_state_but->Tag) {
		web_public_state->Caption = "���������";
		web_public_state_but->Caption = "��������";
		next_web_public_label->Enabled = false;
		next_web_public->Enabled = false;
		next_web_public->Caption = "���������";
		send_from_web_timer->Enabled = false;
		web_timer = 0;
		web_public_state_but->Tag = 0;
	}
	else {
		web_public_state->Caption = "��������";
		web_public_state_but->Caption = "���������";
		next_web_public_label->Enabled = true;
		next_web_public->Enabled = true;
		next_web_public->Caption = "��������";
		send_from_web_timer->Enabled = true;
		web_public_state_but->Tag = 1;
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::plugin_state_butClick(TObject *Sender)
{
	if (is_plugin_active) {
		plugin_state_but->Caption = "��������";
		plugin_state->Caption = "��������";
		plugin_state_item->Caption = "�������� ������";
		online_users_state->Caption = "0";
		if (web_public_state_but->Tag)
			web_public_state_but->Click();
		web_public_state_label->Enabled = false;
		web_public_state->Enabled = false;
		web_public_state_but->Enabled = false;
		online_users_label->Enabled = false;
		online_users_state->Enabled = false;
		online_users_but->Enabled = false;
		virt_user_disconn (bot_name);
		is_plugin_active = 0;
	}
	else {
		plugin_state_but->Caption = "���������";
		plugin_state->Caption = "����������� ����...";
		plugin_state_item->Caption = "��������� ������";
		is_plugin_active = true;
		virt_user_conn (bot_name, bot_ip, bot_pass, bot_is_female);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::stop_plugin_butClick(TObject *Sender)
{
	int id = MessageBox(Handle, "�� ������������� ������ ��������� ������ �������?"
			" ��� ������� � ������ ��������� ��� ������.", "��������������",
			MB_YESNO + MB_ICONQUESTION + MB_APPLMODAL);
	if (id == IDYES)
		(*CommFortProcess)(dwPluginID, 2100, NULL, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::about_butClick(TObject *Sender)
{
	PluginShowAbout();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::settings_butClick(TObject *Sender)
{
	PluginShowOptions();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button123Click(TObject *Sender)
{
	/*if (states_list_rich->Visible) {
		states_list_rich->Visible = false;
	}
	else states_list_rich->Visible = true;*/
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::open_window_itemClick(TObject *Sender)
{
	if (Visible) {
		Close();
		open_window_item->Caption = "������� ����";
	}
	else {
		Show();
		open_window_item->Caption = "�������� ����";
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::settings_itemClick(TObject *Sender)
{
	PluginShowOptions();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::about_itemClick(TObject *Sender)
{
	PluginShowAbout();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::plugin_terminate_itemClick(TObject *Sender)
{
	int id = MessageBox(main_form->Handle, "�� ������������� ������ ��������� ������ �������?"
			" ��� ������� � ������ ��������� ��� ������.", "��������������",
			MB_YESNO + MB_ICONQUESTION + MB_APPLMODAL);
	if (id == IDYES)
		(*CommFortProcess)(dwPluginID, 2100, NULL, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::plugin_state_itemClick(TObject *Sender)
{
	plugin_state_but->Click();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	//ConnectThread = new TConnectThread(true);
	int bar_height = Height - ClientHeight;
	Constraints->MaxHeight = 222 + bar_height;
	Constraints->MinHeight = 222 + bar_height;
	//bd_sql_connection->SQLHourGlass = true;
	Application->OnException = exception_handler;

		Caption = plu_name + " " + plu_ver;
		tray_icon->Hint = Caption;

		//��������� ���� ����� ��� ������
		DateTimeToString (last_init_date, "dd_mm_yy", Date());
		log_file_init(true);
	 IdTCPClient1->Host = conn_address;
	//LongTimeFormat = "hh-mm-ss";
	//id = FileCreate(plugin_data_path + "debug_" + TimeToStr(Now()) + ".txt");
	WSADATA WSAData;
	//Client = INVALID_SOCKET;
	rmTCP.sin_family = AF_INET;
	rmTCP.sin_addr.s_addr = inet_addr(conn_address.t_str());
	rmTCP.sin_port = htons (3306);





	if (!WSAStartup(MAKEWORD(2,0), &WSAData))
	{
		int iSockRet, iSelRet;

	/* monitor for incomming connections */

		//Client = socket (AF_INET,SOCK_STREAM,0);
    }
	if (isfirstrun)
	{
		int id = MessageBox(main_form->Handle, "������ ������� ������� � ������� ������������ ���������. "
			"������ ��������� ������� ������� ��������� (� ��������� ������ ��������� "
			"���� ������ ���������)?", "������ ������", MB_YESNO + MB_ICONQUESTION + MB_APPLMODAL);
		if (id == IDYES)
			load_first_config_run_form();
		else PluginShowOptions();
	}
	else
	{
		if (auto_start)
			plugin_state_but->Click();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bd_sql_connectionConnectionLost(TObject *Sender, TComponent *Component,
		  TConnLostCause ConnLostCause, TRetryMode &RetryMode)
{
	bd_sql_connection->Connected = false;
	RetryMode = rmRaise;
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::IdTCPClient1Connected(TObject *Sender)
{
	IdTCPClient1->Disconnect();
}
//---------------------------------------------------------------------------

