
// SettingDlg.cpp
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SettingDlg.h"
#include "Profile.h"
#include "Controller.h"
#include "resource.h"
#include "Util.h"
#include "MainWnd.h"
#include "ListWnd.h"


HWND SettingDlg::hwndStatic = NULL ;
WNDPROC	wpcStatic ;


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

SettingDlg::SettingDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/24(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

SettingDlg::~SettingDlg() 
{
}



/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ定義
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_DLG_MESSAGE_MAP( SettingDlgProc, SettingDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnCancel)

		ON_COMMAND( IDC_CHK_ID3		, OnCheck)
		ON_COMMAND( IDC_SHOW_ONLY_ZIP	, OnCheck)
		ON_COMMAND( IDC_CHK_COMPI	, OnCheck)
		ON_COMMAND( IDC_NORMAL_VARIABLE	, OnBtnNormal)
		ON_COMMAND( IDC_ID3_VARIABLE	, OnBtnID3)
		ON_COMMAND( IDC_COMPI_VARIABLE	, OnBtnID3)

		ON_COMMAND( IDC_URL		, OnUrlClicked)
		ON_COMMAND( IDC_BMP1		, OnBmpClicked)
		ON_COMMAND( IDC_BMP2		, OnBmpClicked)
	END_COMMAND_MAP()
	ON_COMMAND( WM_SETCURSOR		, OnSetCursor)
	ON_COMMAND( WM_CTLCOLORSTATIC		, OnCtlColorStatic)
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// 初期化
//============================================================================//
// 更新：03/04/20(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	if( hwndStatic != 0 && IsWindow( hwndStatic))
	{
		EndDialog( hDlg, FALSE) ;
		SetFocus( hwndStatic) ;
		return TRUE ;
	}

	// サブクラス化
	wpcStatic = (WNDPROC)GetWindowLong( GetDlgItem( hDlg, IDC_URL), GWL_WNDPROC) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_URL) , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_BMP1), GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_BMP2), GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

	// チェック
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_ZIP, Profile::blnShowOnlyZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_UNCOMPRESS, Profile::blnShowOnlyUncompressedZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CHK_ID3, Profile::blnListID3 ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CHK_COMPI, Profile::blnListCompilation ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_TIMEBAR, Profile::blnShowTimebar ? BST_CHECKED : 0) ;

	// テキスト設定
	SetDlgItemText( hDlg, IDC_LIST_NORMAL, Profile::strListNormal.c_str()) ;
	SetDlgItemText( hDlg, IDC_LIST_ID3, Profile::strListID3.c_str()) ;
	SetDlgItemText( hDlg, IDC_LIST_COMPI, Profile::strListCompilation.c_str()) ;

	// フォント列挙
	HDC hdc = GetWindowDC( Controller::GetInstance()->GetWindow()->GetHwnd()) ;
	LOGFONT lf ;
	memset( &lf, 0, sizeof( LOGFONT)) ;
	lf.lfCharSet = SHIFTJIS_CHARSET ;//DEFAULT_CHARSET ;
	EnumFontFamiliesEx( hdc, &lf, (FONTENUMPROC)FontFamEnumProc, (LPARAM)this, 0) ;
	HWND hwndCombo = GetDlgItem( hDlg, IDC_FONT) ;
	int intIndex = SendMessage( hwndCombo, CB_FINDSTRING, 0, (LPARAM)Profile::strFont.c_str()) ;
	if( intIndex != CB_ERR)
	{
		SendMessage( hwndCombo, CB_SETCURSEL, intIndex, 0) ;
	}
	else
	{
		SendMessage( hwndCombo, CB_SETCURSEL, 0, 0) ;
	}

	// フォントサイズ設定
	SetFontSize() ;

	Validiate() ;
	hwndStatic = hDlg ;
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 旧データを一時的に保存
	int intOldFontSize = Profile::intFontSize ;
	string strOldFont = Profile::strFont ;

	// ダイアログから読みとり
	Profile::blnShowOnlyZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_ZIP) ? TRUE : FALSE ;
	Profile::blnShowOnlyUncompressedZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_UNCOMPRESS) ? TRUE : FALSE ;
	Profile::blnShowTimebar = IsDlgButtonChecked( hDlg, IDC_TIMEBAR) ? TRUE : FALSE ;
	Profile::strListNormal = GetWindowString( GetDlgItem( hDlg, IDC_LIST_NORMAL)) ;
	Profile::strListID3 = GetWindowString( GetDlgItem( hDlg, IDC_LIST_ID3)) ;
	Profile::blnListID3 = IsDlgButtonChecked( hDlg, IDC_CHK_ID3) ? TRUE : FALSE ;
	Profile::strListCompilation = GetWindowString( GetDlgItem( hDlg, IDC_LIST_COMPI)) ;
	Profile::blnListCompilation = IsDlgButtonChecked( hDlg, IDC_CHK_COMPI) ? TRUE : FALSE ;
	Profile::strFont = GetWindowString( GetDlgItem( hDlg, IDC_FONT)) ;
	Profile::intFontSize = atoi(GetWindowString(GetDlgItem(hDlg, IDC_FONTSIZE)).c_str());
	Profile::Save() ;

	if(strOldFont != Profile::strFont || intOldFontSize != Profile::intFontSize)
	{
		Controller::GetInstance()->GetWindow()->GetListWnd()->SetFont() ;
	}

	EndDialog( hDlg, TRUE) ;
	hwndStatic = NULL ;
	return TRUE ;
}


