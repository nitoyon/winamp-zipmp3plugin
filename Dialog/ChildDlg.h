
// ChildDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __CHILD_DLG_H__
#define  __CHILD_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class ChildDlg
{
protected:
	HWND	m_hWnd ;

private:
	HBRUSH	hBrush;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ChildDlg() ;
	virtual ~ChildDlg() ;

// �E�C���h�E�n���h��
	HWND GetHwnd() const{return m_hWnd;}

// �ݒ�K��
	virtual void DoApply();

// ���b�Z�[�W�n���h��
protected:
	virtual BOOL OnCtlColorStatic( HWND, WPARAM, LPARAM) ;
} ;

#endif