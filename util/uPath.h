
// uPath.h
// パスを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __PATH_H__
#define  __PATH_H__

#include  "uTstring.h"
#include  <windows.h>


/******************************************************************************/
// 		プロトタイプ宣言
/******************************************************************************/

tstring GetModuleFileName(HINSTANCE hInstance);
tstring GetDirName(const tstring& strPath);
tstring GetFileName(const tstring& strPath);
tstring GetExtensionName(const tstring& strPath);
tstring GetBaseName(const tstring& strPath);
BOOL IsFullPath(const tstring& strPath);

tstring ChangeExtension(const tstring& strPath, const tstring& strNewExtension);
tstring GetIniFileName(HINSTANCE hInstance);

#endif