
// uWindow.cpp
// ウインドウを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include  "uWindow.h"


/******************************************************************************/
//		一般的なウインドウ
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
// クラス名を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetWindowClass(HWND hwnd)
{
	int intSize = 2;
	PTSTR pszBuf;

	while(1)
	{
		pszBuf = new TCHAR[intSize];
		int i = GetClassName(hwnd, pszBuf, intSize);
		if(i != intSize - 1)
		{
			break;
		}

		intSize *= 2;
		delete[] pszBuf;
	}

	tstring s = pszBuf;
	delete[] pszBuf;
	return s;
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


/******************************************************************************/
//		コントロール
/******************************************************************************/
// リストボックスのテキスト取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetListBoxString(HWND hwndList, int intIndex)
{
	UINT uiLen = (UINT)SendMessage(hwndList, LB_GETTEXTLEN, intIndex, 0);
	PTSTR pszBuf = new TCHAR[uiLen + 1];
	SendMessage(hwndList, LB_GETTEXT, intIndex, (LPARAM)pszBuf);

	tstring s = pszBuf;
	delete[] pszBuf;
	return s;
}


/******************************************************************************/
// コンボボックスのテキスト取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetComboBoxString(HWND hwndCombo, int intIndex)
{
	int intTxtLen = SendMessage(hwndCombo, CB_GETLBTEXTLEN, intIndex, 0);
	tstring strRet = TEXT("");

	if(intTxtLen != CB_ERR)
	{
		TCHAR* pszBuf = new TCHAR[intTxtLen + 1];
		if(SendMessage(hwndCombo, CB_GETLBTEXT, intIndex, (LPARAM)pszBuf) != CB_ERR)
		{
			strRet = pszBuf;
		}
		delete[] pszBuf;
	}

	return strRet;
}


/******************************************************************************/
// ツリービューのテキスト取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetTreeString(HWND hwndTree, HTREEITEM hitem)
{
	DWORD	dwSize = 64;
	TVITEM	ti;
	LPTSTR	pszBuf = NULL;

	while(1)
	{
		pszBuf = new TCHAR[dwSize];
		ti.mask = TVIF_TEXT ;
		ti.hItem = hitem;
		ti.pszText = pszBuf;
		ti.cchTextMax = dwSize;
		if(TreeView_GetItem(hwndTree, &ti))
		{
			ti.pszText[dwSize - 1] = TEXT('\0');
			if((UINT)lstrlen(ti.pszText) == dwSize -1)
			{
				dwSize *= 2;
				delete[] pszBuf;
				pszBuf = NULL;
			}
			else
			{
				// 取得できた時の処理
				break;
			}
		}
		else
		{
			// データ取得失敗。ハンドルがおかしい？
			break;
		}
	}

	tstring	s = TEXT("");
	if(pszBuf)
	{
		s = pszBuf;;
		delete[] pszBuf;
	}

	return s;
}


/******************************************************************************/
// リストビューのテキスト取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetListViewString(HWND hWnd, int intItem, int intSubItem)
{
	DWORD	dwSize = 256;
	LPTSTR	pszBuf;
	while(1)
	{
		pszBuf = new TCHAR[dwSize];
		ListView_GetItemText(hWnd, intItem, intSubItem, pszBuf, dwSize);
		pszBuf[dwSize - 1] = TEXT('\0');
		if(lstrlen(pszBuf) != dwSize - 1)
		{
			break;
		}

		delete[] pszBuf;
		dwSize *= 2;
	}

	tstring s = pszBuf;
	delete[] pszBuf;

	return s;
}