
// NullDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __NULL_DLG_H__
#define  __NULL_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class NullDlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	NullDlg() ;
	~NullDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(NullDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif