
// uWindow.cpp
// ウインドウを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include  "uWindow.h"


/******************************************************************************/
// ウインドウのタイトルを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetWindowString(HWND hwnd)
{
	int intSize = GetWindowTextLength(hwnd);
	TCHAR* pszBuf = new TCHAR[intSize + 1];

	GetWindowText(hwnd, pszBuf, intSize + 1);
	tstring s = pszBuf;
	delete[] pszBuf;

	return s ;
}


/******************************************************************************/
// ウインドウのタイトルを設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SetWindowString(HWND hwnd, const tstring& str)
{
	SetWindowText(hwnd, str.c_str());
}


/******************************************************************************/
// ダイアログのタイトル取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetDlgString(HWND hwnd, UINT uiID)
{
	HWND h = GetDlgItem(hwnd, uiID);
	return GetWindowString(h);
}


/******************************************************************************/
// ダイアログのタイトル設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SetDlgString(HWND hwnd, UINT uiID, const tstring& s)
{
	HWND h = GetDlgItem(hwnd, uiID);
	SetWindowString(h, s);
}


/******************************************************************************/
// ウインドウを中心に表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

VOID CenterWindow(HWND hwnd)
{
	RECT    rect;
	LONG    dx, dy;
	LONG    dxParent, dyParent;
	LONG    Style;

	// Get window rect
	GetWindowRect(hwnd, &rect);

	dx = rect.right  - rect.left;
	dy = rect.bottom - rect.top;

	// Get parent rect
	Style = GetWindowLong(hwnd, GWL_STYLE);
	if ((Style & WS_CHILD) == 0)
	{
		// Return the desktop windows size (size of main screen)
		dxParent = GetSystemMetrics(SM_CXSCREEN);
		dyParent = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		HWND    hwndParent;
		RECT    rectParent;

		hwndParent = GetParent(hwnd);
		if(hwndParent == NULL)
		{
			hwndParent = GetDesktopWindow();
		}

		GetWindowRect(hwndParent, &rectParent);

		dxParent = rectParent.right - rectParent.left;
		dyParent = rectParent.bottom - rectParent.top;
	}

	// Centre the child in the parent
	rect.left = (dxParent - dx) / 2;
	rect.top  = (dyParent - dy) / 3;

	// Move the child into position
	SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);
}


/******************************************************************************/
// ウインドウの親ウインドウ内の場所を取得
//============================================================================//
// 概要：四隅のクライアント座標を返す。
// 補足：なし。
//============================================================================//

RECT GetChildRect(HWND hwndParent, HWND hwndChild)
{
	RECT rcChild;
	RECT rect;
	
	GetWindowRect(hwndChild, &rcChild);

	POINT pt = {rcChild.left, rcChild.top};
	ScreenToClient(hwndParent, &pt);

	rect.left	= pt.x;
	rect.top	= pt.y;
	rect.right	= rcChild.right  - rcChild.left + pt.x;
	rect.bottom	= rcChild.bottom - rcChild.top  + pt.y;

	return rect;
}