/******************************************************************************/
// キャンセル
//============================================================================//
// 更新：02/12/29(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE) ;
	hwndStatic = NULL ;
	return TRUE ;
}


/******************************************************************************/
// チェックする
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnCheck( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Validiate() ;
	return TRUE ;
}


/******************************************************************************/
// 変数ボタン
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnBtnNormal( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\tファイル名。ディレクトリは除く。") ;
	vec.push_back( "%FILE_PATH%\tディレクトリをふくんだファイル名。") ;

	VarMenu( vec, IDC_LIST_NORMAL, IDC_NORMAL_VARIABLE) ;

	return TRUE ;
}


/******************************************************************************/
// 変数ボタン
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnBtnID3( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\tファイル名。ディレクトリは除く。") ;
	vec.push_back( "%FILE_PATH%\tディレクトリをふくんだファイル名。") ;
	vec.push_back( "") ;
	vec.push_back( "%TRACK_NUMBER%\tトラック番号。") ;
	vec.push_back( "%TRACK_NUMBER2%\t２桁以上のトラック番号。") ;
	vec.push_back( "%TRACK_NUMBER3%\t３桁以上のトラック番号。") ;
	vec.push_back( "%TRACK_NAME%\t曲名。") ;
	vec.push_back( "%ARTIST_NAME%\tアーティスト名。") ;
	vec.push_back( "%ALBUM_NAME%\tアルバム名。") ;
	vec.push_back( "%YEAR%\t年") ;
	vec.push_back( "%COMMENT%\tコメント") ;

	if( LOWORD( wParam) == IDC_ID3_VARIABLE)
	{
		VarMenu( vec, IDC_LIST_ID3, IDC_ID3_VARIABLE) ;
	}
	else
	{
		VarMenu( vec, IDC_LIST_COMPI, IDC_COMPI_VARIABLE) ;
	}

	return TRUE ;
}


/******************************************************************************/
// フォントファミリー列挙プロシージャ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int CALLBACK FontFamEnumProc( ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, int FontType, LPARAM lParam)
{
	SettingDlg* p = (SettingDlg*)lParam ;
	HWND hwndCombo = GetDlgItem( p->m_hWnd, IDC_FONT) ;

	if( SendMessage( hwndCombo, CB_FINDSTRING, 0, (LPARAM)lpelf->elfLogFont.lfFaceName) == CB_ERR)
	{
		SendMessage( hwndCombo, CB_ADDSTRING, 0, (LPARAM)lpelf->elfLogFont.lfFaceName) ;
	}

	return 1 ;
}


/******************************************************************************/
// URL をクリック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnUrlClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.nitoyon.com/", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// BMP をクリック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnBmpClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.ai.wakwak.com/~cik/cccd.htm", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// カーソル
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnSetCursor( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HCURSOR hCursor = LoadCursor( NULL, IDC_ARROW) ;
	SetCursor( hCursor);

	return TRUE ;
}


/******************************************************************************/
// スタティックのカラー
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnCtlColorStatic( HWND hDlg, WPARAM wParam, LPARAM lParam)
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


