
// util.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "util.h"


/******************************************************************************/
// ushort
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

USHORT makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ULONG  makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}


/******************************************************************************/
// ウインドウのタイトルを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string GetWindowString( HWND hwnd)
{
	int intSize = GetWindowTextLength( hwnd) ;
	char* pszBuf = new char[ intSize + 1] ;

	GetWindowText( hwnd, pszBuf, intSize + 1) ;
	string s = pszBuf ;
	delete[] pszBuf ;

	return s ;
}


/******************************************************************************/
// ウインドウの場所を取得
//============================================================================//
// 概要：なし。
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
	rect.right	= rcChild.right - rcChild.left + pt.x;
	rect.bottom	= rcChild.bottom - rcChild.top + pt.y;

	return rect;
}


/******************************************************************************/
// フルパスからディレクトリ取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string GetDirName( const string& strPath)
{
	int	intLastYen = 0 ;
	char	pszFile[ MAX_PATH] ;
	char*	pszPointer = pszFile ;
	strcpy( pszFile, strPath.c_str()) ;

	for( int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if( IsDBCSLeadByte( *pszPointer))
		{
			// ２バイト文字なら２進む
			i++ ;
			continue ;
		}

		if( *pszPointer == '\\' || *pszPointer == '/')
		{
			intLastYen = i ;
		}
	}

	if( intLastYen > 0)
	{
		return strPath.substr( 0, intLastYen + 1) ;	// Yen も含めて返す
	}
	else
	{
		return "" ;
	}
}


/******************************************************************************/
// フルパスからファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string GetFileName( const string& strPath)
{
	string strDirName = GetDirName( strPath) ;
	return strPath.substr( strDirName.size()) ;
}


/******************************************************************************/
// NT 系かどうかを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL IsNT()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/******************************************************************************/
// メジャーバージョン取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DWORD GetOsMajorVersion()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwOSVersionInfoSize;
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

	SetForegroundWindow(hwnd);
}
