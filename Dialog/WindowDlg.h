
// WindowDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __WINDOW_DLG_H__
#define  __WINDOW_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class WindowDlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	WindowDlg() ;
	~WindowDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(WindowDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif