# Microsoft Developer Studio Project File - Name="zipmp3_plugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=zipmp3_plugin - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "zipmp3_plugin.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "zipmp3_plugin.mak" CFG="zipmp3_plugin - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "zipmp3_plugin - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "zipmp3_plugin - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zipmp3_plugin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZIPMP3_PLUGIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZIPMP3_PLUGIN_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /dll /machine:I386 /out:"N:\soft\Media\winamp\Plugins\gen_zipalbum.dll"

!ELSEIF  "$(CFG)" == "zipmp3_plugin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZIPMP3_PLUGIN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZIPMP3_PLUGIN_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\Winamp\Plugins\gen_zipalbum.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "zipmp3_plugin - Win32 Release"
# Name "zipmp3_plugin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ArchiveFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\CueFile.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Id3tagv2.cpp
# End Source File
# Begin Source File

SOURCE=.\ListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Mp3File.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\WinampHook.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ArchiveFile.h
# End Source File
# Begin Source File

SOURCE=.\Controller.h
# End Source File
# Begin Source File

SOURCE=.\CueFile.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\frontend.h
# End Source File
# Begin Source File

SOURCE=.\GEN.H
# End Source File
# Begin Source File

SOURCE=.\Id3tagv2.h
# End Source File
# Begin Source File

SOURCE=.\include.h
# End Source File
# Begin Source File

SOURCE=.\ListWnd.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\MainWnd.h
# End Source File
# Begin Source File

SOURCE=.\Mp3File.h
# End Source File
# Begin Source File

SOURCE=.\MsgMap.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vis.h
# End Source File
# Begin Source File

SOURCE=.\WinampHook.h
# End Source File
# Begin Source File

SOURCE=.\zipmp3.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\dll.ico
# End Source File
# Begin Source File

SOURCE=.\expert.ico
# End Source File
# Begin Source File

SOURCE=.\expert1.ico
# End Source File
# Begin Source File

SOURCE=.\font.ico
# End Source File
# Begin Source File

SOURCE=.\resource\hand.cur
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\list.ico
# End Source File
# Begin Source File

SOURCE=.\list1.ico
# End Source File
# Begin Source File

SOURCE=.\mp3.ico
# End Source File
# Begin Source File

SOURCE=.\mp4.ico
# End Source File
# Begin Source File

SOURCE=.\mp5.ico
# End Source File
# Begin Source File

SOURCE=.\resource\nocccd1.bmp
# End Source File
# Begin Source File

SOURCE=.\resource\nocccd2.bmp
# End Source File
# Begin Source File

SOURCE=.\path.ico
# End Source File
# Begin Source File

SOURCE=.\resource\Pledit.bmp
# End Source File
# Begin Source File

SOURCE=.\resource\Posbar.bmp
# End Source File
# Begin Source File

SOURCE=.\skin.ico
# End Source File
# Begin Source File

SOURCE=.\resource\Text.bmp
# End Source File
# Begin Source File

SOURCE=.\window.ico
# End Source File
# Begin Source File

SOURCE=.\window1.ico
# End Source File
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dialog\ChildDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\ChildDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\FontDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\FontDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\ListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\Mp3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\NullDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\WindowDlg.cpp
# End Source File
# End Group
# End Target
# End Project
