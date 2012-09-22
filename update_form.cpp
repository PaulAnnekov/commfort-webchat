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

#include <vcl.h>
#pragma hdrstop

#include "update_form.h"
#include "l18n.h"
#include "notif.h"
#include "global.h"
#include "debug.h"
#include <process.h>
#include <math.h>
#include <IniFiles.hpp>
#include <wininet.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUpdateForm *update_form = NULL;
HANDLE download_thread;
int file_size;
volatile long download_stop;
bool no_error = true, downloading = false;
String new_version, new_version_url, download_target, last_error, new_version_save_path;
//TIdHTTP *connection = NULL;
String get = "http://www.steelrat.info/?target=cf_webchat&version=" + plugin_version + "&download=cf_webchat_";
TStringStream *download_data = NULL;
//---------------------------------------------------------------------------
__fastcall TUpdateForm::TUpdateForm(TComponent* Owner)
	: TForm(Owner)
{
}

int TUpdateForm::UFControl(TMessage &Message) {
	switch (Message.WParam) {
		case 0:
			file_size = ceil(Message.LParam / 1024.0);
			download_progress->Position = 0;
			download_progress->Max = Message.LParam;
			downloading_state_label->Caption = Format("0 / %d", ARRAYOFCONST((file_size)));
		break;
		case 1:
			download_progress->Position = Message.LParam;
			downloading_state_label->Caption = Format("%d / %d Κα", ARRAYOFCONST(((int) ceil(Message.LParam / 1024.0), file_size)));
		break;
		case 2:
			downloading_state_label->Caption = "";
		break;
        case 4:
			downloading_state_label->Caption = "";
			download_progress->Position = 0;
		break;
	}

	return 1;
}

//---------------------------------------------------------------------------
void __fastcall TUpdateForm::FormCreate(TObject *Sender)
{
	TranslateFormElements(this);
	current_version_label->Caption = Format(__("CurrentVersionLabel"), ARRAYOFCONST((plugin_version)));
}
//---------------------------------------------------------------------------

int GetInetFileSize(HANDLE h_file) {
	int size = 0;
	DWORD buf_size = 4;
	if (!HttpQueryInfo(h_file, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &size, &buf_size, NULL)) {
		size = 0;
	}

	return size;
}

