
// WinampHook.cpp
//============================================================================//
// 更新：03/04/28(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "WinampHook.h"
#include "Controller.h"
#include "MainWnd.h"
#include "Profile.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  IDM_WA_ABOUT		40041
#define  IDM_WA_MINIBROWSER	40298


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

WinampHook::WinampHook( MainWnd* p)
: pMainWnd( p)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/29(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

WinampHook::~WinampHook() 
{
}


/******************************************************************************/
// 登録
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

void WinampHook::Init( HWND hWnd)
{
	m_hWnd = hWnd ;
	hwndEQ = FindWindow( "Winamp EQ", NULL) ;
	hwndPE = FindWindow( "Winamp PE", NULL) ;
	hwndMB = FindWindow( "Winamp MB", NULL) ;

	wpcWinamp   = (WNDPROC)GetWindowLong( m_hWnd, GWL_WNDPROC) ;
	wpcWinampEQ = (WNDPROC)GetWindowLong( hwndEQ, GWL_WNDPROC) ;
	wpcWinampPE = (WNDPROC)GetWindowLong( hwndPE, GWL_WNDPROC) ;
	wpcWinampMB = (WNDPROC)GetWindowLong( hwndMB, GWL_WNDPROC) ;

	if( wpcWinamp)
	{
		if( SetWindowLong( hWnd, GWL_WNDPROC, (LONG)WinampHookProc) == 0)
		{
			wpcWinamp = NULL ;
		}
		else
		{
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)this) ;	// ポインタ登録
		}
	}
	if( wpcWinampEQ)
	{
		if( SetWindowLong( hwndEQ, GWL_WNDPROC, (LONG)WinampEQProc) == 0)
		{
			wpcWinampEQ = NULL ;
		}
		else
		{
			SetWindowLong( hwndEQ, GWL_USERDATA, (LONG)this) ;	// ポインタ登録
		}
	}
	if( wpcWinampPE)
	{
		if( SetWindowLong( hwndPE, GWL_WNDPROC, (LONG)WinampPEProc) == 0)
		{
			wpcWinampPE = NULL ;
		}
		else
		{
			SetWindowLong( hwndPE, GWL_USERDATA, (LONG)this) ;	// ポインタ登録
		}
	}
	if( wpcWinampMB)
	{
		if( SetWindowLong( hwndMB, GWL_WNDPROC, (LONG)WinampMBProc) == 0)
		{
			wpcWinampMB = NULL ;
		}
		else
		{
			SetWindowLong( hwndMB, GWL_USERDATA, (LONG)this) ;	// ポインタ登録
		}
	}
}


/******************************************************************************/
//		メッセージマップ
/******************************************************************************/
// 定義
//============================================================================//
// 更新：03/04/28(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_SUBCLASS_MESSAGE_MAP( WinampHookProc, WinampHook)
	ON_MESSAGE( WM_SYSKEYDOWN,		OnSysKeyDown)
	ON_MESSAGE( WM_KEYDOWN,			OnKeyDown)
	ON_MESSAGE( WM_MOVE,			OnMove)
END_SUBCLASS_MESSAGE_MAP( pWndObj->wpcWinamp)

BEGIN_SUBCLASS_MESSAGE_MAP( WinampEQProc, WinampHook)
	ON_MESSAGE( WM_SYSKEYDOWN,		OnSysKeyDown)
	ON_MESSAGE( WM_KEYDOWN,			OnKeyDown)
END_SUBCLASS_MESSAGE_MAP( pWndObj->wpcWinampEQ)

BEGIN_SUBCLASS_MESSAGE_MAP( WinampPEProc, WinampHook)
	ON_MESSAGE( WM_SYSKEYDOWN,		OnSysKeyDown)
	ON_MESSAGE( WM_KEYDOWN,			OnKeyDown)
END_SUBCLASS_MESSAGE_MAP( pWndObj->wpcWinampPE)

BEGIN_SUBCLASS_MESSAGE_MAP( WinampMBProc, WinampHook)
	ON_MESSAGE( WM_SYSKEYDOWN,		OnSysKeyDown)
	ON_MESSAGE( WM_KEYDOWN,			OnKeyDown)
END_SUBCLASS_MESSAGE_MAP( pWndObj->wpcWinampMB)


/******************************************************************************/
// キーダウン
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT WinampHook::OnSysKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	// 表示、非表示の切り替え
	if( wParam == 'M' && lParam & 2 << 28)
	{
		Controller::GetInstance()->ToggleVisiblity() ;
		return 0 ;
	}

	// デフォルトのプロシージャに投げる
	WNDPROC w ;
	if( hWnd == m_hWnd)		w = wpcWinamp ;
	else if( hWnd == hwndEQ)	w = wpcWinampEQ ;
	else if( hWnd == hwndPE)	w = wpcWinampPE ;
	else if( hWnd == hwndMB)	w = wpcWinampMB ;
	else				return 0 ;

	return CallWindowProc( w, hWnd, WM_SYSKEYDOWN, wParam, lParam) ;
}


/******************************************************************************/
// キーダウン
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT WinampHook::OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	// 次の曲、前の曲
	if( wParam == VK_OEM_COMMA || wParam == VK_OEM_PERIOD)
	{
		Controller::GetInstance()->Go( pMainWnd->GetCurSong() + ( wParam == VK_OEM_COMMA ? -1 : 1)) ;
		return 0 ;
	}

	// デフォルトのプロシージャに投げる
	WNDPROC w ;
	if( hWnd == m_hWnd)		w = wpcWinamp ;
	else if( hWnd == hwndEQ)	w = wpcWinampEQ ;
	else if( hWnd == hwndPE)	w = wpcWinampPE ;
	else if( hWnd == hwndMB)	w = wpcWinampMB ;
	else				return 0 ;

	return CallWindowProc( w, hWnd, WM_KEYDOWN, wParam, lParam) ;
}


/******************************************************************************/
// 移動
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//
	
LRESULT WinampHook::OnMove( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( Profile::blnAttachToWinamp && pMainWnd->IsSnapping())
	{
		POINT pt = pMainWnd->GetOffsetSnap() ;
		Profile::intX = (short)LOWORD( lParam) + pt.x ;
		Profile::intY = (short)HIWORD( lParam) + pt.y ;
		SetWindowPos( pMainWnd->GetHwnd(), NULL, Profile::intX, Profile::intY, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOACTIVATE) ;
	}
	return CallWindowProc( wpcWinamp, hWnd, WM_MOVE, wParam, lParam) ;
}