
// SettingDlg.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
class ChildDlg;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class SettingDlg
{
private:
	HWND		m_hWnd ;
	HWND		hwndTree;
	HIMAGELIST	hImgList;
	HINSTANCE	hInstance;

	vector<ChildDlg*>	vecChildDlg;
	int	intCurDlg;

public:
// コンストラクタおよびデストラクタ
	SettingDlg(HINSTANCE h);
	~SettingDlg() ;

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnTreeNotify	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;

// その他
	void DisplayDlg(int);
} ;

#endif