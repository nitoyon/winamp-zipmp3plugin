
// MainWnd.h
//============================================================================//
// �X�V�F02/12/26(��)
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

#define  LIST_LEFT	12
#define  LIST_TOP	20
#define  LIST_RIGHT	19
#define  LIST_MARGIN	3

#define  XMIN_BLOCK	5
#define  YMIN_BLOCK	2
#define  X_BLOCK_SIZE	25
#define  Y_BLOCK_SIZE	29
#define  X_BLOCK_CONST	150
#define  Y_BLOCK_CONST	58


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
		LAST
	} ;

private:
	HWND		m_hWnd ;
	HWND		hwndWinamp ;
	HWND		hwndList ;
	HFONT		hFont ;
	HBITMAP		hbmpPlaylist ;
	HBITMAP		hbmpText ;
	string		strSkinName ;
	string		strSkinPath ;

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

	int		intListSelected ;
	int		intCurrent ;
	vector<string>	vecList ;

	int intLeftPos[ Item::LAST] ;
	int intRightPos[ Item::LAST] ;
	int intTopPos[ Item::LAST] ;
	int intBottomPos[ Item::LAST] ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	MainWnd() ;
	~MainWnd() ;

// �ݒ�
	void ShowWindow( BOOL) const ;
	void SetWinampWindow( HWND h){ hwndWinamp = h ;}
	void SetTime( int, int) ;
	void SetCurSong( int) ;
	void ClearList() ;
	void AddList( const string&) ;

// �擾
	HWND GetWinampWindow() const{ return hwndWinamp ;}

// ���b�Z�[�W�n���h��
private:
	DECLARE_MESSAGE_MAP( MainWndProc)

	LRESULT OnCreate	( HWND, WPARAM, LPARAM) ;
	LRESULT OnTimer		( HWND, WPARAM, LPARAM) ;
	LRESULT OnSize		( HWND, WPARAM, LPARAM) ;
	LRESULT OnFocus		( HWND, WPARAM, LPARAM) ;
	LRESULT OnPaint		( HWND, WPARAM, LPARAM) ;
	LRESULT OnDestroy	( HWND, WPARAM, LPARAM) ;
	LRESULT OnLButtonDown	( HWND, WPARAM, LPARAM) ;
	LRESULT OnLButtonUp	( HWND, WPARAM, LPARAM) ;
	LRESULT OnMouseMove	( HWND, WPARAM, LPARAM) ;

// ���[�e�B���e�B
	Item GetItem( POINT) ;
	void InvalidateItem( Item) ;

// �`��
	void UpdateSkin() ;
	void DrawSkin( HDC) ;
	void DrawList( HDC) ;
	void DrawTime( HDC) ;
	COLORREF GetColor( const string&) ;

} ;

#endif