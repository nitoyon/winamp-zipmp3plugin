
// SkinDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __SKIN_DLG_H__
#define  __SKIN_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class SkinDlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	SkinDlg() ;
	~SkinDlg() ;

	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(SkinDlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnRadioChanged	(HWND, WPARAM, LPARAM);

	void SetEnable();
} ;

#endif