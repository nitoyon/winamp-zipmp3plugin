
// FontDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "FontDlg.h"
#include "..\Profile.h"
#include "..\resource.h"
#include "..\Controller.h"
#include "..\MainWnd.h"
#include "..\ListWnd.h"
#include "..\util\uWindow.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

FontDlg::FontDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

FontDlg::~FontDlg() 
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

BEGIN_DLG_MESSAGE_MAP(FontDlgProc, FontDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLOREDIT	, OnCtlColorStatic)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
//	BEGIN_COMMAND_MAP()
//		ON_COMMAND( IDOK		, OnOk)
//		ON_COMMAND( IDCANCEL		, OnCancel)
//	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL FontDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// フォント列挙
	HDC hdc = GetWindowDC(hDlg) ;
	LOGFONT lf ;
	memset( &lf, 0, sizeof( LOGFONT)) ;
	lf.lfCharSet = SHIFTJIS_CHARSET ;//DEFAULT_CHARSET ;
	EnumFontFamiliesEx( hdc, &lf, (FONTENUMPROC)FontFamEnumProc, (LPARAM)this, 0) ;

	// フォント設定
	HWND hwndCombo1 = GetDlgItem( hDlg, IDC_LIST_FONT);
	HWND hwndCombo2 = GetDlgItem( hDlg, IDC_COLLAPSE_FONT);
	int intIndex;
	SetDlgItemText(hDlg, IDC_LIST_FONT, Profile::strListFont.c_str());
	SetDlgItemText(hDlg, IDC_COLLAPSE_FONT, Profile::strListFont.c_str());
	intIndex = SendMessage(hwndCombo1, CB_FINDSTRING, 0, (LPARAM)Profile::strListFont.c_str());
	if(intIndex != CB_ERR) SendMessage(hwndCombo1, CB_SETCURSEL, intIndex, 0);
	intIndex = SendMessage(hwndCombo2, CB_FINDSTRING, 0, (LPARAM)Profile::strCollapseFont.c_str());
	if(intIndex != CB_ERR) SendMessage(hwndCombo2, CB_SETCURSEL, intIndex, 0);

	// フォントサイズ追加
	HWND hwndListFontSize = GetDlgItem(hDlg, IDC_LIST_FONTSIZE);
	HWND hwndCollapseFontSize = GetDlgItem(hDlg, IDC_COLLAPSE_FONTSIZE);

	SendMessage(hwndListFontSize, CB_ADDSTRING, 0, (LPARAM)"Winamp のフォントサイズ");
	SendMessage(hwndCollapseFontSize, CB_ADDSTRING, 0, (LPARAM)"推奨のフォントサイズ");
	SendMessage(hwndListFontSize, CB_SETCURSEL, 0, 0);
	SendMessage(hwndCollapseFontSize, CB_SETCURSEL, 0, 0);

	for(int i = 6; i < 25; i++)
	{
		char pszBuf[3];
		wsprintf(pszBuf, "%d", i);
		SendMessage(hwndListFontSize, CB_ADDSTRING, 0, (LPARAM)pszBuf);
		SendMessage(hwndCollapseFontSize, CB_ADDSTRING, 0, (LPARAM)pszBuf);

		if(i == Profile::intListFontSize)
		{
			SendMessage(hwndListFontSize, CB_SETCURSEL, i - 5, 0);
		}
		if(i == Profile::intCollapseFontSize)
		{
			SendMessage(hwndCollapseFontSize, CB_SETCURSEL, i - 5, 0);
		}
	}

	// スキン フォント
	CheckDlgButton(hDlg, IDC_USE_SKIN_FONT, Profile::blnUseSkinFont ? BST_CHECKED : BST_UNCHECKED);

	return FALSE;
}


/******************************************************************************/
// フォント列挙プロシージャ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int CALLBACK FontFamEnumProc(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, int FontType, LPARAM lParam)
{
	FontDlg* p = (FontDlg*)lParam ;
	HWND hwndCombo1 = GetDlgItem( p->m_hWnd, IDC_LIST_FONT);
	HWND hwndCombo2 = GetDlgItem( p->m_hWnd, IDC_COLLAPSE_FONT);

	if(SendMessage(hwndCombo1, CB_FINDSTRING, 0, (LPARAM)lpelf->elfLogFont.lfFaceName) == CB_ERR)
	{
		SendMessage(hwndCombo1, CB_ADDSTRING, 0, (LPARAM)lpelf->elfLogFont.lfFaceName);
		SendMessage(hwndCombo2, CB_ADDSTRING, 0, (LPARAM)lpelf->elfLogFont.lfFaceName);
	}

	return 1;
}


/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void FontDlg::DoApply()
{
	HWND hDlg = m_hWnd;
	string	strLF	= Profile::strListFont;
	string	strCF	= Profile::strCollapseFont;
	int	intLFS	= Profile::intListFontSize;
	int	intCFS	= Profile::intCollapseFontSize;
	BOOL	blnUSF	= Profile::blnUseSkinFont;

	Profile::strListFont		= GetWindowString(GetDlgItem(hDlg, IDC_LIST_FONT));
	Profile::intListFontSize	= atoi(GetWindowString(GetDlgItem(hDlg, IDC_LIST_FONTSIZE)).c_str());
	Profile::strCollapseFont	= GetWindowString(GetDlgItem(hDlg, IDC_COLLAPSE_FONT));
	Profile::intCollapseFontSize	= atoi(GetWindowString(GetDlgItem(hDlg, IDC_COLLAPSE_FONTSIZE)).c_str());
	Profile::blnUseSkinFont		= IsDlgButtonChecked(hDlg, IDC_USE_SKIN_FONT);

	// 変更されている場合
	if(strLF != Profile::strListFont || strCF != Profile::strCollapseFont
	|| intLFS != Profile::intListFontSize || intLFS != Profile::intCollapseFontSize || blnUSF != Profile::blnUseSkinFont)
	{
		Controller* pController = Controller::GetInstance();
		if(pController)
		{
			MainWnd* pMainWnd = pController->GetWindow();
			if(pMainWnd)
			{
				ListWnd* pListWnd = pMainWnd->GetListWnd();
				if(pListWnd) pListWnd->SetFont();
			}
		}
	}
}