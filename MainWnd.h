
// MainWnd.h
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MAIN_WND_H__
#define  __MAIN_WND_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  XMIN_BLOCK		5
#define  YMIN_BLOCK		2
#define  X_BLOCK_SIZE		25
#define  Y_BLOCK_SIZE		29
#define  X_BLOCK_CONST		150
#define  Y_BLOCK_CONST		58
#define  Y_COMPACT_HEIGHT	14
#define  TIMEBAR_WIDTH		94

#define  HOTKEY_SHOW	0

class ListWnd ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class MainWnd
{
public:
	enum Item
	{
		CLOSE, 		COMPACT, 
		PAGEDOWN, 	PAGEUP, 
		PREV, 		NEXT, 
		PLAY, 		STOP, 
		PAUSE, 		EJECT, 
		MOVE, 		RESIZE, 
		TIME,		LIST, 
		SCROLLBAR,	TIMEBAR, 
		COM_LIST, 
		LAST
	} ;

private:
	HWND		m_hWnd ;
	HWND		hwndWinamp ;
	HWND		hwndWinampEQ ;
	HWND		hwndWinampPE ;
	HWND		hwndWinampMB ;

	HBITMAP		hbmpPlaylist ;
	HBITMAP		hbmpText ;
	HBITMAP		hbmpTimebar ;
	string		strSkinName ;

	ListWnd*	pListWnd ;

	int		intWidth ;
	int		intHeight ;

	DWORD		dwCurSongLength ;
	int		intMin ;
	int		intSec ;

	COLORREF	colNormal ;
	COLORREF	colCurrent ;
	COLORREF	colNormalBG ;
	COLORREF	colSelectedBG ;

	BOOL		blnFocus ;
	BOOL		blnResize ;
	BOOL		blnMove ;
	POINT		ptOffsetMove ;
	BOOL		blnClose ;
	BOOL		blnScroll ;
	BOOL		blnSnapping ;
	POINT		ptOffsetSnap ;
	BOOL		blnTimeBarDrag ;
	int		intTimeBarDrag ;

	int intLeftPos[ Item::LAST] ;
	int intRightPos[ Item::LAST] ;
	int intTopPos[ Item::LAST] ;
	int intBottomPos[ Item::LAST] ;

public:
// コンストラクタおよびデストラクタ
	MainWnd() ;
	~MainWnd() ;

// メッセージハンドラ
private:
	DECLARE_MESSAGE_MAP( MainWndProc)

	LRESULT OnCreate	( HWND, WPARAM, LPARAM) ;
	LRESULT OnTimer		( HWND, WPARAM, LPARAM) ;
	LRESULT OnHotKey	( HWND, WPARAM, LPARAM) ;
	LRESULT OnSize		( HWND, WPARAM, LPARAM) ;
	LRESULT OnFocus		( HWND, WPARAM, LPARAM) ;
	LRESULT OnKillFocus	( HWND, WPARAM, LPARAM) ;
	LRESULT OnPaint		( HWND, WPARAM, LPARAM) ;
	LRESULT OnDestroy	( HWND, WPARAM, LPARAM) ;
	LRESULT OnLButtonDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnRButtonDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnLButtonUp	( HWND, WPARAM, LPARAM) ;
	LRESULT OnMouseMove	( HWND, WPARAM, LPARAM) ;
	LRESULT OnLButtonDblClk	( HWND, WPARAM, LPARAM) ;
	LRESULT OnMouseWheel	( HWND, WPARAM, LPARAM) ;
	LRESULT OnKeyDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnSysKeyDown	( HWND, WPARAM, LPARAM) ;

public:
// 設定
	void SetBlockSize( int, int) ;
	void SetWinampWindow( HWND h){ hwndWinamp = h ;}
	void SetTime( int, int) ;
	void SetCurSong( int, DWORD) ;
	void ClearList() ;
	void AddList( const string&, DWORD = 0) ;
	void ToggleCompact() ;

// 取得
	HWND GetWinampWindow() const{ return hwndWinamp ;}
	HWND GetHwnd() const{ return m_hWnd ;}
	ListWnd* GetListWnd() const{ return pListWnd ;}
	int GetCurSong() const ;
	BOOL IsSnapping() ;
	POINT GetOffsetSnap() const{ return ptOffsetSnap ;} ;
	HBITMAP GetFontBmp() const{ return hbmpText ;} ;

// ユーティリティ
	Item GetItem( POINT) ;
	void InvalidateItem( Item) ;


// 描画
	void UpdateSkin( BOOL = TRUE) ;
private:
	void UnloadSkin();
	void LoadPluginSkin();
	void LoadPathSkin(const string&);
	void DrawSkin( HDC) ;
	void DrawSkinNormal( HDC) ;
	void DrawSkinCompact( HDC) ;
	void DrawTime( HDC) ;

// 移動
	POINT CalcSnappedPos( POINT) ;
} ;

#endif