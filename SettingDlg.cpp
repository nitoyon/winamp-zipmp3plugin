
// SettingDlg.cpp
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "SettingDlg.h"
#include "Profile.h"
#include "Controller.h"
#include "resource.h"
#include "Util.h"


HWND SettingDlg::hwndStatic = NULL ;
WNDPROC	wpcStatic ;

/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

SettingDlg::SettingDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

SettingDlg::~SettingDlg() 
{
}



/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// ������
//============================================================================//
// �X�V�F03/04/20(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	if( hwndStatic != 0 && IsWindow( hwndStatic))
	{
		EndDialog( hDlg, FALSE) ;
		SetFocus( hwndStatic) ;
		return TRUE ;
	}

	// �T�u�N���X��
	wpcStatic = (WNDPROC)GetWindowLong( GetDlgItem( hDlg, IDC_URL), GWL_WNDPROC) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_URL) , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_BMP1), GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
	SetWindowLong( GetDlgItem( hDlg, IDC_BMP2), GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

	// �`�F�b�N
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_ZIP, Profile::blnShowOnlyZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_SHOW_ONLY_UNCOMPRESS, Profile::blnShowOnlyUncompressedZip ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CHK_ID3, Profile::blnListID3 ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CHK_COMPI, Profile::blnListCompilation ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_TIMEBAR, Profile::blnShowTimebar ? BST_CHECKED : 0) ;

	// �e�L�X�g�ݒ�
	SetDlgItemText( hDlg, IDC_LIST_NORMAL, Profile::strListNormal.c_str()) ;
	SetDlgItemText( hDlg, IDC_LIST_ID3, Profile::strListID3.c_str()) ;
	SetDlgItemText( hDlg, IDC_LIST_COMPI, Profile::strListCompilation.c_str()) ;
	SetDlgItemText( hDlg, IDC_FONT, Profile::strFont.c_str()) ;

	Validiate() ;
	hwndStatic = hDlg ;
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �_�C�A���O����ǂ݂Ƃ�
	Profile::blnShowOnlyZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_ZIP) ? TRUE : FALSE ;
	Profile::blnShowOnlyUncompressedZip = IsDlgButtonChecked( hDlg, IDC_SHOW_ONLY_UNCOMPRESS) ? TRUE : FALSE ;
	Profile::blnShowTimebar = IsDlgButtonChecked( hDlg, IDC_TIMEBAR) ? TRUE : FALSE ;
	Profile::strListNormal = GetWindowString( GetDlgItem( hDlg, IDC_LIST_NORMAL)) ;
	Profile::strListID3 = GetWindowString( GetDlgItem( hDlg, IDC_LIST_ID3)) ;
	Profile::blnListID3 = IsDlgButtonChecked( hDlg, IDC_CHK_ID3) ? TRUE : FALSE ;
	Profile::strListCompilation = GetWindowString( GetDlgItem( hDlg, IDC_LIST_COMPI)) ;
	Profile::blnListCompilation = IsDlgButtonChecked( hDlg, IDC_CHK_COMPI) ? TRUE : FALSE ;
	Profile::strFont = GetWindowString( GetDlgItem( hDlg, IDC_FONT)) ;
	Profile::Save() ;

	EndDialog( hDlg, TRUE) ;
	hwndStatic = NULL ;
	return TRUE ;
}


/******************************************************************************/
// �L�����Z��
//============================================================================//
// �X�V�F02/12/29(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE) ;
	hwndStatic = NULL ;
	return TRUE ;
}


/******************************************************************************/
// �`�F�b�N����
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnCheck( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Validiate() ;
	return TRUE ;
}


/******************************************************************************/
// �ϐ��{�^��
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnBtnNormal( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\t�t�@�C�����B�f�B���N�g���͏����B") ;
	vec.push_back( "%FILE_PATH%\t�f�B���N�g�����ӂ��񂾃t�@�C�����B") ;

	VarMenu( vec, IDC_LIST_NORMAL, IDC_NORMAL_VARIABLE) ;

	return TRUE ;
}


/******************************************************************************/
// �ϐ��{�^��
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnBtnID3( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\t�t�@�C�����B�f�B���N�g���͏����B") ;
	vec.push_back( "%FILE_PATH%\t�f�B���N�g�����ӂ��񂾃t�@�C�����B") ;
	vec.push_back( "") ;
	vec.push_back( "%TRACK_NUMBER%\t�g���b�N�ԍ��B") ;
	vec.push_back( "%TRACK_NUMBER2%\t�Q���ȏ�̃g���b�N�ԍ��B") ;
	vec.push_back( "%TRACK_NUMBER3%\t�R���ȏ�̃g���b�N�ԍ��B") ;
	vec.push_back( "%TRACK_NAME%\t�Ȗ��B") ;
	vec.push_back( "%ARTIST_NAME%\t�A�[�e�B�X�g���B") ;
	vec.push_back( "%ALBUM_NAME%\t�A���o�����B") ;
	vec.push_back( "%YEAR%\t�N") ;
	vec.push_back( "%COMMENT%\t�R�����g") ;

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
// URL ���N���b�N
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnUrlClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.nitoyon.com/", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// BMP ���N���b�N
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnBmpClicked( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ShellExecute( hDlg, NULL, "http://www.ai.wakwak.com/~cik/cccd.htm", NULL, NULL, SW_SHOWNORMAL) ;
	return TRUE ;
}


/******************************************************************************/
// �J�[�\��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnSetCursor( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HCURSOR hCursor = LoadCursor( NULL, IDC_ARROW) ;
	SetCursor( hCursor);

	return TRUE ;
}


/******************************************************************************/
// �X�^�e�B�b�N�̃J���[
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �����N�̂��߂̃T�u�N���X��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uiMsg)
	{
		case WM_SETCURSOR :
		{
			// ��J�[�\���ǂ݂Ƃ�
			HCURSOR hCurHand	= LoadCursor( Profile::hInstance, MAKEINTRESOURCE( IDC_HANDCUR)) ;
			SetCursor( hCurHand) ;
			return 0 ;
		}
	}

	return CallWindowProc( wpcStatic, hWnd, uiMsg, wParam, lParam) ;
}


/******************************************************************************/
//		���̑�
/******************************************************************************/
// ������
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void SettingDlg::Validiate()
{
	EnableWindow( GetDlgItem( m_hWnd, IDC_SHOW_ONLY_UNCOMPRESS), IsDlgButtonChecked( m_hWnd, IDC_SHOW_ONLY_ZIP) ? TRUE : FALSE) ;

	// IDv3 �^�O������ꍇ
	BOOL b = IsDlgButtonChecked( m_hWnd, IDC_CHK_ID3) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_LIST_ID3), b ? TRUE : FALSE) ;
	EnableWindow( GetDlgItem( m_hWnd, IDC_ID3_VARIABLE), b ? TRUE : FALSE) ;

	// �R���s���[�V����
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
// �|�b�v�A�b�v���j���[�\��
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void SettingDlg::VarMenu( vector<string>& vec, int intCtrlEdt, int intCtrlBtn)
{
	// ���j���[�쐬
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

	// �\��
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


