
// SettingDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SettingDlg.h"
#include "..\Profile.h"
#include "..\util.h"
#include "..\resource.h"
#include "WindowDlg.h"
#include "FontDlg.h"
#include "SkinDlg.h"
#include "FontDlg.h"
#include "Mp3Dlg.h"
#include "ListDlg.h"
#include "DllDlg.h"
#include "KeyboardDlg.h"
#include "NullDlg.h"
#include "AboutDlg.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

SettingDlg::SettingDlg(HINSTANCE h)
: hInstance(h), intCurDlg(-1)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/24(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

SettingDlg::~SettingDlg() 
{
	for(int i = 0; i < vecChildDlg.size(); i++)
	{
		if(vecChildDlg[i])
		{
			delete vecChildDlg[i];
			vecChildDlg[i] = NULL;
		}
	}
}



/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ定義
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_DLG_MESSAGE_MAP( SettingDlgProc, SettingDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	BEGIN_NOTIFY_MAP()
		ON_NOTIFY(IDC_TREE	, OnTreeNotify)
	END_NOTIFY_MAP()
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnCancel)
		ON_COMMAND( IDC_APPLY		, OnApply)
		ON_COMMAND( IDC_ABOUT		, OnAbout)
	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// 初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// ウインドウハンドル取得
	hwndTree = GetDlgItem(hDlg, IDC_TREE);
	HWND hwndPanel = GetDlgItem(hDlg, IDC_PANEL);
	SetFocus(hwndTree);

	// ダイアログのリストアップ
	LPCSTR ppszDlgResName[] = {
		MAKEINTRESOURCE(IDD_WINDOW), 
		MAKEINTRESOURCE(IDD_FONT), 
		MAKEINTRESOURCE(IDD_SKIN), 
		MAKEINTRESOURCE(IDD_MP3), 
		MAKEINTRESOURCE(IDD_LIST), 
		MAKEINTRESOURCE(IDD_DLL), 
		MAKEINTRESOURCE(IDD_UNAVAILABLE), 
		MAKEINTRESOURCE(IDD_KEYBOARD), 
		MAKEINTRESOURCE(IDD_UNAVAILABLE)
	};
	DLGPROC pDlgProc[] = {
		WindowDlgProc, 
		FontDlgProc, 
		SkinDlgProc, 
		Mp3DlgProc, 
		ListDlgProc, 
		DllDlgProc, 
		NullDlgProc, 
		KeyboardDlgProc, 
		NullDlgProc
	};
	vecChildDlg.push_back(new WindowDlg());
	vecChildDlg.push_back(new FontDlg());
	vecChildDlg.push_back(new SkinDlg());
	vecChildDlg.push_back(new Mp3Dlg());
	vecChildDlg.push_back(new ListDlg());
	vecChildDlg.push_back(new DllDlg());
	vecChildDlg.push_back(new NullDlg());
	vecChildDlg.push_back(new KeyboardDlg());
	vecChildDlg.push_back(new NullDlg());
	
	// ウインドウ作成
	RECT rcPanel = GetChildRect(m_hWnd, hwndPanel);
	for(int i = 0; i < vecChildDlg.size(); i++)
	{
		HWND h = CreateDialogParam(hInstance, ppszDlgResName[i], m_hWnd, pDlgProc[i], (LPARAM)vecChildDlg[i]);
		ShowWindow(h, SW_HIDE);

		SetWindowPos(h, NULL, rcPanel.left, rcPanel.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	// イメージリスト作成
	hImgList = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR32, 3, 3);
	HICON hicon;
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOW));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FONT));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SKIN));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MP3));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIST));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLL));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPERT));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYBOARD));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	hicon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATH));
	if(hicon) ImageList_AddIcon(hImgList, hicon);
	TreeView_SetImageList(hwndTree, hImgList, TVSIL_NORMAL);

	// ツリービューにアイテム追加
	TVINSERTSTRUCT tvis;
	HTREEITEM hitem;
	tvis.hParent		= TVI_ROOT;
	tvis.hInsertAfter	= TVI_LAST;
	tvis.item.mask		= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvis.item.lParam	= 0;
	tvis.item.pszText	= "ウインドウ";
	tvis.item.iImage	= 0;
	tvis.item.iSelectedImage= 0;
	hitem = TreeView_InsertItem(hwndTree, &tvis);

	tvis.hParent		= hitem;
	tvis.item.lParam	= 1;
	tvis.item.pszText	= "フォント";
	tvis.item.iImage	= 1;
	tvis.item.iSelectedImage= 1;
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.item.lParam	= 2;
	tvis.item.pszText	= "スキン";
	tvis.item.iImage	= 2;
	tvis.item.iSelectedImage= 2;
	TreeView_InsertItem(hwndTree, &tvis);
	TreeView_Select(hwndTree, hitem, TVGN_CARET); // 選択する
	TreeView_Expand(hwndTree, hitem, TVE_EXPAND); // 開く

	tvis.hParent		= TVI_ROOT;
	tvis.item.lParam	= 3;
	tvis.item.pszText	= "MP3";
	tvis.item.iImage	= 3;
	tvis.item.iSelectedImage= 3;
	hitem = TreeView_InsertItem(hwndTree, &tvis);

	tvis.hParent		= hitem;
	tvis.hInsertAfter	= TVI_LAST;
	tvis.item.lParam	= 4;
	tvis.item.pszText	= "表示する文字";
	tvis.item.iImage	= 4;
	tvis.item.iSelectedImage= 4;
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.hInsertAfter	= TVI_LAST;
	tvis.item.lParam	= 5;
	tvis.item.pszText	= "読み取りDLL";
	tvis.item.iImage	= 5;
	tvis.item.iSelectedImage= 5;
	TreeView_InsertItem(hwndTree, &tvis);
	TreeView_Expand(hwndTree, hitem, TVE_EXPAND); // 開く

	tvis.hParent		= TVI_ROOT;
	tvis.item.lParam	= 6;
	tvis.item.pszText	= "高度な設定";
	tvis.item.iImage	= 6;
	tvis.item.iSelectedImage= 6;
	hitem = TreeView_InsertItem(hwndTree, &tvis);

	tvis.hParent		= hitem;
	tvis.hInsertAfter	= TVI_LAST;
	tvis.item.lParam	= 7;
	tvis.item.pszText	= "キーボード";
	tvis.item.iImage	= 7;
	tvis.item.iSelectedImage= 7;
	TreeView_InsertItem(hwndTree, &tvis);

	tvis.hParent		= hitem;
	tvis.hInsertAfter	= TVI_LAST;
	tvis.item.lParam	= 8;
	tvis.item.pszText	= "パス";
	tvis.item.iImage	= 8;
	tvis.item.iSelectedImage= 8;
	TreeView_InsertItem(hwndTree, &tvis);
	TreeView_Expand(hwndTree, hitem, TVE_EXPAND); // 開く

	// ウインドウ表示
	DisplayDlg(0);

	return FALSE;
}


