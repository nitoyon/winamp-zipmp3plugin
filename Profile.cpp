
// Profile.cpp
// アプリケーションの設定
//============================================================================//
// 更新：03/02/01(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Profile.h"


/******************************************************************************/
//		定義
/******************************************************************************/

string	Profile::strPath = "" ;

// 参照用
HINSTANCE	Profile::hInstance = 0 ;
string		Profile::strWinampIniPath = "" ;

// ウインドウ
BOOL	Profile::blnShowOnlyZip = FALSE ;
BOOL	Profile::blnShowOnlyUncompressedZip = FALSE ;
BOOL	Profile::blnCountUp = FALSE ;

// リスト
string	Profile::strListNormal ;
string	Profile::strListID3 ;
string	Profile::strListCompilation ;
BOOL	Profile::blnListID3 ;
BOOL	Profile::blnListCompilation ;


// 場所
int	Profile::intX = 0 ;
int	Profile::intY = 0 ;
int	Profile::intBlockX = 0 ;
int	Profile::intBlockY = 0 ;


/******************************************************************************/
//		保存・読みとり
/******************************************************************************/
// 保存
//============================================================================//
// 更新：02/12/30(月)
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
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		GetModuleFileName( GetModuleHandle( NULL), pszPath, MAX_PATH) ;
		strWinampIniPath = pszPath ;
		strWinampIniPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;
	}
	const char* pszFile = strPath.c_str() ;

	// ウインドウ
	WritePrivateProfileString( "Display", "ShowOnlyZip", blnShowOnlyZip ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Display", "ShowOnlyUncompressedZip", blnShowOnlyUncompressedZip ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Display", "CountUp", blnCountUp ? "yes" : "no", pszFile) ;

	// リスト
	WritePrivateProfileString( "List", "Normal", strListNormal.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "ID3", strListID3.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "Compilation", strListCompilation.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "useID3", blnListID3 ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "List", "useCompilation", blnListCompilation ? "yes" : "no", pszFile) ;

	// 場所
	WriteProfile( pszFile, "pos", "x", intX) ;
	WriteProfile( pszFile, "pos", "y", intY) ;
	WriteProfile( pszFile, "pos", "width", intBlockX) ;
	WriteProfile( pszFile, "pos", "height", intBlockY) ;
}


/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：03/02/01(土)
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
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		GetModuleFileName( GetModuleHandle( NULL), pszPath, MAX_PATH) ;
		strWinampIniPath = pszPath ;
		strWinampIniPath.replace( strWinampIniPath.rfind( "."), 4, ".ini", 5) ;
	}
	const char* pszFile = strPath.c_str() ;
	char	pszBuf[ MAX_PATH] ;

	// ウインドウ
	GetPrivateProfileString( "Display", "ShowOnlyZip", "no", pszBuf, MAX_PATH, pszFile) ;
	blnShowOnlyZip = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "Display", "ShowOnlyUncompressedZip", "no", pszBuf, MAX_PATH, pszFile) ;
	blnShowOnlyUncompressedZip = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "Display", "CountUp", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnCountUp = ( stricmp( pszBuf, "yes") == 0) ;

	// リスト
	GetPrivateProfileString( "List", "Normal", "%FILE_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListNormal = pszBuf ;
	GetPrivateProfileString( "List", "ID3", "%TRACK_NUMBER2%. %TRACK_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListID3 = pszBuf ;
	GetPrivateProfileString( "List", "Compilation", "%TRACK_NUMBER2%. (%ARTIST_NAME%)%TRACK_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListCompilation = pszBuf ;
	GetPrivateProfileString( "List", "useID3", "no", pszBuf, MAX_PATH, pszFile) ;
	blnListID3 = ( stricmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "List", "useCompilation", "no", pszBuf, MAX_PATH, pszFile) ;
	blnListCompilation = ( stricmp( pszBuf, "yes") == 0) ;

	// 場所
	RECT rc ;
	GetWindowRect( GetDesktopWindow(), &rc) ;
	intX = GetPrivateProfileInt( "pos", "x", 50, pszFile) ;
	intX = ( intX < 0 ? 0 : ( intX > rc.right ? 50 : intX)) ;
	intY = GetPrivateProfileInt( "pos", "y", 30, pszFile) ;
	intX = ( intX < 0 ? 0 : ( intX > rc.bottom ? 30 : intX)) ;

	intBlockX = GetPrivateProfileInt( "pos", "width", 5, pszFile) ;
	intBlockX = ( intBlockX < 5 ? 5 : intBlockX) ;
	intBlockY = GetPrivateProfileInt( "pos", "height", 3, pszFile) ;
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

void Profile::WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui)
{
	char	pszBuf[ 256] ;
	wsprintf( pszBuf, "%u", ui) ;
	WritePrivateProfileString( pszSection, pszName, pszBuf, s.c_str()) ;
}