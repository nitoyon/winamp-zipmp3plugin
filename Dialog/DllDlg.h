
// DllDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __DLL_DLG_H__
#define  __DLL_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class DllDlg : public ChildDlg
{
private:
	HWND	hwndList;
	HWND	hwndToolbar;

public:
// �R���X�g���N�^����уf�X�g���N�^
	DllDlg() ;
	~DllDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(DllDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnBtnAdd		(HWND, WPARAM, LPARAM);
	BOOL OnBtnDel		(HWND, WPARAM, LPARAM);
	BOOL OnBtnUp		(HWND, WPARAM, LPARAM);
	BOOL OnBtnDown		(HWND, WPARAM, LPARAM);
	BOOL OnListNotify	(HWND, WPARAM, LPARAM);

	BOOL CreateToolbar();

private:
	BOOL SelectItem(int);
	string GetItemString(int, int);
	BOOL InsertItem(int intIndex, const string&, BOOL);
	BOOL DeleteItem();
	BOOL Add();
	BOOL Move(BOOL);
	UINT GetSelectedItem();
} ;

#endif