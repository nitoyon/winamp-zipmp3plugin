
// KeyboardDlg.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "KeyboardDlg.h"
#include "..\Profile.h"
#include "..\resource.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

KeyboardDlg::KeyboardDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

KeyboardDlg::~KeyboardDlg() 
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

BEGIN_DLG_MESSAGE_MAP(KeyboardDlgProc, KeyboardDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// �_�C�A���O������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL KeyboardDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList = GetDlgItem(hDlg, IDC_KEYBOARDLIST);

	// �R�����ǉ�
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
	lvcol.pszText = "�L�[�{�[�h";
	lvcol.cx = 120;
	ListView_InsertColumn(hwndList, 0, &lvcol);

	lvcol.pszText = "���e";
	lvcol.cx = 180;
	ListView_InsertColumn(hwndList, 1, &lvcol);

	// �A�C�e���ǉ�
	LVITEM item;
	int intIndex = 0;
	item.mask = LVIF_TEXT;
	item.iItem = intIndex;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)"Alt+M";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�E�C���h�E�\��/��\���؂�ւ�");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"Alt + 3";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�t�@�C�����\��");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)", (�J���})";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�A���o�����łP�Ȗ߂�");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)". (�s���I�h)";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�A���o�����łP�Ȑi��");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"��";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�T�b�߂�");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"��";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"�T�b�i��");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"�A�v���P�[�V�����L�[";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"���j���[�\��");
	intIndex++;

	return FALSE;
}


/******************************************************************************/
// �K�p
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void KeyboardDlg::DoApply()
{
}