
// WindowDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __WINDOW_DLG_H__
#define  __WINDOW_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class WindowDlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	WindowDlg() ;
	~WindowDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(WindowDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif