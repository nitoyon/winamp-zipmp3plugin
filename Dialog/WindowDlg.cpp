
// WindowDlg.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "WindowDlg.h"
#include "..\Profile.h"
#include "..\resource.h"
#include "..\util.h"
#include "..\Controller.h"
#include "..\MainWnd.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

WindowDlg::WindowDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

WindowDlg::~WindowDlg() 
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

BEGIN_DLG_MESSAGE_MAP(WindowDlgProc, WindowDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
//	BEGIN_COMMAND_MAP()
//		ON_COMMAND( IDOK		, OnOk)
//		ON_COMMAND( IDCANCEL		, OnCancel)
//	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// �_�C�A���O������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL WindowDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// ���W�I�{�^��
	if(!Profile::blnShowOnlyArchive)
	{
		CheckRadioButton(hDlg, IDC_SHOW_ALWAYS, IDC_ONLY_UNCOMPRESSED, IDC_SHOW_ALWAYS);
	}
	else if(!Profile::blnShowOnlyUncompressed)
	{
		CheckRadioButton(hDlg, IDC_SHOW_ALWAYS, IDC_ONLY_UNCOMPRESSED, IDC_ONLY_ARCHIVE);
	}
	else
	{
		CheckRadioButton(hDlg, IDC_SHOW_ALWAYS, IDC_ONLY_UNCOMPRESSED, IDC_ONLY_UNCOMPRESSED);
	}

	// �`�F�b�N�{�b�N�X
	CheckDlgButton(hDlg, IDC_ATTACH, Profile::blnAttachToWinamp ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_TIMEBAR, Profile::blnUseTimebar ? BST_CHECKED : BST_UNCHECKED);

	// �����x
	hwndTransparency = GetDlgItem(hDlg, IDC_TRANSPARENCY);
	char pszBuf[10];
	for(int i = 0; i <= 10; i++)
	{
		wsprintf(pszBuf, "%d%%", i * 10);
		SendMessage(hwndTransparency, CB_ADDSTRING, 0, (LPARAM)pszBuf);
		if(i * 10 == Profile::intTransparency)
		{
			SendMessage(hwndTransparency, CB_SETCURSEL, i, 0);
		}
	}

	return FALSE;
}


/******************************************************************************/
// �K�p
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void WindowDlg::DoApply()
{
	HWND hDlg = m_hWnd;
	int  intOldTransparency = Profile::intTransparency;

	// ���W�I�{�^��
	if(IsDlgButtonChecked(hDlg, IDC_SHOW_ALWAYS))
	{
		Profile::blnShowOnlyArchive = FALSE;
		Profile::blnShowOnlyUncompressed = FALSE;
	}
	else if(IsDlgButtonChecked(hDlg, IDC_ONLY_ARCHIVE))
	{
		Profile::blnShowOnlyArchive = TRUE;
		Profile::blnShowOnlyUncompressed = FALSE;
	}
	else if(IsDlgButtonChecked(hDlg, IDC_ONLY_UNCOMPRESSED))
	{
		Profile::blnShowOnlyArchive = TRUE;
		Profile::blnShowOnlyUncompressed = TRUE;
	}

	Profile::blnAttachToWinamp = IsDlgButtonChecked(hDlg, IDC_ATTACH);
	Profile::blnUseTimebar = IsDlgButtonChecked(hDlg, IDC_TIMEBAR);

	Profile::intTransparency = atoi(GetWindowString(GetDlgItem(hDlg, IDC_TRANSPARENCY)).c_str());

	// �ύX����Ă���ꍇ
	if(intOldTransparency != Profile::intTransparency)
	{
		Controller* pController = Controller::GetInstance();
		if(pController)
		{
			MainWnd* pMainWnd = pController->GetWindow();
			if(pMainWnd) pMainWnd->SetTransparency();
		}
	}
}