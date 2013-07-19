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
#include <IdHashMessageDigest.hpp>

#include "global.h"
#include "main_form.h"
#include "webchat.h"
#include "notif.h"
#include "l18n.h"
#include "debug.h"
#include <fstream>

#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace std;

TABLE_NAMES_ST tbl_names = {"cf_users_online", "cf_actions", /* "cf_messages_to_send", "cf_settings", "cf_web_users", */
"cf_channels", "cf_users", "cf_user_channel"};

DB_PROP_ST db_prop;

BOT_PROP_ST bot_prop;

const COLORREF color[3] = {RGB(0, 0, 0), RGB(128, 128, 0), RGB(255, 0, 0)};

String tables_prefix = "",
       chnls_line = "",
	   plugin_data_path = "",
	   data_storage,
	   web_storage,
	   language,
	   config_ini_path,
	   log_file_path;

int work_time_begin;

TStringList * white_ips = NULL, *condit_channels = NULL;

TMainForm *main_form = NULL;

const String
  plugin_name = "CommFort WebChat",
  plugin_version  = "2.00",
  json_rpc_version = "2.0";

bool visual_log = false,         file_log,        auto_log_clear,
	 sync_enabled = false,   auto_db_clear,   no_white_ips = true,
	 plugin_started = false,     show_work_time,  sync_autostart,
	 info_action_using = false,  isfirstrun,      channels_policy,
     log_notifications = false, check_version = true, auto_update = false;

int remaining_time, elapsed_time, errors_c = 0, warnings_c = 0;
unsigned main_thread_id;
HINSTANCE Instance;

CRITICAL_SECTION connections_manipulation_cs, users_manipulation_cs, channels_manipulation_cs;

String GetCurrentUniqueName() {
	return GetMD5(GetTime().FormatString("ddddd hh:nn:ss.zzz"));
}

// Shift bytes to right using predefined offset.
// Returns new length.
int ShiftBytes(char **ptr, int len, int offset) {
	int new_len = len - offset;
	if (new_len > 0) {
		char *tmp = new char[new_len];
		memcpy(tmp, *ptr + offset, new_len);
		delete [] *ptr;
		*ptr = tmp;
	}

	return new_len;
}

String implode(StringList array) {
	String line = "";
	StringList::iterator value = NULL;
	StringList::iterator value_end = array.end();
	value_end--;
	for (value = array.begin(); value != array.end(); value++) {
		line += "\"" + (*value) + "\"";

		if (value != value_end) {
			line += ",";
		}
	}

	return line;
}

//---------------------------------------------------------------------------

int GetStreamTime(BYTE **p, double &time) {
	if (time) {
		time = *((double*)*p);
	}
	*p += 8;
	return 1;
}

String Base64BinaryEncode(void* databuf, unsigned int bufsize) {
	char alfBase64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	String rettext = "";

	int ibuf;
	char b64[5];
	char *cp = (char*) databuf;

	for (unsigned int ui = 0; ui < bufsize;) {
		memset(b64, 0x00, sizeof(b64));
		ibuf = (*cp & 0xFF) << 16;
		ui++;
		cp++;
		if (ui < bufsize) {
			ibuf |= (*cp & 0xFF) <<  8;
			ui++;
			cp++;
		}  else {
			b64[2] = b64[3] = '=';
		}
		if (ui < bufsize) {
			ibuf |= (*cp & 0xFF) <<  0;
			ui++;
			cp++;
		}  else {
			b64[3] = '=';
		}

		b64[0] = alfBase64[(ibuf >> 18) & 0x3F];
		b64[1] = alfBase64[(ibuf >> 12) & 0x3F];
		if (b64[2] == 0) {
			b64[2] = alfBase64[(ibuf >>  6) & 0x3F];
		}
		if (b64[3] == 0) {
			b64[3] = alfBase64[(ibuf >>  0) & 0x3F];
		}
		rettext += String(b64);
	}

	return rettext;
}

