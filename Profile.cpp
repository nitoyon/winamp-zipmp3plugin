
// Profile.cpp
// アプリケーションの設定
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Profile.h"
#include "util.h"


/******************************************************************************/
//		定義
/******************************************************************************/

string	Profile::strPath = "" ;

// 参照用
HINSTANCE	Profile::hInstance = 0 ;
string		Profile::strWinampIniPath = "" ;
string		Profile::strPluginDir;

// ウインドウ
BOOL	Profile::blnShowOnlyArchive;
BOOL	Profile::blnShowOnlyUncompressed;
BOOL	Profile::blnAttachToWinamp;
BOOL	Profile::blnUseTimebar;
int	Profile::intTransparency;

// フォント
string	Profile::strListFont;
int	Profile::intListFontSize;
string	Profile::strCollapseFont;
int	Profile::intCollapseFontSize;
BOOL	Profile::blnUseSkinFont;

// スキン
int	Profile::intSkin1;
int	Profile::intSkin2;
string	Profile::strSkinDir1;
string	Profile::strSkinDir2;

// MP3
BOOL	Profile::blnUseId3v2;
BOOL	Profile::blnUseCue;
BOOL	Profile::blnUseMp3Cue;

// リスト
string	Profile::strListNormal ;
string	Profile::strListID3 ;
string	Profile::strListCompilation ;
BOOL	Profile::blnListID3 ;
BOOL	Profile::blnListCompilation ;

// DLL
vector<string>	Profile::vecHeaderDll;
vector<BOOL>	Profile::vecUseHeaderDll;

// 場所
int	Profile::intX = 0 ;
int	Profile::intY = 0 ;
int	Profile::intBlockX = 0 ;
int	Profile::intBlockY = 0 ;
BOOL	Profile::blnCountUp;
BOOL	Profile::blnCompact;


/******************************************************************************/
//		保存・読みとり
/******************************************************************************/
// 保存
//============================================================================//
// 更新：03/04/20(日)
// 概要：なし。
// 補足：なし。
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

	// ウインドウ
	WriteProfileBln("window", "ShowOnlyArchive", 		blnShowOnlyArchive, 		strPath) ;
	WriteProfileBln("window", "ShowOnlyUncompressed", 	blnShowOnlyUncompressed, 	strPath) ;
	WriteProfileBln("window", "AttachToWinamp", 		blnAttachToWinamp, 		strPath) ;
	WriteProfileBln("window", "UseTimebar", 		blnUseTimebar, 			strPath) ;
	WriteProfileInt("window", "Transparency", 		intTransparency, 		strPath) ;

	// フォント
	WriteProfileStr("font", "ListFont",		strListFont, 		strPath);
	WriteProfileInt("font", "ListFontSize",		intListFontSize, 	strPath);
	WriteProfileStr("font", "CollapseFont",		strCollapseFont, 	strPath);
	WriteProfileInt("font", "CollapseFontSize",	intCollapseFontSize, 	strPath);
	WriteProfileBln("font", "UseSkinFont", 		blnUseSkinFont, 	strPath);

	// スキン
	WriteProfileInt("skin", "Skin1", 	intSkin1, 	strPath);
	WriteProfileInt("skin", "Skin2", 	intSkin2, 	strPath);
	WriteProfileStr("skin", "SkinDir1",	strSkinDir1, 	strPath);
	WriteProfileStr("skin", "SkinDir2",	strSkinDir2, 	strPath);

	// MP3
	WriteProfileBln("mp3", "UseId3v2", 		blnUseId3v2, 	strPath);
	WriteProfileBln("mp3", "UseCue", 		blnUseCue, 	strPath);
	WriteProfileBln("mp3", "UseMp3Cue", 		blnUseMp3Cue, 	strPath);

	// リスト
	WriteProfileStr("List", "Normal", 		strListNormal, 		strPath);
	WriteProfileStr("List", "ID3", 			strListID3, 		strPath);
	WriteProfileStr("List", "Compilation", 		strListCompilation, 	strPath);
	WriteProfileBln("List", "useID3", 		blnListID3, 		strPath);
	WriteProfileBln("List", "useCompilation", 	blnListCompilation, 	strPath);

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

	// キャッシュ
	WriteProfileInt("pos", "x", intX, pszFile) ;
	WriteProfileInt("pos", "y", intY, pszFile) ;
	WriteProfileInt("pos", "width", intBlockX, pszFile) ;
	WriteProfileInt("pos", "height", intBlockY, pszFile) ;
	WriteProfileBln("Display", "CountUp", blnCountUp, pszFile) ;
	WriteProfileBln("Display", "compact", blnCompact, pszFile) ;

}


