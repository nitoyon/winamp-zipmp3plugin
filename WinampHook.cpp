
// WinampHook.cpp
//============================================================================//
// �X�V�F03/04/28(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "WinampHook.h"
#include "Controller.h"
#include "MainWnd.h"
#include "Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  IDM_WA_ABOUT		40041
#define  IDM_WA_MINIBROWSER	40298


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

WinampHook::WinampHook( MainWnd* p)
: pMainWnd( p)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/29(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

WinampHook::~WinampHook() 
{
}


/******************************************************************************/
// �o�^
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)this) ;	// �|�C���^�o�^
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
			SetWindowLong( hwndEQ, GWL_USERDATA, (LONG)this) ;	// �|�C���^�o�^
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
			SetWindowLong( hwndPE, GWL_USERDATA, (LONG)this) ;	// �|�C���^�o�^
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
			SetWindowLong( hwndMB, GWL_USERDATA, (LONG)this) ;	// �|�C���^�o�^
		}
	}
}


/******************************************************************************/
//		���b�Z�[�W�}�b�v
/******************************************************************************/
// ��`
//============================================================================//
// �X�V�F03/04/28(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �L�[�_�E��
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT WinampHook::OnSysKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	// �\���A��\���̐؂�ւ�
	if( wParam == 'M' && lParam & 2 << 28)
	{
		Controller::GetInstance()->ToggleVisiblity() ;
		return 0 ;
	}

	// �f�t�H���g�̃v���V�[�W���ɓ�����
	WNDPROC w ;
	if( hWnd == m_hWnd)		w = wpcWinamp ;
	else if( hWnd == hwndEQ)	w = wpcWinampEQ ;
	else if( hWnd == hwndPE)	w = wpcWinampPE ;
	else if( hWnd == hwndMB)	w = wpcWinampMB ;
	else				return 0 ;

	return CallWindowProc( w, hWnd, WM_SYSKEYDOWN, wParam, lParam) ;
}


/******************************************************************************/
// �L�[�_�E��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT WinampHook::OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	// ���̋ȁA�O�̋�
	if( wParam == VK_OEM_COMMA || wParam == VK_OEM_PERIOD)
	{
		Controller::GetInstance()->Go( pMainWnd->GetCurSong() + ( wParam == VK_OEM_COMMA ? -1 : 1)) ;
		return 0 ;
	}

	// �f�t�H���g�̃v���V�[�W���ɓ�����
	WNDPROC w ;
	if( hWnd == m_hWnd)		w = wpcWinamp ;
	else if( hWnd == hwndEQ)	w = wpcWinampEQ ;
	else if( hWnd == hwndPE)	w = wpcWinampPE ;
	else if( hWnd == hwndMB)	w = wpcWinampMB ;
	else				return 0 ;

	return CallWindowProc( w, hWnd, WM_KEYDOWN, wParam, lParam) ;
}


/******************************************************************************/
// �ړ�
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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