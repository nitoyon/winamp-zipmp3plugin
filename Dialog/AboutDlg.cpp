
// AboutDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "AboutDlg.h"
#include "..\resource.h"
#include "..\Profile.h"


/******************************************************************************/
//		グローバル変数
/******************************************************************************/

WNDPROC	wpcStatic ;


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

AboutDlg::AboutDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

AboutDlg::~AboutDlg() 
{
}



/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_DLG_MESSAGE_MAP(AboutDlgProc, AboutDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnOk)
		ON_COMMAND( IDC_URL		, OnUrlClicked)
	END_COMMAND_MAP()
	ON_COMMAND( WM_CTLCOLORSTATIC		, OnCtlColorStatic)
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL AboutDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// サブクラス化
	wpcStatic = (WNDPROC)GetWindowLong( GetDlgItem( hDlg, IDC_URL), GWL_WNDPROC) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_URL) , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

	CheckDlgButton(hDlg, IDC_USE_ID3V2, Profile::blnUseId3v2 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_USE_CUE,Profile::blnUseCue ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_USE_MP3CUE, Profile::blnUseMp3Cue ? BST_CHECKED : BST_UNCHECKED);

	return FALSE;
}


/******************************************************************************/
// OK
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL AboutDlg::OnOk(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hDlg, TRUE);
	return TRUE;
}


/******************************************************************************/
// URL をクリック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL AboutDlg::OnUrlClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.nitoyon.com/", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// スタティックのカラー
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL AboutDlg::OnCtlColorStatic( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam ;
	if( GetDlgItem( hDlg, IDC_URL) == (HWND)lParam)
	{
		SetTextColor( hdc, RGB( 0, 0, 255)) ;
		SetBkMode(hdc, TRANSPARENT);
		return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH) ;
	}

	return FALSE ;
}


/******************************************************************************/
// リンクのためのサブクラス化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uiMsg)
	{
		case WM_SETCURSOR :
		{
			// 手カーソル読みとり
			HCURSOR hCurHand	= LoadCursor( Profile::hInstance, MAKEINTRESOURCE( IDC_HANDCUR)) ;
			SetCursor( hCurHand) ;
			return 0 ;
		}
	}

	return CallWindowProc( wpcStatic, hWnd, uiMsg, wParam, lParam) ;
}