/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
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

	// ウインドウ
	blnShowOnlyArchive	= ReadProfileBln("window", "ShowOnlyArchive", strPath, FALSE);
	blnShowOnlyUncompressed	= ReadProfileBln("window", "ShowOnlyUncompressed", strPath, FALSE);
	blnUseTimebar		= ReadProfileBln("window", "UseTimebar", strPath, TRUE);
	blnAttachToWinamp	= ReadProfileBln("window", "AttachToWinamp", strPath, TRUE);
	intTransparency		= ReadProfileInt("window", "Transparency", strPath);
	intTransparency		= (intTransparency >= 0 && intTransparency <= 100 ? intTransparency : 0);

	// フォント
	strListFont		= ReadProfileStr("font", "ListFont",		strPath, "ＭＳ Ｐゴシック");
	intListFontSize		= ReadProfileInt("font", "ListFontSize",	strPath);
	strCollapseFont		= ReadProfileStr("font", "CollapseFont",	strPath, "ＭＳ Ｐゴシック");
	intCollapseFontSize	= ReadProfileInt("font", "CollapseFontSize",	strPath);
	blnUseSkinFont		= ReadProfileBln("font", "UseSkinFont",		strPath, TRUE);

	// スキン
	intSkin1	= ReadProfileInt("skin", "Skin1", strPath, 0);
	intSkin2	= ReadProfileInt("skin", "Skin2", strPath, 0);
	strSkinDir1	= ReadProfileStr("skin", "SkinDir1", strPath, "");
	strSkinDir2	= ReadProfileStr("skin", "SkinDir2", strPath, "");
	intSkin1	= (intSkin1 < 0 || intSkin1 > 1) ? 0 : intSkin1;
	intSkin2	= (intSkin2 < 0 || intSkin2 > 2) ? 0 : intSkin2;

	// MP3
	blnUseId3v2	= ReadProfileBln("mp3", "UseId3v2",	strPath, TRUE);
	blnUseCue	= ReadProfileBln("mp3", "UseCue",	strPath, TRUE);
	blnUseMp3Cue	= ReadProfileBln("mp3", "UseMp3Cue",	strPath, TRUE);

	// リスト
	strListNormal		= ReadProfileStr("List", "Normal", 		strPath, "%FILE_NAME%");
	strListID3		= ReadProfileStr("List", "ID3", 		strPath, "%TRACK_NUMBER2%. %TRACK_NAME%");
	strListCompilation	= ReadProfileStr("List", "Compilation",		strPath, "%TRACK_NUMBER2%. (%ARTIST_NAME%)%TRACK_NAME%") ;
	blnListID3		= ReadProfileBln("List", "useID3",		strPath, TRUE);
	blnListCompilation	= ReadProfileBln("List", "useCompilation",	strPath, TRUE);

	// Dll
	vector<string> vecData = ReadProfileSection("Dll",  strPath);
	if(vecData.size() == 0)
	{
		// 初期化 (<winamp>\Plugin\gen_zipalbum\zip_r.dll をリストに加える)
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
		// 読み取り
		for(int i = 0; i < vecData.size(); i++)
		{
			// <num>=[o|x]/<path>
			string strLine = vecData[i];
			strLine = strLine.substr(strLine.find('=') + 1);
			BOOL blnUse = strLine[0] == 'o';	// o ならば利用

			strLine = strLine.substr(strLine.find('/') + 1);
			if(strLine != "")
			{
				vecHeaderDll.push_back(strLine);		// パス
				vecUseHeaderDll.push_back(blnUse);
			}
		}
	}

	// キャッシュ
	intX		= ReadProfileInt("pos", "x", strPath, 50) ;
	intY		= ReadProfileInt("pos", "y", strPath, 30) ;
	intBlockX	= ReadProfileInt("pos", "width", strPath, 5) ;
	intBlockY	= ReadProfileInt("pos", "height", strPath, 3) ;
	blnCountUp	= ReadProfileBln("Display", "CountUp", strPath, TRUE) ;
	blnCompact	= ReadProfileBln("Display", "compact", strPath, FALSE) ;

	RECT rc ;
	GetWindowRect( GetDesktopWindow(), &rc) ;
	intX = ( intX < 0 ? 0 : ( intX > rc.right ? 50 : intX)) ;
	intY = ( intY < 0 ? 0 : ( intY > rc.bottom ? 30 : intY)) ;
	intBlockX = ( intBlockX < 5 ? 5 : intBlockX) ;
	intBlockY = ( intBlockY < 2 ? 2 : intBlockY) ;
}


