
// ListDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __LIST_DLG_H__
#define  __LIST_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class ListDlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	ListDlg() ;
	~ListDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(ListDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnChecked		(HWND, WPARAM, LPARAM);
	BOOL OnClickBtn		(HWND, WPARAM, LPARAM);

	void SetEnable();
	void DisplayMenu(const vector<string>&, int);
};

#endif