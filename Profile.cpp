
// Profile.cpp
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Profile.h"
#include "util\uProfile.h"
#include "util\uPath.h"


/******************************************************************************/
//		��`
/******************************************************************************/

string	Profile::strPath = "" ;

// �Q�Ɨp
HINSTANCE	Profile::hInstance = 0 ;
string		Profile::strWinampIniPath = "" ;
string		Profile::strPluginDir;

// �Đ����@
int	Profile::intRepeat;

// �E�C���h�E
BOOL	Profile::blnShowOnlyArchive;
BOOL	Profile::blnShowOnlyUncompressed;
BOOL	Profile::blnAttachToWinamp;
BOOL	Profile::blnUseTimebar;
int	Profile::intTransparency;

// �t�H���g
string	Profile::strListFont;
int	Profile::intListFontSize;
string	Profile::strCollapseFont;
int	Profile::intCollapseFontSize;
BOOL	Profile::blnUseSkinFont;

// �X�L��
int	Profile::intSkin1;
int	Profile::intSkin2;
string	Profile::strSkinDir1;
string	Profile::strSkinDir2;

// MP3
BOOL	Profile::blnUseId3v2;
BOOL	Profile::blnUseCue;
BOOL	Profile::blnUseMp3Cue;

// ���X�g
string	Profile::strListNormal ;
string	Profile::strListID3 ;
string	Profile::strListCompilation ;
BOOL	Profile::blnListID3 ;
BOOL	Profile::blnListCompilation ;

// DLL
vector<string>	Profile::vecHeaderDll;
vector<BOOL>	Profile::vecUseHeaderDll;

// �ꏊ
int	Profile::intX = 0 ;
int	Profile::intY = 0 ;
int	Profile::intBlockX = 0 ;
int	Profile::intBlockY = 0 ;
BOOL	Profile::blnCountUp;
BOOL	Profile::blnCompact;


/******************************************************************************/
//		�ۑ��E�ǂ݂Ƃ�
/******************************************************************************/
// �ۑ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Save()
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH + 1] ;
		GetModuleFileName( hInstance, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPluginDir = GetDirName(strPath);
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		GetModuleFileName( GetModuleHandle( NULL), pszPath, MAX_PATH) ;
		strWinampIniPath = pszPath ;
		strWinampIniPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;
	}
	const char* pszFile = strPath.c_str() ;

	// �E�C���h�E
	WriteIniBln("window", "ShowOnlyArchive", 		blnShowOnlyArchive, 		strPath) ;
	WriteIniBln("window", "ShowOnlyUncompressed", 	blnShowOnlyUncompressed, 	strPath) ;
	WriteIniBln("window", "AttachToWinamp", 		blnAttachToWinamp, 		strPath) ;
	WriteIniBln("window", "UseTimebar", 		blnUseTimebar, 			strPath) ;
	WriteIniInt("window", "Transparency", 		intTransparency, 		strPath) ;

	// �Đ����@
	WriteIniInt("play", "repeat", 		intRepeat, 			strPath) ;

	// �t�H���g
	WriteIniStr("font", "ListFont",		strListFont, 		strPath);
	WriteIniInt("font", "ListFontSize",		intListFontSize, 	strPath);
	WriteIniStr("font", "CollapseFont",		strCollapseFont, 	strPath);
	WriteIniInt("font", "CollapseFontSize",	intCollapseFontSize, 	strPath);
	WriteIniBln("font", "UseSkinFont", 		blnUseSkinFont, 	strPath);

	// �X�L��
	WriteIniInt("skin", "Skin1", 	intSkin1, 	strPath);
	WriteIniInt("skin", "Skin2", 	intSkin2, 	strPath);
	WriteIniStr("skin", "SkinDir1",	strSkinDir1, 	strPath);
	WriteIniStr("skin", "SkinDir2",	strSkinDir2, 	strPath);

	// MP3
	WriteIniBln("mp3", "UseId3v2", 		blnUseId3v2, 	strPath);
	WriteIniBln("mp3", "UseCue", 		blnUseCue, 	strPath);
	WriteIniBln("mp3", "UseMp3Cue", 		blnUseMp3Cue, 	strPath);

	// ���X�g
	WriteIniStr("List", "Normal", 		strListNormal, 		strPath);
	WriteIniStr("List", "ID3", 			strListID3, 		strPath);
	WriteIniStr("List", "Compilation", 		strListCompilation, 	strPath);
	WriteIniBln("List", "useID3", 		blnListID3, 		strPath);
	WriteIniBln("List", "useCompilation", 	blnListCompilation, 	strPath);

	// DLL
	string strData = "";
	for(int i = 0; i < vecHeaderDll.size(); i++)
	{
		char pszBuf[100];
		wsprintf(pszBuf, "%d", i);
		strData += string(pszBuf) + "=" + (vecUseHeaderDll[i] ? 'o' : 'x') + '/' + vecHeaderDll[i] + '\0';
	}
	const char* p = strData.c_str();
	WritePrivateProfileSection("Dll", strData.c_str(), strPath.c_str());

	// �L���b�V��
	WriteIniInt("pos", "x", intX, pszFile) ;
	WriteIniInt("pos", "y", intY, pszFile) ;
	WriteIniInt("pos", "width", intBlockX, pszFile) ;
	WriteIniInt("pos", "height", intBlockY, pszFile) ;
	WriteIniBln("Display", "CountUp", blnCountUp, pszFile) ;
	WriteIniBln("Display", "compact", blnCompact, pszFile) ;

}


