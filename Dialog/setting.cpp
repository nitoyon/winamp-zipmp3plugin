
// setting.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "SettingDlg.h"
#include "resource.h"
#include "..\Profile.h"
#include "..\include.h"
#include "commctrl.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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