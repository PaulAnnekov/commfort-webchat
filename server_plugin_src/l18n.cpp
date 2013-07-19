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


#pragma hdrstop

#include "l18n.h"
#include "notif.h"
#include <IniFiles.hpp>
#include <StrUtils.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

map<String, String> l18n;

void LoadLocalization(TMemIniFile *localization_file, String language) {
	TResourceStream *localization_resource = NULL;
	TStringList *localization_strings = NULL;
	TMemIniFile *default_localization_file = NULL;
	TStringList *sections = NULL;
	TStringList *keys = NULL;

	try {
		// Loading defaul localization form application resources.
		localization_resource = new TResourceStream((unsigned int)Instance, "en_lang", (System::WideChar*)RT_RCDATA);
		localization_strings = new TStringList();
		localization_strings->LoadFromStream(localization_resource);
		default_localization_file = new TMemIniFile("");
		default_localization_file->SetStrings(localization_strings);

		sections = new TStringList();
		keys = new TStringList();
		String key = "", value = "";
		default_localization_file->ReadSections(sections);
        l18n.empty();
		for (int i = 0; i < sections->Count; i++) {
			if (!sections->Strings[i].IsEmpty()) {
				default_localization_file->ReadSection(sections->Strings[i], keys);
				for (int j = 0; j < keys->Count; j++) {
					key = keys->Strings[j];
					if (!key.IsEmpty()) {
						if (language != "en" && localization_file->ValueExists(sections->Strings[i], key)) {
							value = localization_file->ReadString(sections->Strings[i], key, "");
						} else {
							value = default_localization_file->ReadString(sections->Strings[i], key, "");
						}
						value = StringReplace(value, "\\n", "\n", TReplaceFlags() << rfReplaceAll);
						l18n.insert(MapList::value_type(key, value));
					}
				}
			}
		}

	} catch(Exception *E) {
		if (localization_resource) {
			delete localization_resource;
		}
		if (localization_strings) {
			delete localization_strings;
		}
		if (default_localization_file) {
			delete localization_file;
		}
		if (sections) {
			delete sections;
		}
		if (keys) {
			delete keys;
		}
	}
}

void LoadDefaultLocalization() {
	TResourceStream *localization_resource = new TResourceStream((unsigned int)Instance, "en_lang", (System::WideChar*)RT_RCDATA);
	TStringList *localization_strings = new TStringList();
	localization_strings->LoadFromStream(localization_resource);
	TMemIniFile *localization_file = new TMemIniFile("");
	localization_file->SetStrings(localization_strings);
	delete localization_resource;
	delete localization_strings;
	delete localization_file;
}

void LoadLocalizationFromFile(String language) {
	TMemIniFile *localization_file = NULL;
	try {
		localization_file = new TMemIniFile(plugin_data_path + "\\languages\\" + language + ".ini");
		LoadLocalization(localization_file, language);
	} catch(Exception *E) {
    	InfoAction("Localization file load error: \"" + E->Message + "\"", false, 0, 1);
	}

	if (localization_file) {
		delete localization_file;
	}
}

// Get translation string.
String __(String key) {
	String value = "";

	MapList::iterator it = l18n.find(key);
	if (it != l18n.end()) {
		value = it->second;
	} else {
        value = key + " (untranslated)";
    }

	return value;
}

String UpdateComponentName(String name) {
	int pos = PosEx("_", name, 1);

	name[1] = UpperCase(name[1])[1];
	while (pos != 0) {
		name[pos + 1] = UpperCase(name[pos + 1])[1];
        name.Delete(pos, 1);
		pos = PosEx("_", name, pos);
	}

	return name;
}

void TranslateFormElements(TComponent *object) {
	String class_name;
	TForm *form = dynamic_cast<TForm *>(object);
	if (form) {
		form->Caption = __(UpdateComponentName(form->Name));
	}
	for (int i = 0; i < object->ComponentCount; i++) {
		class_name = object->Components[i]->ClassName();
		if (class_name == "TLabel") {
			TLabel *label = dynamic_cast<TLabel *>(object->Components[i]);
			if (label && !label->Caption.IsEmpty()) {
				label->Caption = __(UpdateComponentName(label->Name));
			}
		} else if (class_name == "TButton") {
			TButton *button = dynamic_cast<TButton *>(object->Components[i]);
			if (button && !button->Caption.IsEmpty()) {
				button->Caption = __(UpdateComponentName(button->Name));
			}
		}
	}
}
