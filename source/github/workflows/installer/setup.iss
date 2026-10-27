[Setup]
AppName=ORYMY Bass
AppVersion=1.0.0
DefaultDirName={pf}\YMY Sounds VST\ORYMY Bass
DefaultGroupName=ORYMY Bass
OutputDir=..\installer_output
OutputBaseFilename=ORYMY-Bass-Setup
Compression=lzma
SolidCompression=yes

[Files]
Source: "..\build\OrymyBass_artefacts\Release\VST3\ORYMY Bass.vst3"; DestDir: "{commoncf}\VST3\YMY Sounds VST"; Flags: ignoreversion recursesubdirs
Source: "..\build\OrymyBass_artefacts\Release\Standalone\ORYMY Bass.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\ORYMY Bass"; Filename: "{app}\ORYMY Bass.exe"
Name: "{commondesktop}\ORYMY Bass"; Filename: "{app}\ORYMY Bass.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Crear acceso directo en el escritorio"; GroupDescription: "Adicionales:"
