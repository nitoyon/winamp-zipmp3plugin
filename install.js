// ビルド後に Winamp のプラグインフォルダにコピーをする WSH
// by nitoyon

debug = 0;

var objWshShell = WScript.CreateObject("WScript.Shell");
var objFS = WScript.CreateObject("Scripting.FileSystemObject")

// パス読み取り
strWinampDir = objWshShell.RegRead("HKCU\\Software\\Winamp\\");
if(debug)WScript.Echo(strWinampDir);

strPluginDir = strWinampDir + "\\Plugins\\";
if(debug)WScript.Echo(strPluginDir);

Arg = WScript.Arguments;
strTargetPath = Arg(0);
strTargetName = Arg(1);
if(debug)WScript.Echo(strTargetName);
if(debug)WScript.Echo(strTargetPath);

// コピー
if(strTargetName == "gen_zipalbum")
{
	objFS.CopyFile(Arg(0), strPluginDir + "gen_zipalbum.dll");
}
else
{
	objFS.CopyFile(Arg(0), strPluginDir + "gen_zipalbum\\" + strTargetName + ".dll");
}
