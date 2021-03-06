; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "BB Notifier Server"
#define MyAppVersion "1.0.1"
#define MyAppPublisher "Green Snake Software"
#define MyAppURL "http://www.gss.freeiz.com"
#define MyAppExeName "BBNotifierServer.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{53637051-6CC1-467E-9EBF-CA3D2C783541}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\BBNotifierServer
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=..\CallNotifierServer\LICENSE.txt
OutputBaseFilename=setup
SetupIconFile=..\CallNotifierServer\Images\RedIco.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "xpCompatible"; Description: "XP+ Compatible version"; GroupDescription: "Versions"; Flags: exclusive
Name: "win7Compatible"; Description: "Windows 7+ compatible version (you'll need MSVC2013 x86 redistributable)"; GroupDescription: "Versions"; Flags: exclusive

[Files]
Source: "..\build-CallNotifierServer-QT5_3_2_MSVC_Static-Release\release\BBNotifierServer.exe"; DestDir: "{app}"; Flags: ignoreversion; Tasks: win7Compatible
Source: "..\build-CallNotifierServer-QT_5_3_2_Static-Release\release\BBNotifierServer.exe"; DestDir: "{app}"; Flags: ignoreversion; Tasks: xpCompatible
;Source: "..\CallNotifierServer\Sounds\Call.mp3 "; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usPostUninstall then
  begin
        RegDeleteValue(HKEY_CURRENT_USER, 'Software\Microsoft\Windows\CurrentVersion\Run', 'BBNotifier');
  end;
end;

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

