
// ChildDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __CHILD_DLG_H__
#define  __CHILD_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ChildDlg
{
protected:
	HWND	m_hWnd ;

private:
	HBRUSH	hBrush;

public:
// コンストラクタおよびデストラクタ
	ChildDlg() ;
	virtual ~ChildDlg() ;

// ウインドウハンドル
	HWND GetHwnd() const{return m_hWnd;}

// 設定適応
	virtual void DoApply();

// メッセージハンドラ
protected:
	virtual BOOL OnCtlColorStatic( HWND, WPARAM, LPARAM) ;
} ;

#endif