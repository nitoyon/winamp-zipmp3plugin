
// SkinDlg.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SkinDlg.h"
#include <ShlObj.h>
#include "..\Profile.h"
#include "..\util\uWindow.h"
#include "..\util\uPath.h"
#include "..\resource.h"
#include "..\Controller.h"
#include "..\MainWnd.h"


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

int CALLBACK BrowseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lpData);


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
		ON_COMMAND( IDC_SKIN_BROWSE1	, OnBrowse1)
		ON_COMMAND( IDC_SKIN_BROWSE2	, OnBrowse2)
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


/******************************************************************************/
// ブラウズボタン１
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SkinDlg::OnBrowse1(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	string s = DisplayDirList(GetWindowString(GetDlgItem(hDlg, IDC_SKIN_DIR1)));
	if(s != "")
	{
		SetDlgItemText(hDlg, IDC_SKIN_DIR1, s.c_str());
	}

	return TRUE;
}


/******************************************************************************/
// ブラウズボタン２
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL SkinDlg::OnBrowse2(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	string s = DisplayDirList(GetWindowString(GetDlgItem(hDlg, IDC_SKIN_DIR2)));
	if(s != "")
	{
		SetDlgItemText(hDlg, IDC_SKIN_DIR2, s.c_str());
	}

	return TRUE;
}


/******************************************************************************/
// ブラウズボタン汎用関数
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string SkinDlg::DisplayDirList(const string& strCurrent)
{
	char pszPath[MAX_PATH + 1];
	strcpy(pszPath, GetDirName(strCurrent).c_str()) ;

	BROWSEINFO bi ;
	bi.hwndOwner		= m_hWnd;
	bi.pidlRoot		= NULL ;
	bi.pszDisplayName	= NULL ;
	bi.lpszTitle		= "フォルダを選択して下さい" ;
	bi.ulFlags		= 0x8000 ;//BIF_SHAREABLE ;
	bi.lpfn			= (BFFCALLBACK)BrowseProc ;
	bi.lParam		= (LPARAM)pszPath ;
	bi.iImage		= NULL ;

	LPITEMIDLIST idlist = SHBrowseForFolder( &bi);
	if( idlist)
	{
		char pszPath[MAX_PATH + 1];
		SHGetPathFromIDList( idlist, pszPath) ;
		CoTaskMemFree( idlist) ;

		string s = pszPath;
		return s + '\\';
	}

	return "";
}


/******************************************************************************/
// フォルダの選択コールバック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int CALLBACK BrowseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
			break;
	}
	return 0;
}
