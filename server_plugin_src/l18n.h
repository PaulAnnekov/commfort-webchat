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

#ifndef l18nH
#define l18nH
#include <map>
#include "global.h"
//---------------------------------------------------------------------------
using namespace std;

extern MapList l18n;
extern void LoadDefaultLocalization();
extern void LoadLocalizationFromFile(String language);
extern String __(String key);
extern void TranslateFormElements(TComponent *object);
#endif
