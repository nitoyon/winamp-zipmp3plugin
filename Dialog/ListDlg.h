
// ListDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __LIST_DLG_H__
#define  __LIST_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ListDlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	ListDlg() ;
	~ListDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(ListDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnChecked		(HWND, WPARAM, LPARAM);
	BOOL OnClickBtn		(HWND, WPARAM, LPARAM);

	void SetEnable();
	void DisplayMenu(const vector<string>&, int);
};

#endif