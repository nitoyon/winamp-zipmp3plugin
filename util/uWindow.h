
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
#include  <commctrl.h>


/******************************************************************************/
// 		プロトタイプ宣言
/******************************************************************************/

// 一般的なウインドウ
tstring GetWindowString(HWND hwnd);
void SetWindowString(HWND hwnd, const tstring& str);
tstring GetDlgString(HWND hwnd, UINT uiID);
void SetDlgString(HWND hwnd, UINT uiID, const tstring& s);
tstring GetWindowClass(HWND hwnd);

VOID CenterWindow(HWND hwnd);
RECT GetChildRect(HWND hwndParent, HWND hwndChild);

// コントロール
tstring GetComboBoxString(HWND hwndCombo, int intIndex);
tstring GetListBoxString(HWND hwndList, int intIndex);
tstring GetTreeString(HWND hwndTree, HTREEITEM hitem);
tstring GetListViewString(HWND hWnd, int intItem, int);

#endif