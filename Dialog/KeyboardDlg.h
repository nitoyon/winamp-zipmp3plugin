
// KeyboardDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __KEYBOARD_DLG_H__
#define  __KEYBOARD_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class KeyboardDlg : public ChildDlg
{
public:
// コンストラクタおよびデストラクタ
	KeyboardDlg() ;
	~KeyboardDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(KeyboardDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif