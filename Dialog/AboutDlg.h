
// AboutDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __ABOUT_DLG_H__
#define  __ABOUT_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class AboutDlg
{
private:
	HWND	m_hWnd;

public:
// �R���X�g���N�^����уf�X�g���N�^
	AboutDlg() ;
	~AboutDlg() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(AboutDlgProc)
	friend LRESULT CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnOk		(HWND, WPARAM, LPARAM);
	BOOL OnUrlClicked	(HWND, WPARAM, LPARAM);
	BOOL OnCtlColorStatic	(HWND, WPARAM, LPARAM);
} ;

#endif