
// NullDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __NULL_DLG_H__
#define  __NULL_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class NullDlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	NullDlg() ;
	~NullDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(NullDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif