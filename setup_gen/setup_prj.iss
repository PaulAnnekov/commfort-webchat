[Files]
Source: PluginsData\WebChat\*; DestDir: {commonappdata}\CommFort_server\PluginsData\WebChat; Flags: onlyifdoesntexist uninsneveruninstall recursesubdirs createallsubdirs
Source: system\WindowsCodecs.dll; DestDir: {sys}; Flags: uninsneveruninstall onlyifdoesntexist
Source: commfort_server\*; DestDir: {app}; Flags: recursesubdirs createallsubdirs
[Setup]
//HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\CommFort server_is1,InstallLocation=
DefaultDirName={reg:HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\CommFort server_is1,InstallLocation|{pf}\CommFort_server}
OutputDir=e:\projects\webchat\
SourceDir=e:\projects\webchat\setup
VersionInfoVersion=2.00
VersionInfoCompany=SteelRat Productions
VersionInfoDescription=WebChat for CommFort 5
VersionInfoTextVersion=2.00
VersionInfoCopyright=SteelRat Productions
VersionInfoProductName=CommFort WebChat
VersionInfoProductVersion=2.00
AppCopyright=SteelRat Productions
AppName=CommFort WebChat
AppVerName=CommFort WebChat 2.00
AppPublisher=SteelRat Productions
AppPublisherURL=http://steelrat.info/
AppSupportURL=http://steelrat.info/webchat/
AppUpdatesURL=http://steelrat.info/webchat/
AppVersion=2.00
UninstallDisplayIcon=e:\projects\webchat\setup\Uninstall.ico
UninstallDisplayName=CommFort WebChat
OutputBaseFilename=webchat_cf_5_setup
InfoBeforeFile=CHANGES.txt
EnableDirDoesntExistWarning=false
DisableProgramGroupPage=true
Compression=lzma/ultra
InternalCompressLevel=ultra
SetupIconFile=Install.ico
WizardImageBackColor=clWhite
WizardImageFile=e:\projects\webchat\setup\wizard_img.bmp
WizardSmallImageFile=e:\projects\webchat\setup\wizard_icon.bmp
WizardImageStretch=false
DirExistsWarning=no
AppComments=Uninstall CommFort WebChat plugin
AppContact=http://steelrat.info/contacts/
AppSupportPhone=+380994914594
[Languages]
Name: Russian; MessagesFile: compiler:Languages\Russian.isl
Name: English; MessagesFile: compiler:Default.isl
Name: German; MessagesFile: compiler:Languages\German.isl
Name: Italian; MessagesFile: compiler:Languages\Italian.isl
[_ISToolPostCompile]
Name: e:\projects\webchat\zip.exe; Parameters: -j e:\projects\webchat\webchat_cf_5.zip e:\projects\webchat\webchat_cf_5_setup.exe e:\projects\webchat\setup\CHANGES.txt; Flags: abortonerror
Name: e:\projects\webchat\setup\make-tar.bat; Parameters: ; Flags: abortonerror