/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Load() 
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH + 1] ;
		GetModuleFileName( hInstance, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPluginDir = GetDirName(strPath);
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		GetModuleFileName( GetModuleHandle( NULL), pszPath, MAX_PATH) ;
		strWinampIniPath = pszPath ;
		strWinampIniPath.replace( strWinampIniPath.rfind( "."), 4, ".ini", 5) ;

	}

	// �E�C���h�E
	blnShowOnlyArchive	= ReadIniBln("window", "ShowOnlyArchive", strPath, FALSE);
	blnShowOnlyUncompressed	= ReadIniBln("window", "ShowOnlyUncompressed", strPath, FALSE);
	blnUseTimebar		= ReadIniBln("window", "UseTimebar", strPath, TRUE);
	blnAttachToWinamp	= ReadIniBln("window", "AttachToWinamp", strPath, TRUE);
	intTransparency		= ReadIniInt("window", "Transparency", strPath, 0);
	intTransparency		= (intTransparency >= -1 && intTransparency <= 100 ? intTransparency : 0);

	// �Đ����@
	intRepeat		= ReadIniInt("play", "repeat", strPath, 0);

	// �t�H���g
	strListFont		= ReadIniStr("font", "ListFont",		strPath, "�l�r �o�S�V�b�N");
	intListFontSize		= ReadIniInt("font", "ListFontSize",	strPath, 0);
	strCollapseFont		= ReadIniStr("font", "CollapseFont",	strPath, "�l�r �o�S�V�b�N");
	intCollapseFontSize	= ReadIniInt("font", "CollapseFontSize",	strPath, 0);
	blnUseSkinFont		= ReadIniBln("font", "UseSkinFont",		strPath, TRUE);

	// �X�L��
	intSkin1	= ReadIniInt("skin", "Skin1", strPath, 0);
	intSkin2	= ReadIniInt("skin", "Skin2", strPath, 0);
	strSkinDir1	= ReadIniStr("skin", "SkinDir1", strPath, "");
	strSkinDir2	= ReadIniStr("skin", "SkinDir2", strPath, "");
	intSkin1	= (intSkin1 < 0 || intSkin1 > 1) ? 0 : intSkin1;
	intSkin2	= (intSkin2 < 0 || intSkin2 > 2) ? 0 : intSkin2;

	// MP3
	blnUseId3v2	= ReadIniBln("mp3", "UseId3v2",	strPath, TRUE);
	blnUseCue	= ReadIniBln("mp3", "UseCue",	strPath, TRUE);
	blnUseMp3Cue	= ReadIniBln("mp3", "UseMp3Cue",	strPath, TRUE);

	// ���X�g
	strListNormal		= ReadIniStr("List", "Normal", 		strPath, "%FILE_NAME%");
	strListID3		= ReadIniStr("List", "ID3", 		strPath, "%TRACK_NUMBER2%. %TRACK_NAME%");
	strListCompilation	= ReadIniStr("List", "Compilation",		strPath, "%TRACK_NUMBER2%. (%ARTIST_NAME%)%TRACK_NAME%") ;
	blnListID3		= ReadIniBln("List", "useID3",		strPath, TRUE);
	blnListCompilation	= ReadIniBln("List", "useCompilation",	strPath, TRUE);

	// Dll
	vector<tstring> vecData = SplitStr(GetSectionString("Dll",  strPath), "\n");
	if(vecData.size() == 0)
	{
		// ������ (<winamp>\Plugin\gen_zipalbum\zip_r.dll �����X�g�ɉ�����)
		char pszPath[ MAX_PATH + 1] ;
		GetModuleFileName(hInstance, pszPath, MAX_PATH);
		string strPath = pszPath;
		strPath = strPath.substr(0, strPath.rfind("."));
		strPath += "\\zip_r.dll";
		vecHeaderDll.push_back(strPath);
		vecUseHeaderDll.push_back(TRUE);
	}
	else
	{
		// �ǂݎ��
		for(int i = 0; i < vecData.size(); i++)
		{
			// <num>=[o|x]/<path>
			string strLine = vecData[i];
			strLine = strLine.substr(strLine.find('=') + 1);
			BOOL blnUse = strLine[0] == 'o';	// o �Ȃ�Η��p

			strLine = strLine.substr(strLine.find('/') + 1);
			if(strLine != "")
			{
				vecHeaderDll.push_back(strLine);		// �p�X
				vecUseHeaderDll.push_back(blnUse);
			}
		}
	}

	// �L���b�V��
	intX		= ReadIniInt("pos", "x", strPath, 50) ;
	intY		= ReadIniInt("pos", "y", strPath, 30) ;
	intBlockX	= ReadIniInt("pos", "width", strPath, 5) ;
	intBlockY	= ReadIniInt("pos", "height", strPath, 3) ;
	blnCountUp	= ReadIniBln("Display", "CountUp", strPath, TRUE) ;
	blnCompact	= ReadIniBln("Display", "compact", strPath, FALSE) ;

	RECT rc ;
	GetWindowRect( GetDesktopWindow(), &rc) ;
	intX = ( intX < 0 ? 0 : ( intX > rc.right ? 50 : intX)) ;
	intY = ( intY < 0 ? 0 : ( intY > rc.bottom ? 30 : intY)) ;
	intBlockX = ( intBlockX < 5 ? 5 : intBlockX) ;
	intBlockY = ( intBlockY < 2 ? 2 : intBlockY) ;
}
