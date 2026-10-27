#define MyAppName "ORYMY Bass"
#define MyAppVersion "1.0.1"
#define MyPublisher "YMY Sounds VST"

[Setup]
AppId={{A6F3E4A1-6B37-4A7D-9C02-0A6D9B551000}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyPublisher}
DefaultDirName={autopf}\YMY Sounds VST\ORYMY Bass
DefaultGroupName={#MyAppName}
OutputDir=output
OutputBaseFilename=ORYMY-Bass-Setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=admin
UninstallDisplayIcon={app}\ORYMY Bass.exe

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Dirs]
Name: "{commoncf}\VST3\ORYMY Bass.vst3\Contents\Resources"
Name: "{commoncf}\VST3\ORYMY Bass.vst3\Contents\x86_64-win"

[Files]
; Install the VST3 bundle using explicit paths so the Contents hierarchy cannot be flattened.
Source: "build\OrymyBass_artefacts\Release\VST3\ORYMY Bass.vst3\Contents\Resources\moduleinfo.json"; DestDir: "{commoncf}\VST3\ORYMY Bass.vst3\Contents\Resources"; Flags: ignoreversion
Source: "build\OrymyBass_artefacts\Release\VST3\ORYMY Bass.vst3\Contents\x86_64-win\ORYMY Bass.vst3"; DestDir: "{commoncf}\VST3\ORYMY Bass.vst3\Contents\x86_64-win"; Flags: ignoreversion
Source: "build\OrymyBass_artefacts\Release\Standalone\ORYMY Bass.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\ORYMY Bass"; Filename: "{app}\ORYMY Bass.exe"
Name: "{autodesktop}\ORYMY Bass"; Filename: "{app}\ORYMY Bass.exe"; Tasks: desktopicon