/******************************************************************************/
// ツリービューの通知メッセージ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnTreeNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	NMHDR* pnmhdr = (LPNMHDR)lParam;
	if(pnmhdr->idFrom == IDC_TREE)
	{
		switch(pnmhdr->code)
		{
			case TVN_SELCHANGED:
			{
				NMTREEVIEW* pnmtv = (NMTREEVIEW*)lParam;
				DisplayDlg(pnmtv->itemNew.lParam);
				return TRUE;
			}
		}
	}

	return FALSE;
}


/******************************************************************************/
// OK
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	OnApply(hDlg, 0, 0);
	EndDialog( hDlg, TRUE) ;
	return TRUE ;
}


/******************************************************************************/
// キャンセル
//============================================================================//
// 更新：02/12/29(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE);
	return TRUE;
}


/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnApply(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 設定適応
	for(int i = 0; i < vecChildDlg.size(); i++)
	{
		if(vecChildDlg[i])
		{
			vecChildDlg[i]->DoApply();
		}
	}

	Profile::Save();
	return TRUE;
}


/******************************************************************************/
// バージョン情報
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnAbout(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	AboutDlg ad;
	DialogBoxParam(Profile::hInstance, MAKEINTRESOURCE(IDD_ABOUT), hDlg, AboutDlgProc, (LPARAM)&ad);
	return TRUE;
}


/******************************************************************************/
// ウインドウを表示する
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SettingDlg::DisplayDlg(int i)
{
	if(i < 0 || i >= vecChildDlg.size() || i == intCurDlg)
	{
		return;
	}

	// ウインドウ 表示
	ShowWindow(vecChildDlg[i]->GetHwnd(), SW_SHOW);
	if(intCurDlg != -1)
	{
		ShowWindow(vecChildDlg[intCurDlg]->GetHwnd(), SW_HIDE);
	}

	intCurDlg = i;
}
