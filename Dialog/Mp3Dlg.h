
// Mp3Dlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MP3_DLG_H__
#define  __MP3_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Mp3Dlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	Mp3Dlg() ;
	~Mp3Dlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(Mp3DlgProc)

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnChecked		(HWND, WPARAM, LPARAM);
	void SetEnable();
} ;

#endif