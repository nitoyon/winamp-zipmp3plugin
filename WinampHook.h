
// WinampHook.h
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __WINAMP_HOOK_H__
#define  __WINAMP_HOOK_H__

#include "include.h"
#include "MsgMap.h"


class MainWnd ;

/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

class WinampHook
{
private:
	MainWnd* pMainWnd ;

	HWND	m_hWnd ;
	HWND	hwndEQ ;
	HWND	hwndPE ;
	HWND	hwndMB ;
	WNDPROC	wpcWinamp ;
	WNDPROC	wpcWinampEQ ;
	WNDPROC	wpcWinampPE ;
	WNDPROC	wpcWinampMB ;

public:
// コンストラクタおよびデストラクタ
	WinampHook( MainWnd*) ;
	~WinampHook() ;
	void Init( HWND) ;

private:
// メッセージマップ
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampHookProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampEQProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampPEProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampMBProc)

	LRESULT OnSysKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnMove		( HWND, WPARAM, LPARAM) ;
} ;

#endif