
// Profile.h
// アプリケーションの設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  REPEAT_NORMAL		0
#define  REPEAT_SONG		1
#define  REPEAT_ALBUM		2
#define  REPEAT_ENDLESSRANDOM	3
#define  REPEAT_RANDOM		4


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
	static string strPluginDir ;

// 再生方法
	static int	intRepeat;

// ウインドウ
	static BOOL	blnShowOnlyArchive ;
	static BOOL	blnShowOnlyUncompressed;
	static BOOL	blnAttachToWinamp;
	static BOOL	blnUseTimebar ;
	static int	intTransparency;

// フォント
	static string	strListFont;
	static int	intListFontSize;
	static string	strCollapseFont;
	static int	intCollapseFontSize;
	static BOOL	blnUseSkinFont;

// スキン
	static int	intSkin1;
	static int	intSkin2;
	static string	strSkinDir1;
	static string	strSkinDir2;

	static BOOL blnCountUp ;
	static BOOL blnCompact ;

// MP3
	static BOOL blnUseId3v2;
	static BOOL blnUseCue;
	static BOOL blnUseMp3Cue;

// リスト
	static string strListNormal ;
	static string strListID3 ;
	static string strListCompilation ;
	static BOOL blnListID3 ;
	static BOOL blnListCompilation ;

// DLL
	static vector<string>	vecHeaderDll;
	static vector<BOOL>	vecUseHeaderDll;

// 場所
	static int intX ;
	static int intY ;
	static int intBlockX ;
	static int intBlockY ;
	static int intInfoX ;
	static int intInfoY ;
	static int intInfoWidth ;
	static int intInfoHeight ;
	static BOOL blnInfoVisible;

// 読み書き
	static void Save() ;
	static void Load() ;
} ;


#endif