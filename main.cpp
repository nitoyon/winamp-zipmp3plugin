
// zipmp3.cpp
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "main.h"
#include "Controller.h"
#include "MainWnd.h"
#include "SettingDlg.h"
#include "Profile.h"
#include "resource.h"


/******************************************************************************/
//		�O���[�o���ϐ�
/******************************************************************************/

static winampGeneralPurposePlugin plugin =
{
	GPPHDR_VER,
	"zip.mp3 Display-plugin [gen_zipalbum.dll]",
	init,
	config,
	quit,
};


/******************************************************************************/
//		�G�N�X�|�[�g�֐�
/******************************************************************************/
// �w�b�_�[�擾
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DLLEXPORT winampGeneralPurposePlugin* winampGetGeneralPurposePlugin()
{
	return &plugin ;
}


/******************************************************************************/
//		�O���[�o���֐�
/******************************************************************************/
// �ݒ�
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void config()
{
	SettingDlg sd ;
	DialogBoxParam( plugin.hDllInstance, MAKEINTRESOURCE( IDD_SETTING), plugin.hwndParent, SettingDlgProc, (LPARAM)&sd) ;
}


/******************************************************************************/
// ������
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int init()
{
	// �ݒ�ǂ݂Ƃ�
	Profile::hInstance = plugin.hDllInstance ;
	Profile::Load() ;

	// Register our window class
	WNDCLASS wc;
	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc = (WNDPROC)MainWndProc ;				// our window procedure
	wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wc.hInstance = plugin.hDllInstance;	// hInstance of DLL
	wc.lpszClassName = APP_NAME ;			// our window class name

	if (!RegisterClass(&wc)) 
	{
		MessageBox( plugin.hwndParent,"Error registering window class","blah",MB_OK);
		return 1;
	}

	MainWnd* pMainWnd = new MainWnd() ;
	pMainWnd->SetWinampWindow( plugin.hwndParent) ;

	HWND hMainWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		APP_NAME,
		NULL,
		WS_POPUP | WS_CLIPSIBLINGS, 
		0, 0,
		300, 100,
		plugin.hwndParent,
		NULL,
		plugin.hDllInstance,
		(LPVOID)pMainWnd);

	if (!hMainWnd) 
	{
		MessageBox( plugin.hwndParent,"Error creating window","blah",MB_OK);
		return 1;
	}

	Controller* pController = Controller::GetInstance() ;
	pController->SetWindow( pMainWnd) ;

	// show the window
	ShowWindow( hMainWnd, Profile::blnShowOnlyZip ? SW_HIDE : SW_SHOW) ;
	return 0;
}


/******************************************************************************/
// �I����
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void quit()
{
	Profile::Save() ;
	return ;
}
