
// FontDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __FONT_DLG_H__
#define  __FONT_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class FontDlg : public ChildDlg
{
private:

public:
// コンストラクタおよびデストラクタ
	FontDlg() ;
	~FontDlg() ;

// 適用
	void DoApply();

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(FontDlgProc)
	friend int CALLBACK FontFamEnumProc(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, int FontType, LPARAM lParam);

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif