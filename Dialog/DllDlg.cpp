
// DllDlg.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "DllDlg.h"
#include "..\resource.h"
#include "..\util.h"
#include "..\Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

#define IDM_NEW		10
#define IDM_DEL		11
#define IDM_UP		12
#define IDM_DOWN	13


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DllDlg::DllDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DllDlg::~DllDlg() 
{
}


/******************************************************************************/
//		�K�p
/******************************************************************************/
// �K�p
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void DllDlg::DoApply()
{
	int intCount = ListView_GetItemCount(hwndList);
	Profile::vecHeaderDll.clear();
	Profile::vecUseHeaderDll.clear();

	for(int i = 0; i < intCount; i++)
	{
		// �p�X�ݒ�
		Profile::vecHeaderDll.push_back(GetItemString(i, 1));

		// �L������
		Profile::vecUseHeaderDll.push_back(ListView_GetCheckState(hwndList, i));
	}
}


/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_DLG_MESSAGE_MAP(DllDlgProc, DllDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDC_ADD		, OnBtnAdd)
		ON_COMMAND( IDC_DEL		, OnBtnDel)
		ON_COMMAND( IDC_UP		, OnBtnUp)
		ON_COMMAND( IDC_DOWN		, OnBtnDown)
	END_COMMAND_MAP()
	BEGIN_NOTIFY_MAP()
		ON_NOTIFY(IDC_DLL_LIST	, OnListNotify)
	END_NOTIFY_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// �_�C�A���O������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	hInstance = Profile::hInstance;
	hwndList = GetDlgItem(hDlg, IDC_DLL_LIST);
	ListView_SetExtendedListViewStyleEx(hwndList, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	// �R�����ǉ�
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
	lvcol.pszText = "DLL ��";
	lvcol.cx = 100;
	ListView_InsertColumn(hwndList, 0, &lvcol);

	lvcol.pszText = "�p�X";
	lvcol.cx = 200;
	ListView_InsertColumn(hwndList, 1, &lvcol);

	// �C���[�W�ǉ�
	hImgList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	HICON hicon;
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLL_FOUND));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLL_NOTFOUND));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	ListView_SetImageList(hwndList, hImgList, LVSIL_SMALL);

	// �A�C�e���ǉ�
	for(int i = 0; i < Profile::vecHeaderDll.size(); i++)
	{
		InsertItem(i, Profile::vecHeaderDll[i], Profile::vecUseHeaderDll[i]);
		if(i == 0)
		{
			SelectItem(0);
		}
	}

/*	CreateToolbar();
	RECT rect, rectDlg;
	GetClientRect(hDlg, &rectDlg);
	GetClientRect(hwndToolbar, &rect);
	MoveWindow(hwndToolbar, rectDlg.right - rect.right, 0, rect.right, rect.bottom, FALSE);
	ShowWindow(hwndToolbar, SW_SHOW);
*/
	return FALSE;
}


/******************************************************************************/
// �c�[���o�[�쐬
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::CreateToolbar()
{
#define NUM_BUTTONS	4
/*	TBBUTTON	tbb[NUM_BUTTONS];
	int		intStr[NUM_BUTTONS];
	TBADDBITMAP tbab;

	// �E�C���h�E�쐬
	hwndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, (LPSTR)NULL, 
		WS_CHILD | TBSTYLE_TOOLTIPS | CCS_ADJUSTABLE | TBSTYLE_LIST, 
		0, 0, 0, 0, GetDlgItem(m_hWnd, IDC_TOOLBAR_PANEL), (HMENU)100, Profile::hInstance, NULL);
	SendMessage(hwndToolbar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS);

	// �{�^���E�r�b�g�}�b�v�쐬
	SendMessage(hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID   = IDB_STD_SMALL_COLOR;
	SendMessage(hwndToolbar, TB_ADDBITMAP, (WPARAM)0, (WPARAM) &tbab);

	// Add the button strings to the toolbar. 
	intStr[0] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"�V�K�쐬");
	intStr[1] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"�폜");
	intStr[2] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"��ֈړ�");
	intStr[3] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"���ֈړ�");

	// Fill the TBBUTTON array with button information, and add the 
	// buttons to the toolbar. 
	tbb[0].iBitmap = STD_FILENEW;
	tbb[0].idCommand = IDM_NEW;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].dwData = 0;
	tbb[0].iString = intStr[0];

	tbb[1].iBitmap = STD_DELETE;
	tbb[1].idCommand = IDM_DEL;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].dwData = 0;
	tbb[1].iString = intStr[1];

	tbb[2].iBitmap = STD_UNDO;
	tbb[2].idCommand = IDM_UP;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].dwData = 0;
	tbb[2].iString = intStr[2];

	tbb[3].iBitmap = STD_REDOW;
	tbb[3].idCommand = IDM_DOWN;
	tbb[3].fsState = TBSTATE_ENABLED;
	tbb[3].fsStyle = TBSTYLE_BUTTON;
	tbb[3].dwData = 0;
	tbb[3].iString = intStr[3];

	SendMessage(hwndToolbar, TB_ADDBUTTONS, (WPARAM)NUM_BUTTONS, (LPARAM)(LPTBBUTTON) &tbb);
	SendMessage(hwndToolbar, TB_AUTOSIZE, 0, 0);
*/	return TRUE;
}


