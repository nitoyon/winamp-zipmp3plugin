
// uWindow.h
// ウインドウを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __WINDOW_H__
#define  __WINDOW_H__

#include  "uTstring.h"
#include  <windows.h>


/******************************************************************************/
// 		プロトタイプ宣言
/******************************************************************************/

tstring GetWindowString(HWND hwnd);
void SetWindowString(HWND hwnd, const tstring& str);
tstring GetDlgString(HWND hwnd, UINT uiID);
void SetDlgString(HWND hwnd, UINT uiID, const tstring& s);
VOID CenterWindow(HWND hwnd);
RECT GetChildRect(HWND hwndParent, HWND hwndChild);

#endif