String SaveData(char *data, int data_length) {
	String file_name = GetCurrentUniqueName();
	String path;
	bool unique_file = false, write_error = false;
	int i = 2;

	try {
		while (i < 6) {
			path = data_storage + "\\" + file_name + ".jpg";
			if (FileExists(path)) {
				file_name += "_" + i;
			} else {
				unique_file = true;
				break;
			}
			i++;
		}
		if (!unique_file) {
			throw Exception(__("UniqueNameError"));
		}
		ofstream file(AnsiString(path).c_str(), ios::out | ios::binary);
		if (!file.is_open()) {
			throw Exception(__("FileOpenWriteError"));
		}
		file.write(data, data_length);
		if (file.fail()) {
			write_error = true;
		}
		file.close();
		if (write_error) {
			throw Exception(__("FileWriteError"));
		} else if (file.fail()) {
			throw Exception(__("FileCloseError"));
		}
	} catch(Exception *E) {
		InfoAction(Format(__("FileSaveError"), ARRAYOFCONST((E->Message))), false, 2, 1);
		file_name = "";
	} catch (...) {
		InfoAction(__("FileSaveUnexpectedError"), false, 2, 2);
		file_name = "";
	}

	return file_name;
}

String LoadImage(String file_name) {
	String data = "";
	String path = data_storage + "\\" + file_name + ".jpg";
	char *image = NULL;
	int size = 0;
	bool read_error = true;

	if (FileExists(path)) {
		try {
			ifstream file(AnsiString(path).c_str(), ios::in | ios::binary);

			if (!file.is_open()) {
				throw Exception(__("FileOpenReadError"));
			}

            // get length of file:
			file.seekg(0, ios::end);
			size = file.tellg();
			file.seekg(0, ios::beg);
			if (size > 0) {
				image = new char[size];

				file.read(image, size);

				if (!file.fail()) {
					data = Base64BinaryEncode(image, size);
					read_error = false;
				}
			}
			file.close();
			if (read_error) {
				throw Exception(__("FileReadError"));
			} else if (file.fail()) {
				throw Exception(__("FileCloseError"));
			}
		} catch(Exception *E) {
			InfoAction(Format(__("FileLoadError"), ARRAYOFCONST((E->Message))), false, 2, 1);
			data = "";
		} catch (...) {
			InfoAction(__("FileLoadUnexpectedError"), false, 2, 2);
			data = "";
		}
		if (image) {
            delete []image;
		}
	}

	return data;
}

//---------------------------------------------------------------------------

int GetStreamBinary(BYTE **p, char &data, int len) {
	if (data) {
		memcpy(&data, *p, len);
	}
	*p += len;

	return 1;
}

int GetStreamDword(BYTE **p, DWORD *len) {
	if (len) {
		*len = *((DWORD*)*p);
    }
	*p+=4;
	return 1;
}
// ---------------------------------------------------------------------------
int GetStreamString(BYTE **p, UnicodeString *var) {
	DWORD n;
	GetStreamDword(p, &n);
	if (var) {
		(*var).SetLength(n);
		memcpy((*var).c_str(), *p, n * 2);
	}
	*p += n * 2;
	return 1;
}
// ---------------------------------------------------------------------------
void SetStreamString(BYTE *p, DWORD * offset, UnicodeString var, DWORD len)
{
	memcpy(p + *offset, &len, 4);
	*offset += 4;
	if (var != NULL) {
		var.SetLength(len);
		memcpy(p + *offset, var.c_str(), len * 2);
		*offset += len * 2;
	}
}

// Поиск строки в списке строк TStringList.
// Возвращаемое значение - индекс найденного элемента или -1, если элемент не найден.
int ListSearch(TStringList *list, String line)
{
	int i = 0, result = -1;
	int count = list->Count;

	while (i != -1 && i < count) {
		if (list->Strings[i] == line) {
			result = i;
			i = -1;
		}
		else i++;
	}

	return result;
}

// Поиск строки в списке строк StringList.
// Возвращаемое значение - логическое.
StringList::iterator ListSearch(StringList &list, String line) {
	StringList::iterator item = NULL;
	bool is_found = false;

	if (!list.empty()) {
		for (item = list.begin(); item != list.end(); item++) {
			if (*item == line) {
				is_found = true;
				break;
			}
		}
		if (!is_found) {
			item = NULL;
		}
	}

	return item;
}