/******************************************************************************/
//		ユーティリティー関数
/******************************************************************************/
// 数字を書き込み
//============================================================================//
// 更新：02/10/27(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Profile::WriteProfileInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const string& s)
{
	char	pszBuf[ 256] ;
	wsprintf(pszBuf, "%u", ui) ;
	WritePrivateProfileString( pszSection, pszName, pszBuf, s.c_str()) ;
}


/******************************************************************************/
// 文字列を書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Profile::WriteProfileStr(LPTSTR pszSection, LPTSTR pszName, const string& strData, const string& s)
{
	WritePrivateProfileString(pszSection, pszName, strData.c_str(), s.c_str()) ;
}


/******************************************************************************/
// 真偽値を書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Profile::WriteProfileBln(LPTSTR pszSection, LPTSTR pszName, BOOL b, const string& s)
{
	WritePrivateProfileString(pszSection, pszName, b ? "yes" : "no", s.c_str());
}


/******************************************************************************/
// 整数を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int Profile::ReadProfileInt(LPTSTR pszSection, LPTSTR pszName, const string& s, int intDefault)
{
	return GetPrivateProfileInt(pszSection, pszName, intDefault, s.c_str());
}


/******************************************************************************/
// 文字列を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string Profile::ReadProfileStr(LPTSTR pszSection, LPTSTR pszName, const string& s, const string& strDefault)
{
	char pszBuf[MAX_PATH + 1];
	GetPrivateProfileString(pszSection, pszName, strDefault.c_str(), pszBuf, MAX_PATH, s.c_str());

	return pszBuf;
}


/******************************************************************************/
// 真偽を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Profile::ReadProfileBln(LPTSTR pszSection, LPTSTR pszName, const string& s, BOOL blnDefault)
{
	char pszBuf[MAX_PATH + 1];
	GetPrivateProfileString(pszSection, pszName, blnDefault ? "yes" : "no", pszBuf, MAX_PATH, s.c_str()) ;
	return ( strcmp(pszBuf, "yes") == 0);
}


/******************************************************************************/
// セクション全体を読み取る
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

vector<string> Profile::ReadProfileSection(LPTSTR pszSection, const string& s)
{
	PTSTR pszBuf;
	DWORD dwSize = 256;

	while(1)
	{
		pszBuf = new TCHAR[dwSize];
		DWORD dwRet = GetPrivateProfileSection(pszSection, pszBuf, dwSize, s.c_str());

		if(dwRet == dwSize - 2)
		{
			delete[] pszBuf;
			continue;
		}

		break;
	}

	vector<string> v;
	const char* p = pszBuf;
	while(*p)
	{
		if(*p == '#' || *p == ';')
		{
			p += lstrlen(p) + 1;
			continue;
		}
		v.push_back(p);
		p += lstrlen(p) + 1;
	}

	return v;
}