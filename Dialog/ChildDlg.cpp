
// ChildDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ChildDlg.h"
#include "..\resource.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ChildDlg::ChildDlg() 
: m_hWnd(0), hBrush(NULL)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ChildDlg::~ChildDlg() 
{
	if(hBrush)
	{
		DeleteObject(hBrush);
	}
}


/******************************************************************************/
// 適応
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void ChildDlg::DoApply()
{
	return;
}


/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// スタティックの色
//============================================================================//
// 更新：03/05/29(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ChildDlg::OnCtlColorStatic(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;
	if(GetDlgItem(hDlg, IDC_TITLE) == (HWND)lParam)
	{
		// 赤色
		SetTextColor(hdc, RGB(0, 0, 0)) ;
		SetBkColor( hdc, RGB( 255, 238, 238)) ;

		if(!hBrush)
		{
			hBrush = CreateSolidBrush(RGB(255, 238, 238));
		}
		
		return (BOOL)hBrush;
	}

	return FALSE;
}

