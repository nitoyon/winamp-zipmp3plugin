
// zipmp3.cpp
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "main.h"
#include "Controller.h"
#include "MainWnd.h"
#include "WinampHook.h"
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
	DialogBoxParam( plugin.hDllInstance, MAKEINTRESOURCE( IDD_SETTING), GetForegroundWindow(), SettingDlgProc, (LPARAM)&sd) ;
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

	HWND hMainWnd = CreateWindowEx(
		0,//WS_EX_TOOLWINDOW
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
		MessageBox( plugin.hwndParent,"Error creating window","blah",MB_OK);
		return 1;
	}

	Controller* pController = Controller::GetInstance() ;
	pController->SetWindow( pMainWnd) ;

	// �t�b�N�J�n
	WinampHook* pwh = new WinampHook( pMainWnd) ;
	pwh->Init( plugin.hwndParent) ;

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
	delete Controller::GetInstance() ;
	return ;
}
