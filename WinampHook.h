
// WinampHook.h
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __WINAMP_HOOK_H__
#define  __WINAMP_HOOK_H__

#include "include.h"
#include "MsgMap.h"


class MainWnd ;

/******************************************************************************/
//		�v���g�^�C�v�錾
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
// �R���X�g���N�^����уf�X�g���N�^
	WinampHook( MainWnd*) ;
	~WinampHook() ;
	void Init( HWND) ;

private:
// ���b�Z�[�W�}�b�v
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampHookProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampEQProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampPEProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( WinampMBProc)

	LRESULT OnSysKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnMove		( HWND, WPARAM, LPARAM) ;
} ;

#endif