
// Mp3Dlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __MP3_DLG_H__
#define  __MP3_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class Mp3Dlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	Mp3Dlg() ;
	~Mp3Dlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(Mp3DlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnChecked		(HWND, WPARAM, LPARAM);
	void SetEnable();
} ;

#endif