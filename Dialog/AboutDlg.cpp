
// AboutDlg.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "AboutDlg.h"
#include "..\resource.h"
#include "..\Profile.h"


/******************************************************************************/
//		�O���[�o���ϐ�
/******************************************************************************/

WNDPROC	wpcStatic ;


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

AboutDlg::AboutDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

AboutDlg::~AboutDlg() 
{
}



/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_DLG_MESSAGE_MAP(AboutDlgProc, AboutDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnOk)
		ON_COMMAND( IDC_URL		, OnUrlClicked)
	END_COMMAND_MAP()
	ON_COMMAND( WM_CTLCOLORSTATIC		, OnCtlColorStatic)
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// �_�C�A���O������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL AboutDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �T�u�N���X��
	wpcStatic = (WNDPROC)GetWindowLong( GetDlgItem( hDlg, IDC_URL), GWL_WNDPROC) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_URL) , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

	CheckDlgButton(hDlg, IDC_USE_ID3V2, Profile::blnUseId3v2 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_USE_CUE,Profile::blnUseCue ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_USE_MP3CUE, Profile::blnUseMp3Cue ? BST_CHECKED : BST_UNCHECKED);

	return FALSE;
}


/******************************************************************************/
// OK
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL AboutDlg::OnOk(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hDlg, TRUE);
	return TRUE;
}


/******************************************************************************/
// URL ���N���b�N
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL AboutDlg::OnUrlClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.nitoyon.com/", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// �X�^�e�B�b�N�̃J���[
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL AboutDlg::OnCtlColorStatic( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam ;
	if( GetDlgItem( hDlg, IDC_URL) == (HWND)lParam)
	{
		SetTextColor( hdc, RGB( 0, 0, 255)) ;
		SetBkMode(hdc, TRANSPARENT);
		return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH) ;
	}

	return FALSE ;
}


/******************************************************************************/
// �����N�̂��߂̃T�u�N���X��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uiMsg)
	{
		case WM_SETCURSOR :
		{
			// ��J�[�\���ǂ݂Ƃ�
			HCURSOR hCurHand	= LoadCursor( Profile::hInstance, MAKEINTRESOURCE( IDC_HANDCUR)) ;
			SetCursor( hCurHand) ;
			return 0 ;
		}
	}

	return CallWindowProc( wpcStatic, hWnd, uiMsg, wParam, lParam) ;
}
