
// SkinDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __SKIN_DLG_H__
#define  __SKIN_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class SkinDlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	SkinDlg() ;
	~SkinDlg() ;

	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(SkinDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnRadioChanged	(HWND, WPARAM, LPARAM);

	void SetEnable();
} ;

#endif