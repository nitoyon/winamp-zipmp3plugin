
// DllDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __DLL_DLG_H__
#define  __DLL_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class DllDlg : public ChildDlg
{
private:
	HWND	hwndList;
	HWND	hwndToolbar;

public:
// コンストラクタおよびデストラクタ
	DllDlg() ;
	~DllDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
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