
// SkinDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SkinDlg.h"
#include "..\Profile.h"
#include "..\util.h"
#include "..\resource.h"
#include "..\Controller.h"
#include "..\MainWnd.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

SkinDlg::SkinDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

SkinDlg::~SkinDlg() 
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

BEGIN_DLG_MESSAGE_MAP(SkinDlgProc, SkinDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	ON_MESSAGE( WM_CTLCOLORSTATIC	, OnCtlColorStatic)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDC_SKIN_PLUGIN1	, OnRadioChanged)
		ON_COMMAND( IDC_SKIN_OTHER1	, OnRadioChanged)
		ON_COMMAND( IDC_SKIN_WINAMP2	, OnRadioChanged)
		ON_COMMAND( IDC_SKIN_PLUGIN2	, OnRadioChanged)
		ON_COMMAND( IDC_SKIN_OTHER2	, OnRadioChanged)
	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ダイアログ初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SkinDlg::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	CheckRadioButton(hDlg, IDC_SKIN_PLUGIN1, IDC_SKIN_OTHER1, IDC_SKIN_PLUGIN1 + Profile::intSkin1);
	CheckRadioButton(hDlg, IDC_SKIN_WINAMP2, IDC_SKIN_OTHER2, IDC_SKIN_WINAMP2 + Profile::intSkin2);
	SetDlgItemText(hDlg, IDC_SKIN_DIR1, Profile::strSkinDir1.c_str());
	SetDlgItemText(hDlg, IDC_SKIN_DIR2, Profile::strSkinDir2.c_str());

	SetEnable();
	return FALSE;
}


/******************************************************************************/
// ラジオの状態が変化したとき
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SkinDlg::OnRadioChanged(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	SetEnable();
	return TRUE;
}


/******************************************************************************/
// enable 状態を設定する
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SkinDlg::SetEnable()
{
	BOOL b1 = IsDlgButtonChecked(m_hWnd, IDC_SKIN_OTHER1);
	BOOL b2 = IsDlgButtonChecked(m_hWnd, IDC_SKIN_OTHER2);

	EnableWindow(GetDlgItem(m_hWnd, IDC_SKIN_DIR1), b1);
	EnableWindow(GetDlgItem(m_hWnd, IDC_SKIN_BROWSE1), b1);
	EnableWindow(GetDlgItem(m_hWnd, IDC_SKIN_DIR2), b2);
	EnableWindow(GetDlgItem(m_hWnd, IDC_SKIN_BROWSE2), b2);
}


/******************************************************************************/
// 適用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void SkinDlg::DoApply()
{
	int intOld1 = Profile::intSkin1;
	int intOld2 = Profile::intSkin2;
	string str1 = Profile::strSkinDir1;
	string str2 = Profile::strSkinDir2;

	if(IsDlgButtonChecked(m_hWnd, IDC_SKIN_PLUGIN1))	Profile::intSkin1 = 0;
	else if(IsDlgButtonChecked(m_hWnd, IDC_SKIN_OTHER1))	Profile::intSkin1 = 1;

	if(IsDlgButtonChecked(m_hWnd, IDC_SKIN_PLUGIN2))	Profile::intSkin2 = 1;
	else if(IsDlgButtonChecked(m_hWnd, IDC_SKIN_WINAMP2))	Profile::intSkin2 = 0;
	else if(IsDlgButtonChecked(m_hWnd, IDC_SKIN_OTHER2))	Profile::intSkin2 = 2;

	Profile::strSkinDir1 = GetWindowString(GetDlgItem(m_hWnd, IDC_SKIN_DIR1));
	Profile::strSkinDir2 = GetWindowString(GetDlgItem(m_hWnd, IDC_SKIN_DIR2));

	// 変更されている場合
	if(intOld1 != Profile::intSkin1 || intOld2 != Profile::intSkin2 
	|| str1 != Profile::strSkinDir1 || str2 != Profile::strSkinDir2)
	{
		Controller* pController = Controller::GetInstance();
		if(pController)
		{
			MainWnd* pMainWnd = pController->GetWindow();
			if(pMainWnd) pMainWnd->UpdateSkin();
		}
	}
}