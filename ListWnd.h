
// ListWnd.h
//============================================================================//
// �X�V�F03/04/11(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __LIST_WND_H__
#define  __LIST_WND_H__

#include "include.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  LIST_LEFT	12
#define  LIST_TOP	20
#define  LIST_RIGHT	19
#define  LIST_XMARGIN	1
#define  LIST_YMARGIN	3
#define  SCROL_HEIGHT	18

#define  NO_ITEM	-1

class MainWnd ;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class ListWnd
{
private:
	HWND		m_hWnd ;
	MainWnd*	pMainWnd ;
	HFONT		hFont ;
	HFONT		hFontSmall ;
	HMENU		hMenuPopup ;

	int		intWidth  ;
	int		intHeight ;
	int		intLineHeight ;
	int		intLineCount ;

	COLORREF	colNormal ;
	COLORREF	colCurrent ;
	COLORREF	colNormalBG ;
	COLORREF	colSelectedBG ;

	int		intSelected ;
	int		intCurrent ;
	int		intScrollPos ;
	int		intScrollRange ;
	vector<string>	vecList ;
	vector<DWORD>	vecTime ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ListWnd( MainWnd*) ;
	~ListWnd() ;

// ���X�g�̐ݒ�
	void Init() ;
	void Destroy() ;
	void SetSkin( const string&) ;
	void ScrollDown() ;
	void ScrollUp() ;
	void PageDown() ;
	void PageUp() ;
	void ScrollTo( int) ;
private:
	void SetScroll( int) ;
	int  CalcScrollbarPos() ;

public:
// �A�C�e���ݒ�
	void ClearList() ;
	void AddList( const string&, DWORD) ;
	void SetSelectedItem( int i) ;
	void SetCurrentItem( int i) ;
	void SetSize() ;

// �擾
	int GetCurrentItem() const{ return intCurrent ;}
	int GetSelectedItem() const{ return intSelected ;}
	const string GetCurrentName() const{ return vecList[ intCurrent];}
	DWORD GetCurrentLength() const{ return vecTime[ intCurrent];}
	int GetScrollBarPos() const ;

// �C�x���g
	LRESULT OnLButtonDown( WPARAM, LPARAM) ;
	LRESULT OnRButtonDown( WPARAM, LPARAM) ;
	LRESULT OnLButtonDblClk( WPARAM, LPARAM) ;

// �`��
	void DrawList( HDC) ;
private:
	void	DrawItem( HDC, int) ;
	void	DrawCompactText( HDC) ;
	void	EraseAll( HDC) ;
	void	InvalidateItem( int) ;

// ���[�e�B���e�B�[
private:
	int	GetItemFromPt( POINT) ;
	COLORREF GetColor( const string&) ;
} ;

#endif