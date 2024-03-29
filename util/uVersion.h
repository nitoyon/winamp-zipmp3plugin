
// uVersion.h
// バージョンを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __VERSION_H__
#define  __VERSION_H__


/******************************************************************************/
//		インクルード
/******************************************************************************/

#include  <windows.h>
#include  "uTstring.h"


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

// windows のバージョン
BOOL IsNT();
DWORD GetOsMajorVersion();

// ファイルのバージョン
tstring GetFileVersion(const tstring& strPath);

#endif