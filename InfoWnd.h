
// InfoWnd.h
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __INFO_WND_H__
#define  __INFO_WND_H__

#include "include.h"
#include "MsgMap.h"
class ArchiveFile;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class InfoWnd
{
private:
	HWND		m_hWnd;
	HWND		hwndList;
	static BOOL	blnInit;

	ArchiveFile*	pArchiveFile;

public:
// コンストラクタおよびデストラクタ
	InfoWnd() ;
	~InfoWnd() ;
	HWND Create();

// アーカイブファイル関係
	void SetArchiveFile(ArchiveFile*);

// メッセージハンドラ
private:
	DECLARE_MESSAGE_MAP( InfoWndProc)

	LRESULT OnCreate	( HWND, WPARAM, LPARAM) ;
	LRESULT OnSize		( HWND, WPARAM, LPARAM) ;
	LRESULT OnDestroy	( HWND, WPARAM, LPARAM) ;
	LRESULT OnKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnSysKeyDown	( HWND, WPARAM, LPARAM) ;
} ;

#endif