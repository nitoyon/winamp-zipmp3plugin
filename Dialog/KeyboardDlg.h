
// KeyboardDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __KEYBOARD_DLG_H__
#define  __KEYBOARD_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class KeyboardDlg : public ChildDlg
{
public:
// �R���X�g���N�^����уf�X�g���N�^
	KeyboardDlg() ;
	~KeyboardDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(KeyboardDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif