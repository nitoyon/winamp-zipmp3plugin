
// setting.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "SettingDlg.h"
#include "resource.h"
#include "..\Profile.h"
#include "..\include.h"
#include "commctrl.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int intCmdShow)
{
	SettingDlg sd;
	Profile::Load();
	
	InitCommonControls();

	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SETTING), NULL, SettingDlgProc, (LPARAM)&sd);
	Profile::Save();
/*
	HWND hDialog = 
	CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_SETTING), NULL, SettingDlgProc, (LPARAM)&sd);
	if(!hDialog)
	{
		return 0;
	}
	ShowWindow(hDialog, SW_SHOW);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
//		if(!sd.IsDialogMessage(&msg) && !IsDialogMessage(hDialog, &msg))
		if(!IsDialogMessage(hDialog, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
*/
	return 0;
}