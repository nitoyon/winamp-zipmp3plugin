
// SettingDlg.cpp
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SettingDlg.h"
#include "Profile.h"
#include "Controller.h"
#include "resource.h"
#include "Util.h"


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
// 更新：02/12/28(土)
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
		ON_COMMAND( IDC_NORMAL_VARIABLE	, OnBtnNormal)
		ON_COMMAND( IDC_ID3_VARIABLE	, OnBtnID3)
	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// 初期化
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// ウインドウハンドル取得
	hwndHotKey	= GetDlgItem( hDlg, IDC_HOTKEY) ;

	// チェック
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_ZIP, Profile::blnShowOnlyZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_UNCOMPRESS, Profile::blnShowOnlyUncompressedZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CHK_ID3, Profile::blnListID3 ? BST_CHECKED : 0) ;

	// ホットキー
	SendMessage( hwndHotKey, HKM_SETHOTKEY, Profile::wrdHotKey, 0) ;

	// テキスト設定
	SetDlgItemText( hDlg, IDC_LIST_NORMAL, Profile::strListNormal.c_str()) ;
	SetDlgItemText( hDlg, IDC_LIST_ID3, Profile::strListID3.c_str()) ;

	Validiate() ;
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// ホットキー設定
	WORD w = SendMessage( hwndHotKey, HKM_GETHOTKEY, 0, 0) ;
	switch( Controller::GetInstance()->SetHotKey( w))
	{
		case 1 :
			break ;
		case 0 :
			MessageBox( hDlg, "ウインドウハンドルが適切ではありませんでした", "ホットキー割り当ての失敗", MB_OK) ;
			return TRUE ;
		case -1 :
			MessageBox( hDlg, "ホットキーが不適切でした。", "ホットキー割り当ての失敗", MB_OK) ;
			return TRUE ;
		case 2 :
			MessageBox( hDlg, "他のアプリケーションが既に使っています。", "ホットキー割り当ての失敗", MB_OK) ;
			return TRUE ;
	}

	// ダイアログから読みとり
	Profile::blnShowOnlyZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_ZIP) ? TRUE : FALSE ;
	Profile::blnShowOnlyUncompressedZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_UNCOMPRESS) ? TRUE : FALSE ;
	Profile::wrdHotKey = SendMessage( hwndHotKey, HKM_GETHOTKEY, 0, 0) ;
	Profile::strListNormal = GetWindowString( GetDlgItem( hDlg, IDC_LIST_NORMAL)) ;
	Profile::strListID3 = GetWindowString( GetDlgItem( hDlg, IDC_LIST_ID3)) ;
	Profile::blnListID3 = IsDlgButtonChecked( hDlg, IDC_CHK_ID3) ? TRUE : FALSE ;
	Profile::Save() ;

	EndDialog( hDlg, TRUE) ;
	return TRUE ;
}


/******************************************************************************/
// キャンセル
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SettingDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE) ;
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

	VarMenu( vec, IDC_LIST_ID3, IDC_ID3_VARIABLE) ;

	return TRUE ;
}


/******************************************************************************/
//		その他
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
	BOOL b = IsDlgButtonChecked( m_hWnd, IDC_CHK_ID3) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_LIST_ID3), b ? TRUE : FALSE) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_ID3_VARIABLE), b ? TRUE : FALSE) ;
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