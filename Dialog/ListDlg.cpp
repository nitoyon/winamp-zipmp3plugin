
// ListDlg.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ListDlg.h"
#include "..\resource.h"
#include "..\Profile.h"
#include "..\util.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ListDlg::ListDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ListDlg::~ListDlg() 
{
}



/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �_�C�A���O������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ListDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �`�F�b�N�{�b�N�X
	CheckDlgButton(hDlg, IDC_CHK_ID3, 	Profile::blnListID3 ? BST_CHECKED : 0);
	CheckDlgButton(hDlg, IDC_CHK_COMPI, 	Profile::blnListCompilation ? BST_CHECKED : 0);

	// �G�f�B�b�g�{�b�N�X
	SetDlgItemText(hDlg, IDC_LIST_NORMAL,	Profile::strListNormal.c_str());
	SetDlgItemText(hDlg, IDC_LIST_ID3,	Profile::strListID3.c_str());
	SetDlgItemText(hDlg, IDC_LIST_COMPI,	Profile::strListCompilation.c_str());

	SetEnable();
	return FALSE;
}


/******************************************************************************/
// �`�F�b�N���ꂽ�Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ListDlg::OnChecked(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	SetEnable();
	return TRUE;
}


/******************************************************************************/
// �K�p
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �L�������̐؂�ւ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// ���j���[�\��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ListDlg::OnClickBtn(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// ���j���[�̕�����ݒ�
	vector<string> vec ;
	vec.push_back( "%FILE_NAME%\t�t�@�C�����B�f�B���N�g���͏����B") ;
	vec.push_back( "%FILE_PATH%\t�f�B���N�g�����ӂ��񂾃t�@�C�����B") ;

	if(LOWORD(wParam) != IDC_NORMAL_VARIABLE)
	{
		vec.push_back( "") ;
		vec.push_back( "%TRACK_NUMBER%\t�g���b�N�ԍ��B") ;
		vec.push_back( "%TRACK_NUMBER2%\t�Q���ȏ�̃g���b�N�ԍ��B") ;
		vec.push_back( "%TRACK_NUMBER3%\t�R���ȏ�̃g���b�N�ԍ��B") ;
		vec.push_back( "%TRACK_NAME%\t�Ȗ��B") ;
		vec.push_back( "%ARTIST_NAME%\t�A�[�e�B�X�g���B") ;
		vec.push_back( "%ALBUM_NAME%\t�A���o�����B") ;
		vec.push_back( "%YEAR%\t�N") ;
		vec.push_back( "%COMMENT%\t�R�����g") ;
	}

	// �\��
	DisplayMenu(vec, LOWORD(wParam));

	return TRUE ;
}


/******************************************************************************/
// ���j���[�\��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void ListDlg::DisplayMenu(const vector<string>& vec, int intCtrlBtn)
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
		HWND h = GetDlgItem( m_hWnd, intCtrlBtn - 1) ;
		SendMessage( h, EM_REPLACESEL, (LPARAM)TRUE, (WPARAM)s.c_str()) ;
	}

	DestroyMenu( hmenu) ;
	DestroyMenu( hmnPopup) ;
}


