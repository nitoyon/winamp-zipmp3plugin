
// AboutDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ABOUT_DLG_H__
#define  __ABOUT_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class AboutDlg
{
private:
	HWND	m_hWnd;

public:
// コンストラクタおよびデストラクタ
	AboutDlg() ;
	~AboutDlg() ;

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP(AboutDlgProc)
	friend LRESULT CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
	BOOL OnOk		(HWND, WPARAM, LPARAM);
	BOOL OnUrlClicked	(HWND, WPARAM, LPARAM);
	BOOL OnCtlColorStatic	(HWND, WPARAM, LPARAM);
} ;

#endif