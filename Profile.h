
// Profile.h
// アプリケーションの設定
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Profile
{
private:
	static string strPath ;

public:
// 参照用
	static HINSTANCE hInstance ;
	static string strWinampIniPath ;

// 表示
	static BOOL blnShowOnlyZip ;
	static BOOL blnShowOnlyUncompressedZip ;
	static BOOL blnCountUp ;
	static WORD wrdHotKey ;

// リスト
	static string strListNormal ;
	static string strListID3 ;
	static string strListCompilation ;
	static BOOL blnListID3 ;
	static BOOL blnListCompilation ;
	
// 場所
	static int intX ;
	static int intY ;
	static int intBlockX ;
	static int intBlockY ;

// 読み書き
	static void Save() ;
	static void Load() ;

private:
	static void WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui) ;
} ;


#endif