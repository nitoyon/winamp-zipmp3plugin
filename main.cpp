
// zipmp3.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "main.h"
#include "Controller.h"
#include "MainWnd.h"
#include "WinampHook.h"
#include "Dialog\SettingDlg.h"
#include "Profile.h"
#include "resource.h"
#include <time.h>

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x80000
#endif


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
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void config()
{
	SettingDlg sd(plugin.hDllInstance);
	DialogBoxParam(plugin.hDllInstance, MAKEINTRESOURCE(IDD_SETTING), GetForegroundWindow(), SettingDlgProc, (LPARAM)&sd) ;
}


/******************************************************************************/
// ������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int init()
{
	// ����������
	srand(time(NULL));

	// �ݒ�ǂ݂Ƃ�
	Profile::hInstance = plugin.hDllInstance ;
	Profile::Load() ;

	// Register our window class
	INITCOMMONCONTROLSEX ic = { sizeof( INITCOMMONCONTROLSEX), ICC_HOTKEY_CLASS } ;
	InitCommonControlsEx( &ic) ;
	WNDCLASS wc;
	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc = (WNDPROC)MainWndProc ;				// our window procedure
	wc.hInstance = plugin.hDllInstance;	// hInstance of DLL
	wc.hCursor = LoadCursor( NULL, IDC_ARROW) ;
	wc.style = CS_DBLCLKS ;
	wc.lpszClassName = APP_NAME ;			// our window class name

	if (!RegisterClass(&wc)) 
	{
		MessageBox( plugin.hwndParent,"Error registering window class","blah",MB_OK);
		return 1;
	}
 
	MainWnd* pMainWnd = new MainWnd() ;
	pMainWnd->SetWinampWindow( plugin.hwndParent) ;

	Controller* pController = Controller::GetInstance() ;
	pController->SetWindow( pMainWnd) ;

	HWND hMainWnd = CreateWindowEx(
		WS_EX_LAYERED,//WS_EX_TOOLWINDOW
		APP_NAME,
		NULL,
		WS_CLIPSIBLINGS, //WS_POPUP, 
		0, 0,
		300, 100,
		plugin.hwndParent,
		NULL,
		plugin.hDllInstance,
		(LPVOID)pMainWnd);

	if (!hMainWnd) 
	{
		MessageBox( plugin.hwndParent,"Error creating window","�G���[",MB_OK);
		return 1;
	}

	// �t�b�N�J�n
	WinampHook* pwh = new WinampHook( pMainWnd) ;
	pwh->Init( plugin.hwndParent) ;

	return 0;
}


/******************************************************************************/
// �I����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void quit()
{
	Profile::Save() ;
	delete Controller::GetInstance() ;
	return ;
}


/******************************************************************************/
//		DllMain
/******************************************************************************/
// DllMain
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}