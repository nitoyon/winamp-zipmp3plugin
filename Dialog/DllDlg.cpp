
// DllDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "DllDlg.h"
#include "..\resource.h"
#include "..\util.h"
#include "..\Profile.h"


/******************************************************************************/
//		定義
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
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DllDlg::DllDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DllDlg::~DllDlg() 
{
}


/******************************************************************************/
//		適用
/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void DllDlg::DoApply()
{
	int intCount = ListView_GetItemCount(hwndList);
	Profile::vecHeaderDll.clear();
	Profile::vecUseHeaderDll.clear();

	for(int i = 0; i < intCount; i++)
	{
		// パス設定
		Profile::vecHeaderDll.push_back(GetItemString(i, 1));

		// 有効無効
		Profile::vecUseHeaderDll.push_back(ListView_GetCheckState(hwndList, i));
	}
}


/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	hInstance = Profile::hInstance;
	hwndList = GetDlgItem(hDlg, IDC_DLL_LIST);
	ListView_SetExtendedListViewStyleEx(hwndList, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	// コラム追加
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
	lvcol.pszText = "DLL 名";
	lvcol.cx = 100;
	ListView_InsertColumn(hwndList, 0, &lvcol);

	lvcol.pszText = "パス";
	lvcol.cx = 200;
	ListView_InsertColumn(hwndList, 1, &lvcol);

	// イメージ追加
	hImgList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);
	HICON hicon;
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLL_FOUND));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLL_NOTFOUND));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	ListView_SetImageList(hwndList, hImgList, LVSIL_SMALL);

	// アイテム追加
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
// ツールバー作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::CreateToolbar()
{
#define NUM_BUTTONS	4
/*	TBBUTTON	tbb[NUM_BUTTONS];
	int		intStr[NUM_BUTTONS];
	TBADDBITMAP tbab;

	// ウインドウ作成
	hwndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, (LPSTR)NULL, 
		WS_CHILD | TBSTYLE_TOOLTIPS | CCS_ADJUSTABLE | TBSTYLE_LIST, 
		0, 0, 0, 0, GetDlgItem(m_hWnd, IDC_TOOLBAR_PANEL), (HMENU)100, Profile::hInstance, NULL);
	SendMessage(hwndToolbar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS);

	// ボタン・ビットマップ作成
	SendMessage(hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID   = IDB_STD_SMALL_COLOR;
	SendMessage(hwndToolbar, TB_ADDBITMAP, (WPARAM)0, (WPARAM) &tbab);

	// Add the button strings to the toolbar. 
	intStr[0] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"新規作成");
	intStr[1] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"削除");
	intStr[2] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"上へ移動");
	intStr[3] = SendMessage(hwndToolbar, TB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"下へ移動");

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
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::OnBtnAdd(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return Add();
}


/******************************************************************************/
// Del
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::OnBtnDel(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return DeleteItem();
}


/******************************************************************************/
// Up
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::OnBtnDown(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Move(FALSE);
	SetFocus(hwndList);
	return TRUE;
}


/******************************************************************************/
// リストの通知
//============================================================================//
// 概要：なし。
// 補足：なし。
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
//		リスト用のユーティリティー関数
/******************************************************************************/
// アイテム選択
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL DllDlg::SelectItem(int intIndex)
{
	ListView_SetItemState(hwndList, intIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	return TRUE;
}


/******************************************************************************/
// アイテムの文字列取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// アイテムの追加
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 追加
//============================================================================//
// 概要：なし。
// 補足：なし。
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
			MessageBox(m_hWnd, "すでに同じ DLL がリスト内に存在してますYO", "警告", MB_OK);
		}
	}

	return TRUE;
}


/******************************************************************************/
// 削除
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 移動
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 選択されたアイテムを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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