
// SettingDlg.h
//============================================================================//
// 更新：02/12/29(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class SettingDlg
{
private:
	HWND	m_hWnd ;
	HWND	hwndHotKey ;
	static HWND hwndStatic ;

public:
// コンストラクタおよびデストラクタ
	SettingDlg() ;
	~SettingDlg() ;

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnCheck		( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnNormal	( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnID3		( HWND, WPARAM, LPARAM) ;

// その他
	void Validiate() ;
	void VarMenu( vector<string>&, int, int) ;
} ;

#endif