// Поиск строки в списке строк String[...]. count - количество элементов динамического массива list.
// Возвращаемое значение - индекс найденного элемента или -1, если элемент не найден.
int ListSearch(String *list, String line, int count)
{
	int i = 0, result = -1;

	while (i != -1 && i < count) {
		if (list[i] == line) {
			result = i;
			i = -1;
		}
		else i++;
	}

	return result;
}

String GetPluginFullName(String version) {
	return plugin_name + " " + ((version.IsEmpty()) ? plugin_version : version);
}

// Get WinAPI error message.
String FormatError(int error_code, char *library) {
	char *message;
	DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	if (library) {
		flags |= FORMAT_MESSAGE_FROM_HMODULE;
	}
	HANDLE module_handle = NULL;
	if (library) {
		module_handle = GetModuleHandle(_T(library));
	}
	FormatMessage(
		flags,
		module_handle,
		error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &message,
		0, NULL
	);

	String error = String(message) + " (" + error_code + ")";
	LocalFree(message);

	return error;
}

// Get WinAPI error message.
String FormatError(int error_code) {
	return FormatError(error_code, NULL);
}

// ---------------------------------------------------------------------------
String get_ip(HWND hWindow)
{
	String ip;
	DWORD long_ip;
	SendMessage(hWindow, IPM_GETADDRESS, 0, (LPARAM)(LPDWORD)&long_ip);

	ip = (String)FIRST_IPADDRESS((LPARAM)long_ip) + "." +
		(String)SECOND_IPADDRESS((LPARAM)long_ip) + "." +
		(String)THIRD_IPADDRESS((LPARAM)long_ip) + "." +
		(String)FOURTH_IPADDRESS((LPARAM)long_ip);
	return ip;
}
// ---------------------------------------------------------------------------
//void Base64BinaryEncode(char *data, String &encoded_data) {
//	TIdEncoderMIME *Base64Encode = new TIdEncoderMIME(Application);
//
//	try {
//		encoded_data = Base64Encode->EncodeBytes(;
//	} __finally {
//		delete Base64Encode;
//	}
//}
// ---------------------------------------------------------------------------
String Base64Encode(String line)
{
	TIdEncoderMIME *Base64Encode = new TIdEncoderMIME(Application);
	try {
		line = Base64Encode->EncodeString(line, TEncoding::UTF8);
	} __finally {
		delete Base64Encode;
	}

	return line;
}
// ---------------------------------------------------------------------------
String Base64Decode(String line)
{
	TIdDecoderMIME *Base64Decode = new TIdDecoderMIME(Application);
	try
	{
		line = Base64Decode->DecodeString(line);
    }
    __finally
	{
		delete Base64Decode;
	}

	return line;
}
// ---------------------------------------------------------------------------
String GetMD5(String line) {
	String result;
	TIdHashMessageDigest5 *idmd5 = new TIdHashMessageDigest5();
	try {
		result = idmd5->HashStringAsHex(line);
	}
	__finally {
		delete idmd5;
	}

	return result;
}
// ---------------------------------------------------------------------------
String get_time_offset(int beg_time) {
    int work_time_curr = time(NULL);
	int time = work_time_curr - beg_time;
    int hh, mm, ss;

	hh = time / 3600;
	mm = (time - hh * 3600) / 60;
	ss = time - hh * 3600 - mm * 60;

	return Format("%.2d:%.2d:%.2d", ARRAYOFCONST((hh, mm, ss)));
}
// ---------------------------------------------------------------------------


//String url_decode(String source) {
//  int len = source.Length();
//  int offset = 0;
//  String dest = "";
//  char * start = dest;
//
//  while (offset < len) {
//	switch (source[offset]) {
//	case '+':
//	  dest += " ";
//	  break;
//	case '%':
//	  if (source[offset + 1] && source[offset + 2]) {
//		int value = HexPairValue(source + 1);
//		if (value >= 0) {
//		  *(dest++) = value;
//		  source += 2;
//        }
//		else {
//          *dest++ = '?';
//		}
//      }
//	  else {
//        *dest++ = '?';
//	  }
//	  break;
//    default:
//	  *dest++ = *source;
//	}
//	source++;
//  }
//
//  *dest = 0;
//  return dest - start;
//}
