
// KeyboardDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "KeyboardDlg.h"
#include "..\Profile.h"
#include "..\resource.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

KeyboardDlg::KeyboardDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

KeyboardDlg::~KeyboardDlg() 
{
}



/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_DLG_MESSAGE_MAP(KeyboardDlgProc, KeyboardDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL KeyboardDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList = GetDlgItem(hDlg, IDC_KEYBOARDLIST);

	// コラム追加
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
	lvcol.pszText = "キーボード";
	lvcol.cx = 120;
	ListView_InsertColumn(hwndList, 0, &lvcol);

	lvcol.pszText = "内容";
	lvcol.cx = 180;
	ListView_InsertColumn(hwndList, 1, &lvcol);

	// アイテム追加
	LVITEM item;
	int intIndex = 0;
	item.mask = LVIF_TEXT;
	item.iItem = intIndex;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)"Alt+M";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"ウインドウ表示/非表示切り替え");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"Alt + 3";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"ファイル情報表示");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)", (カンマ)";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"アルバム内で１曲戻る");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)". (ピリオド)";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"アルバム内で１曲進む");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"←";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"５秒戻る");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"→";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"５秒進む");
	intIndex++;

	item.iItem = intIndex;
	item.pszText = (LPTSTR)"アプリケーションキー";
	ListView_InsertItem(hwndList, &item);
	ListView_SetItemText(hwndList, intIndex, 1, (LPTSTR)"メニュー表示");
	intIndex++;

	return FALSE;
}


/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void KeyboardDlg::DoApply()
{
}