/******************************************************************************/
// Add
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnBtnAdd(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return Add();
}


/******************************************************************************/
// Del
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnBtnDel(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return DeleteItem();
}


/******************************************************************************/
// Up
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnBtnUp(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Move(TRUE);
	SetFocus(hwndList);
	return TRUE;
}


/******************************************************************************/
// Down
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnBtnDown(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Move(FALSE);
	SetFocus(hwndList);
	return TRUE;
}


/******************************************************************************/
// ���X�g�̒ʒm
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::OnListNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR pnmhdr = (LPNMHDR)lParam;

	switch(pnmhdr->code)
	{
		case LVN_KEYDOWN:
		{
			LPNMLVKEYDOWN pnmkey = (LPNMLVKEYDOWN)lParam;
			if(pnmkey->wVKey == VK_DELETE)
			{
				DeleteItem();
			}
			else if(pnmkey->wVKey == VK_INSERT)
			{
				Add();
			}
			else if(pnmkey->wVKey == VK_UP || pnmkey->wVKey == VK_DOWN)
			{
				if(GetKeyState(VK_MENU) & 0x8000)
				{
					Move(pnmkey->wVKey == VK_UP);
				}
			}
		}
	}

	return TRUE;
}


/******************************************************************************/
//		���X�g�p�̃��[�e�B���e�B�[�֐�
/******************************************************************************/
// �A�C�e���I��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::SelectItem(int intIndex)
{
	ListView_SetItemState(hwndList, intIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return TRUE;
}


/******************************************************************************/
// �A�C�e���̕�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string DllDlg::GetItemString(int intItem, int intSubItem)
{
	DWORD dwSize = 2;
	char* pszBuf;
	while(1)
	{
		pszBuf = new char[dwSize];
		ListView_GetItemText(hwndList, intItem, intSubItem, pszBuf, dwSize);
		if(lstrlen(pszBuf) != dwSize - 1)
		{
			break;
		}

		delete[] pszBuf;
		dwSize *= 2;
	}

	string s = pszBuf;
	delete[] pszBuf;
	return s;
}


/******************************************************************************/
// �A�C�e���̒ǉ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::InsertItem(int intIndex, const string& strFileName, BOOL blnChecked)
{
	for(int i = 0; i < ListView_GetItemCount(hwndList); i++)
	{
		if(strFileName == GetItemString(i, 1))
		{
			return FALSE;
		}
	}

	string	str = GetFileName(strFileName);
	BOOL	blnExist = (GetFileAttributes(strFileName.c_str()) != -1);

	LVITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	item.iItem = intIndex;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)str.c_str();
	item.state  = 0;
	item.stateMask = LVIS_CUT;
	item.iImage = (blnExist ? 0 : 1);
	item.lParam = 0;
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)strFileName.c_str());

	if(blnChecked)
	{
		ListView_SetCheckState(hwndList, intIndex, 1);
	}

	return TRUE;
}


/******************************************************************************/
// �ǉ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::Add()
{
	OPENFILENAME of;
	char pszPath[MAX_PATH + 1] = "";
	ZeroMemory(&of, sizeof(of));
	of.lStructSize		= sizeof(OPENFILENAME);
	of.hwndOwner		= m_hWnd;
	of.lpstrFilter		= "*.dll\0*.*\0";
	of.lpstrInitialDir	= Profile::strPluginDir.c_str();
	of.Flags		= OFN_PATHMUSTEXIST;
	of.lpstrFile		= pszPath;
	of.nMaxFile		= MAX_PATH + 1;
	
	if(GetOpenFileName(&of))
	{
		if(!InsertItem(0, of.lpstrFile, TRUE))
		{
			MessageBox(m_hWnd, "���łɓ��� DLL �����X�g���ɑ��݂��Ă܂�YO", "�x��", MB_OK);
		}
	}

	return TRUE;
}


/******************************************************************************/
// �폜
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::DeleteItem()
{
	int intIndex = GetSelectedItem();

	if(intIndex != -1)
	{
		ListView_DeleteItem(hwndList, intIndex);
	}
	SelectItem(intIndex == ListView_GetItemCount(hwndList) ? intIndex - 1 : intIndex);

	return TRUE;
}


/******************************************************************************/
// �ړ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL DllDlg::Move(BOOL blnUp)
{
	int intIndex = GetSelectedItem();

	if(intIndex == -1)
	{
		return FALSE;
	}

	if(blnUp && intIndex != 0 || !blnUp && intIndex < ListView_GetItemCount(hwndList) - 1)
	{
		string strData = GetItemString(intIndex, 1);
		BOOL blnChecked = ListView_GetCheckState(hwndList, intIndex);
		DeleteItem();
		InsertItem(intIndex + (blnUp ? -1 : 1), strData, blnChecked);
		ListView_SetItemState(hwndList, intIndex + (blnUp ? -1 : 1), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

		return TRUE;
	}

	return FALSE;
}


/******************************************************************************/
// �I�����ꂽ�A�C�e�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT DllDlg::GetSelectedItem()
{
	int intCount = ListView_GetItemCount(hwndList);

	for(int i = 0; i < intCount; i++)
	{
		if(ListView_GetItemState(hwndList, i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			return i;
		}
	}

	return -1;
}