
// MainWnd.h
//============================================================================//
// �X�V�F02/12/30(��)
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

#define  XMIN_BLOCK	5
#define  YMIN_BLOCK	2
#define  X_BLOCK_SIZE	25
#define  Y_BLOCK_SIZE	29
#define  X_BLOCK_CONST	150
#define  Y_BLOCK_CONST	58

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
		SCROLLBAR,	LAST
	} ;

private:
	HWND		m_hWnd ;
	HWND		hwndWinamp ;
	HWND		hwndWinampEQ ;
	HWND		hwndWinampPE ;
	HWND		hwndWinampMB ;

	HBITMAP		hbmpPlaylist ;
	HBITMAP		hbmpText ;
	string		strSkinName ;
	string		strSkinPath ;

	ListWnd*	pListWnd ;

	int		intWidth ;
	int		intHeight ;

	int		intMin ;
	int		intSec ;

	COLORREF	colNormal ;
	COLORREF	colCurrent ;
	COLORREF	colNormalBG ;
	COLORREF	colSelectedBG ;

	BOOL		blnResize ;
	BOOL		blnMove ;
	POINT		ptOffsetMove ;
	BOOL		blnClose ;
	BOOL		blnScroll ;
	BOOL		blnSnapping ;
	POINT		ptOffsetSnap ;

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
	void SetCurSong( int) ;
	void ClearList() ;
	void AddList( const string&) ;

// �擾
	HWND GetWinampWindow() const{ return hwndWinamp ;}
	HWND GetHwnd() const{ return m_hWnd ;}
	int GetCurSong() const ;
	BOOL IsSnapping() ;
	POINT GetOffsetSnap() const{ return ptOffsetSnap ;} ;

// ���[�e�B���e�B
	Item GetItem( POINT) ;
	void InvalidateItem( Item) ;

private:
// �`��
	void UpdateSkin( BOOL = FALSE) ;
	void DrawSkin( HDC) ;
	void DrawTime( HDC) ;

// �ړ�
	POINT CalcSnappedPos( POINT) ;
} ;

#endif