/******************************************************************************/
//		その他
/******************************************************************************/
// フォントサイズ設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SettingDlg::SetFontSize()
{
	// コンボ初期化
	HWND hwndCombo = GetDlgItem( m_hWnd, IDC_FONT) ;
	HWND hwndFontSize = GetDlgItem( m_hWnd, IDC_FONTSIZE) ;
	while( SendMessage( hwndFontSize, CB_GETCOUNT, 0, 0) != 0)
	{
		SendMessage( hwndFontSize, CB_DELETESTRING, 0, 0) ;
	}

	// フォントサイズ追加
	SendMessage(hwndFontSize, CB_ADDSTRING, 0, (LPARAM)"Winamp のフォントサイズ");
	if(Profile::intFontSize == 0)
	{
		SendMessage(hwndFontSize, CB_SETCURSEL, 0, 0);
	}

	for(int i = 6; i < 25; i++)
	{
		char pszBuf[3];
		wsprintf(pszBuf, "%d", i);
		SendMessage(hwndFontSize, CB_ADDSTRING, 0, (LPARAM)pszBuf);

		if(i == Profile::intFontSize)
		{
			SendMessage(hwndFontSize, CB_SETCURSEL, i - 5, 0);
		}
	}
}


/******************************************************************************/
// 正当化
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

void SettingDlg::Validiate()
{
	EnableWindow( GetDlgItem( m_hWnd, IDC_SHOW_ONLY_UNCOMPRESS), IsDlgButtonChecked( m_hWnd, IDC_SHOW_ONLY_ZIP) ? TRUE : FALSE) ;

	// IDv3 タグがある場合
	BOOL b = IsDlgButtonChecked( m_hWnd, IDC_CHK_ID3) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_LIST_ID3), b ? TRUE : FALSE) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_ID3_VARIABLE), b ? TRUE : FALSE) ;

	// コンピレーション
	if( b)
	{
		EnableWindow( GetDlgItem( m_hWnd, IDC_CHK_COMPI), TRUE) ;
		BOOL b2 = IsDlgButtonChecked( m_hWnd, IDC_CHK_COMPI) ;
		EnableWindow( GetDlgItem( m_hWnd, IDC_LIST_COMPI), b2 ? TRUE : FALSE) ;
		EnableWindow( GetDlgItem( m_hWnd, IDC_COMPI_VARIABLE), b2 ? TRUE : FALSE) ;
	}
	else
	{
		EnableWindow( GetDlgItem( m_hWnd, IDC_CHK_COMPI), FALSE) ;
		EnableWindow( GetDlgItem( m_hWnd, IDC_LIST_COMPI), FALSE) ;
		EnableWindow( GetDlgItem( m_hWnd, IDC_COMPI_VARIABLE), FALSE) ;
	}
}


/******************************************************************************/
// ポップアップメニュー表示
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

void SettingDlg::VarMenu( vector<string>& vec, int intCtrlEdt, int intCtrlBtn)
{
	// メニュー作成
	HMENU  hmenu = CreateMenu() ;
	for( int i = 0; i < vec.size(); i++)
	{
		if( vec[ i] == "")
		{
			AppendMenu( hmenu, MF_SEPARATOR, i + 1, NULL) ;
		}
		else
		{
			AppendMenu( hmenu, MF_STRING, i + 1, vec[ i].c_str()) ;
		}
	}
	HMENU  hmnPopup = CreateMenu() ;
	AppendMenu( hmnPopup, MF_POPUP, (UINT)hmenu, "") ;

	// 表示
	RECT rc ;
	GetWindowRect( GetDlgItem( m_hWnd, intCtrlBtn), &rc) ;
	UINT intRet = TrackPopupMenu( hmenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, rc.right, rc.top, 
			0, m_hWnd, NULL) ;
	if( intRet != 0 && vec[ intRet - 1] != "")
	{
		string s = vec[ intRet - 1] ;
		s = s.substr( 0, s.find( '\t')) ;
		HWND h = GetDlgItem( m_hWnd, intCtrlEdt) ;
		SendMessage( h, EM_REPLACESEL, (LPARAM)TRUE, (WPARAM)s.c_str()) ;
	}

	DestroyMenu( hmenu) ;
	DestroyMenu( hmnPopup) ;
}


