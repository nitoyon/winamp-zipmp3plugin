
// ListWnd.h
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __LIST_WND_H__
#define  __LIST_WND_H__

#include "include.h"


/******************************************************************************/
//		定義
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
//		クラス定義
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
// コンストラクタおよびデストラクタ
	ListWnd( MainWnd*) ;
	~ListWnd() ;

// リストの設定
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
// アイテム設定
	void ClearList() ;
	void AddList( const string&, DWORD) ;
	void SetSelectedItem( int i) ;
	void SetCurrentItem( int i) ;
	void SetSize() ;

// 取得
	int GetCurrentItem() const{ return intCurrent ;}
	int GetSelectedItem() const{ return intSelected ;}
	const string GetCurrentName() const{ return vecList[ intCurrent];}
	DWORD GetCurrentLength() const{ return vecTime[ intCurrent];}
	int GetScrollBarPos() const ;

// イベント
	LRESULT OnLButtonDown( WPARAM, LPARAM) ;
	LRESULT OnRButtonDown( WPARAM, LPARAM) ;
	LRESULT OnLButtonDblClk( WPARAM, LPARAM) ;

// 描画
	void DrawList( HDC) ;
private:
	void	DrawItem( HDC, int) ;
	void	DrawCompactText( HDC) ;
	void	EraseAll( HDC) ;
	void	InvalidateItem( int) ;

// ユーティリティー
private:
	int	GetItemFromPt( POINT) ;
	COLORREF GetColor( const string&) ;
} ;

#endif