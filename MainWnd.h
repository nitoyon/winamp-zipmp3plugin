
// MainWnd.h
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __MAIN_WND_H__
#define  __MAIN_WND_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		��`
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
//		�N���X��`
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
// �R���X�g���N�^����уf�X�g���N�^
	MainWnd() ;
	~MainWnd() ;

// ���b�Z�[�W�n���h��
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
// �ݒ�
	void SetBlockSize( int, int) ;
	void SetWinampWindow( HWND h){ hwndWinamp = h ;}
	void SetTime( int, int) ;
	void SetCurSong( int, DWORD) ;
	void ClearList() ;
	void AddList( const string&, DWORD = 0) ;
	void ToggleCompact() ;

// �擾
	HWND GetWinampWindow() const{ return hwndWinamp ;}
	HWND GetHwnd() const{ return m_hWnd ;}
	ListWnd* GetListWnd() const{ return pListWnd ;}
	int GetCurSong() const ;
	BOOL IsSnapping() ;
	POINT GetOffsetSnap() const{ return ptOffsetSnap ;} ;
	HBITMAP GetFontBmp() const{ return hbmpText ;} ;

// ���[�e�B���e�B
	Item GetItem( POINT) ;
	void InvalidateItem( Item) ;


// �`��
	void UpdateSkin( BOOL = TRUE) ;
private:
	void UnloadSkin();
	void LoadPluginSkin();
	void LoadPathSkin(const string&);
	void DrawSkin( HDC) ;
	void DrawSkinNormal( HDC) ;
	void DrawSkinCompact( HDC) ;
	void DrawTime( HDC) ;

// �ړ�
	POINT CalcSnappedPos( POINT) ;
} ;

#endif