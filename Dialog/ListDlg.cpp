
// ListDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ListDlg.h"
#include "..\resource.h"
#include "..\Profile.h"
#include "..\util.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ListDlg::ListDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ListDlg::~ListDlg() 
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

BEGIN_DLG_MESSAGE_MAP(ListDlgProc, ListDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDC_CHK_ID3		, OnChecked)
		ON_COMMAND( IDC_CHK_COMPI	, OnChecked)
		ON_COMMAND( IDC_NORMAL_VARIABLE	, OnClickBtn)
		ON_COMMAND( IDC_ID3_VARIABLE	, OnClickBtn)
		ON_COMMAND( IDC_COMPI_VARIABLE	, OnClickBtn)
	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ListDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// チェックボックス
	CheckDlgButton(hDlg, IDC_CHK_ID3, 	Profile::blnListID3 ? BST_CHECKED : 0);
	CheckDlgButton(hDlg, IDC_CHK_COMPI, 	Profile::blnListCompilation ? BST_CHECKED : 0);

	// エディットボックス
	SetDlgItemText(hDlg, IDC_LIST_NORMAL,	Profile::strListNormal.c_str());
	SetDlgItemText(hDlg, IDC_LIST_ID3,	Profile::strListID3.c_str());
	SetDlgItemText(hDlg, IDC_LIST_COMPI,	Profile::strListCompilation.c_str());

	SetEnable();
	return FALSE;
}


/******************************************************************************/
// チェックされたとき
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ListDlg::OnChecked(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	SetEnable();
	return TRUE;
}


/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListDlg::DoApply()
{
	Profile::blnListID3		= IsDlgButtonChecked(m_hWnd, IDC_CHK_ID3);
	Profile::blnListCompilation	= IsDlgButtonChecked(m_hWnd, IDC_CHK_COMPI);

	Profile::strListNormal		= GetWindowString(GetDlgItem(m_hWnd, IDC_LIST_NORMAL));
	Profile::strListID3		= GetWindowString(GetDlgItem(m_hWnd, IDC_LIST_ID3));
	Profile::strListCompilation	= GetWindowString(GetDlgItem(m_hWnd, IDC_LIST_COMPI));
}


/******************************************************************************/
// 有効無効の切り替え
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListDlg::SetEnable()
{
	BOOL b1 = IsDlgButtonChecked(m_hWnd, IDC_CHK_ID3);
	BOOL b2 = b1 && IsDlgButtonChecked(m_hWnd, IDC_CHK_COMPI);

	EnableWindow(GetDlgItem(m_hWnd, IDC_LIST_ID3), b1);
	EnableWindow(GetDlgItem(m_hWnd, IDC_ID3_VARIABLE), b1);

	EnableWindow(GetDlgItem(m_hWnd, IDC_LIST_COMPI), b2);
	EnableWindow(GetDlgItem(m_hWnd, IDC_COMPI_VARIABLE), b2);
}


/******************************************************************************/
// メニュー表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ListDlg::OnClickBtn(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// メニューの文字列設定
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\tファイル名。ディレクトリは除く。") ;
	vec.push_back( "%FILE_PATH%\tディレクトリをふくんだファイル名。") ;

	if(LOWORD(wParam) != IDC_NORMAL_VARIABLE)
	{
		vec.push_back( "") ;
		vec.push_back( "%TRACK_NUMBER%\tトラック番号。") ;
		vec.push_back( "%TRACK_NUMBER2%\t２桁以上のトラック番号。") ;
		vec.push_back( "%TRACK_NUMBER3%\t３桁以上のトラック番号。") ;
		vec.push_back( "%TRACK_NAME%\t曲名。") ;
		vec.push_back( "%ARTIST_NAME%\tアーティスト名。") ;
		vec.push_back( "%ALBUM_NAME%\tアルバム名。") ;
		vec.push_back( "%YEAR%\t年") ;
		vec.push_back( "%COMMENT%\tコメント") ;
	}

	// 表示
	DisplayMenu(vec, LOWORD(wParam));

	return TRUE ;
}


/******************************************************************************/
// メニュー表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListDlg::DisplayMenu(const vector<string>& vec, int intCtrlBtn)
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
		HWND h = GetDlgItem( m_hWnd, intCtrlBtn - 1) ;
		SendMessage( h, EM_REPLACESEL, (LPARAM)TRUE, (WPARAM)s.c_str()) ;
	}

	DestroyMenu( hmenu) ;
	DestroyMenu( hmnPopup) ;
}


