// �r���h��� Winamp �̃v���O�C���t�H���_�ɃR�s�[������ WSH
// by nitoyon

debug = 0;

var objWshShell = WScript.CreateObject("WScript.Shell");
var objFS = WScript.CreateObject("Scripting.FileSystemObject")

// �p�X�ǂݎ��
strWinampDir = objWshShell.RegRead("HKCU\\Software\\Winamp\\");
if(debug)WScript.Echo(strWinampDir);

strPluginDir = strWinampDir + "\\Plugins\\";
if(debug)WScript.Echo(strPluginDir);

Arg = WScript.Arguments;
strTargetPath = Arg(0);
strTargetName = Arg(1);
if(debug)WScript.Echo(strTargetName);
if(debug)WScript.Echo(strTargetPath);

// �R�s�[
if(strTargetName == "gen_zipalbum")
{
	objFS.CopyFile(Arg(0), strPluginDir + "gen_zipalbum.dll");
}
else
{
	objFS.CopyFile(Arg(0), strPluginDir + "gen_zipalbum\\" + strTargetName + ".dll");
}
