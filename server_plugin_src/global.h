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


// ���������� ����������.

// ��������� � ���������� ������.
extern struct TABLE_NAMES_ST {
	String users_online; //�������� ������� ��� �������� ������ ������������� ������
	String actions;      //�������� ������� ��� �������� ���� �������� � ����
	//String mess_to_send; //�������� ������� ��� ��������� ��������� ��������
	//String settings;     //�������� ������� ��� �������� �������
	//String web_users;    //�������� ������� ��� ������ ������������� � ����
	String channels;     //�������� �� ������� ������������� �������
	String users;        //�������� ������� ��� ������ ������������������ �������������
	String user_channel; //�������� ������� ��� ������������� ������������ � ������� � ������� �� ���������
} tbl_names;

// ��������� � ������� ��� ���������� � ��.
extern struct DB_PROP_ST {
	String host; //����� ����� ��
	String user; //��� ������������ ��
	String pass; //������ ��
	String name; //��� ��
} db_prop;

// ��������� � ������� ��� ����������� ����.
extern struct BOT_PROP_ST {
	String name;    //��� ����
	String ip;      //ip ����
	String pass;    //������ ����
	bool is_female; //��� �������� ����?
} bot_prop;

extern int work_time_begin;  // ����� ������ ������ �������.

extern String tables_prefix,       // ������� ������
              chnls_line, 	       // ������ �������� ������.
			  plugin_data_path,    // ���� � ����� � ������� �������
			  data_storage,       // ���� � ����� � �������� ����������
			  web_storage,         // ���� � ����� � ���� � �������� ����������
              language,            // ���� ���������.
			  config_ini_path;     // ���� � ini ����� �������

extern const COLORREF color[3];

extern const String plugin_name,      // �������� �������.
					plugin_version,       // ������ �������.
					json_rpc_version; // ������ JSON-RPC.

extern bool visual_log,         //���������� ����������� � ���� ��������?
			file_log,           //����������� � ����� ��������?
			sync_enabled,       //������ ��������?
			auto_db_clear,      //������������� ������� ��?
			auto_log_clear,     //������������� ������� ���������� ���?
            show_work_time,     // ���������� ����� ������ �������?
			plugin_started,     //��� �� ��������������� ������? (������������ ��������� � �.�.)
			sync_autostart,     //�������� �� ������ ��� ���������?
			isfirstrun,         //������� �� ������� ������?
			channels_policy,    //�������� ������������ ������� - 0. ���, ����� �� ������, 1. ������ �� ������
			info_action_using,  //����, ����������� �� ��������� ������� ����. ������������ ��� ����������� ������� ���������� ��������� � ������� �����������
			no_white_ips,       // ��������� ��� ���������� ������� IP: ����� ������ � ������� ��� ����� ����� ������ � ����������
			log_notifications,  // Add notifications messages to log.
			check_version,      // Auto version check.
			auto_update;        // Auto update to newest version.

extern int remaining_time,      //���������� ������� �� ����������
		   elapsed_time,        //���������� ���������� ������� �� ����������
		   errors_c,            // ������� ������.
		   warnings_c;          // ������� ��������������.

extern unsigned main_thread_id; // Main thread ID.

extern TStringList *white_ips;     //������ ����� �������������, IP ������� �� ������������ � ����
				   //*condit_channels; //������ ������� ������� ���������\�������� ��� ������������ �����
				   //*watched_chnls; //������ ������������� ����� �������, ��� ��������� �������������� ������

extern TMainForm *main_form;

// DLL Handle;
extern HINSTANCE Instance;

extern CRITICAL_SECTION connections_manipulation_cs;
extern CRITICAL_SECTION users_manipulation_cs;
extern CRITICAL_SECTION channels_manipulation_cs;

// ������ �����.
typedef std::vector<String> StringList;

// ������ ����������=��������.
typedef std::map<String, String> MapList;

extern String SaveData(char *data, int data_length);
extern String LoadImage(String file_name);

extern String Base64BinaryEncode(void* databuf, unsigned int bufsize);

// ���������� ������ �� ������.
extern int GetStreamDword(BYTE **p, DWORD *len);
extern int GetStreamBinary(BYTE **p, char &data, int len);
extern int GetStreamString(BYTE **p, UnicodeString *var);
extern int GetStreamTime(BYTE **p, double &time);
extern int ShiftBytes(char **ptr, int len, int offset);

extern String implode(StringList array);

// ������� ���������� �������� �� ������.
extern void SetStreamString(BYTE *p, DWORD * offset, UnicodeString var, DWORD len);

// ����� ������ � ������ �����.
extern int ListSearch(TStringList *list, String line);
extern int ListSearch(String *list, String line, int count);
extern StringList::iterator ListSearch(StringList &list, String line);

extern String GetPluginFullName(String version = "");

extern String FormatError(int error_code);
extern String FormatError(int error_code, char *library);
// ������� ��� ����������� � ������������� �����.
//extern String Base64Encode(String line);
//extern String Base64Decode(String line);
extern String GetMD5(String line);

extern String get_ip(HWND hWindow);

// �������� �����, ��������� �� ��������� ������� �� �������� � ������� hh:mm:ss.
extern String get_time_offset(int beg_time);
//---------------------------------------------------------------------------
#endif