int GetInternetFile(String path, BYTE **file) {
	HINTERNET h_session, h_file;
	int size = 0;
	BYTE *buffer = NULL;

	try {
		h_session = InternetOpen("CommFort WebChat Version Checker", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
		if (h_session == 0) {
			throw Exception(Format(__("InternetUseError"), ARRAYOFCONST((FormatError(GetLastError(), "wininet.dll")))));
		}

		h_file = InternetOpenUrl(h_session, AnsiString(path).c_str(), 0, 0, INTERNET_FLAG_RELOAD, 0);

		if (h_file == 0) {
			throw Exception(Format(__("InternetFileOpenError"), ARRAYOFCONST((FormatError(GetLastError(), "wininet.dll")))));
		}
		size = GetInetFileSize(h_file);
		if (size == 0) {
			throw Exception(Format(__("InternetFileSizeError"), ARRAYOFCONST((FormatError(GetLastError(), "wininet.dll")))));
		}
		if (update_form) {
			SendMessage(update_form->Handle, UF_CONTROL, 0, size);
		}
		*file = new BYTE[size];
		buffer = new BYTE[1024];
		DWORD data_read, current_read = 0;
		do {
			if (!InternetReadFile (h_file, buffer, 1024,  &data_read) ) {
				throw Exception(Format(__("InternetFileDownloadingError"), ARRAYOFCONST((FormatError(GetLastError(), "wininet.dll")))));
			}

			// Condition of dwSize=0 indicate EOF. Stop.
			if (data_read) {
				memcpy(*file + current_read, buffer, data_read);
				current_read += data_read;
				if (update_form) {
					SendMessage(update_form->Handle, UF_CONTROL, 1, current_read);
				}
			}
		} while (data_read && !download_stop);
		if (update_form) {
			SendMessage(update_form->Handle, UF_CONTROL, (download_stop) ? 4 : 2, 0);
		}
	} catch (Exception *E) {
		last_error = E->Message;
	} catch (...) {
		last_error = __("FileDownloadUnexpectedError");
	}

	InternetCloseHandle(h_file);
	InternetCloseHandle(h_session);

    if (buffer) {
        delete []buffer;
	}

	return size;
}

unsigned __stdcall DownloadThread(void* pArguments) {
	BYTE *file = NULL;
	try {
		int size = GetInternetFile(download_target, &file);
		if (size > 0 && !download_stop) {
			download_data->Write(file, size);
		}
	} catch(Exception *E) {
		last_error = Format(__("FileDownloadThreadError"), ARRAYOFCONST((E->Message)));
	} catch(...) {
		last_error = __("FileDownloadThreadUnexpectedError");
	}

	if (file) {
    	delete []file;
	}

	if (main_form) {
		PostMessage(main_form->Handle, MF_DO_ACTION, 1, (LPARAM)pArguments);
	}

	_endthreadex(0);

	return 0;
}

void DownloadFileInit() {
	last_error = "";
	download_stop = 0;
	downloading = true;
	download_data = new TStringStream("", TEncoding::UTF8, true);
}

void DownloadFileEnd() {
	downloading = false;
	if (download_data) {
		delete download_data;
	}
}

void DownloadFile(void(*callback)(), String path) {
	DownloadFileInit();
	download_target = path;
	download_thread = (HANDLE)_beginthreadex(NULL, 0, &DownloadThread, callback, 0, NULL);
}

void VersionInfoDownloaded() {
	if (last_error.IsEmpty()) {
        if (update_form) {
			update_form->checking_state_label->Caption = __("VersionInfoChecked");
		}
		if (download_data && !download_data->DataString.IsEmpty()) {
			TStringList *version_info = new TStringList();
			version_info->Text = download_data->DataString;
			new_version = version_info->Strings[0];
			if (!new_version.IsEmpty() && new_version != plugin_version) {
				new_version_url = version_info->Strings[1];
				version_info->Delete(0);
				version_info->Delete(0);
				if (main_form) {
					main_form->version_label->Hint = Format(__("NewVersionAvailableHint"), ARRAYOFCONST((new_version)));
					main_form->version_label->Caption = plugin_version + " (!)";
				}
				if (update_form) {
					update_form->new_version_label->Caption = Format(__("NewVersionInfo"), ARRAYOFCONST((new_version)));
					update_form->new_version_info_label->Caption = version_info->Text;
					update_form->update_plugin_button->Enabled = true;
				}
				if (auto_update) {
                    DownloadFileEnd();
					GetNewVersion();
				}
			} else {
				if (main_form) {
					main_form->version_label->Hint = __("CheckNewVersionHint");
					main_form->version_label->Caption = plugin_version;
				}
				if (update_form) {
					update_form->new_version_label->Caption = __("LatestVersionInstalled");
				}
			}
			delete version_info;
		}
	} else {
		bool show = (update_form) ? true : false;
		if (update_form) {
			update_form->checking_state_label->Caption = __("VersionCheckerStateError");
		}
		InfoAction(Format(__("VersionCheckError"), ARRAYOFCONST((last_error))), show, 0, 2);
	}
	if (!auto_update) {
		DownloadFileEnd();
		if (main_form) {
			main_form->version_label->Enabled = true;
		}
	}
}

void NewVersionDownloaded() {
	bool error = false;

	if (last_error.IsEmpty() && !download_stop) {
		if (update_form) {
			update_form->checking_state_label->Caption = __("NewVersionDownloaded");
		}
		try {
			download_data->SaveToFile(new_version_save_path);
			ShowMsg(update_form->Handle, Format(__("NewVersionRunTip"), ARRAYOFCONST((GetPluginFullName(), GetPluginFullName(new_version)))), 0);
			if (main_form) {
				main_form->version_label->Hint = Format(__("NewVersionDownloadedHint"), ARRAYOFCONST((new_version)));
            }
		} catch (Exception *E) {
			InfoAction(Format(__("NewFileSaveError"), ARRAYOFCONST((E->Message))), (update_form) ? true : false, 0, 2);
			error = true;
		} catch (...) {
			InfoAction(__("NewFileSaveUnexpectedError"), (update_form) ? true : false, 0, 2);
			error = true;
		}
		if (main_form && error) {
			main_form->version_label->Hint = __("NewVersionFileSaveError");
		}
	} else if (!last_error.IsEmpty()) {
		if (update_form) {
			update_form->checking_state_label->Caption = __("VersionCheckerStateError");
		}
		if (main_form) {
			main_form->version_label->Hint = __("VersionCheckerStateError");
		}
		InfoAction(Format(__("NewVersionDownloadError"), ARRAYOFCONST((last_error))), (update_form) ? true : false, 0, 2);
	} else {
		if (update_form) {
			update_form->checking_state_label->Caption = __("DownloadStopped");
		}
		if (main_form) {
			main_form->version_label->Hint = __("CheckNewVersionHint");
		}
	}

	if (main_form) {
		main_form->version_label->Enabled = true;
	}

    if (update_form) {
		update_form->update_plugin_button->Enabled = true;
		update_form->update_plugin_button->Caption = __("UpdatePluginButton");
	}

	DownloadFileEnd();
}

void __fastcall TUpdateForm::update_plugin_buttonClick(TObject *Sender)
{
 	GetNewVersion();
}

void GetNewVersion() {
	if (!downloading) {
		new_version_save_path = GetCurrentDir() + "\\Plugins\\cf_webchat_" + new_version_url + ".cfplug";
		if (!FileExists(new_version_save_path)) {
			if (update_form) {
				update_form->checking_state_label->Caption = __("DownloadingNewVersion") + "...";
				update_form->update_plugin_button->Caption = __("UpdateStopButton");
			}
			if (main_form) {
				main_form->version_label->Hint = __("DownloadingNewVersion") + "...";
				main_form->version_label->Enabled = false;
			}
			DownloadFile(&NewVersionDownloaded, get + new_version_url);
		} else {
			if (update_form) {
				update_form->checking_state_label->Caption = __("NewVersionDownloadedLater");
				ShowMsg(update_form->Handle, Format(__("NewVersionRunTip"), ARRAYOFCONST((GetPluginFullName(), GetPluginFullName(new_version)))), 0);
			}
		}
	} else {
		if (update_form) {
			update_form->update_plugin_button->Enabled = false;
		}
		InterlockedIncrement(&download_stop);
	}
}

void GetVersionInfo() {
	if (!downloading) {
		if (main_form) {
			main_form->version_label->Hint = __("GettingNewVersionInfo") + "...";
			main_form->version_label->Enabled = false;
		}
		if (update_form) {
			update_form->checking_state_label->Caption = __("CheckingNewVersion") + "...";
		}
		DownloadFile(&VersionInfoDownloaded, get + "current_version");
	}
}

void __fastcall TUpdateForm::FormShow(TObject *Sender)
{
	GetVersionInfo();
}
//---------------------------------------------------------------------------


