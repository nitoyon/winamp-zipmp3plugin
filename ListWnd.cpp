
// ListWnd.cpp
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ListWnd.h"
#include "MainWnd.h"
#include "Controller.h"
#include "Profile.h"
#include "resource.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

ListWnd::ListWnd( MainWnd* p)
: m_hWnd( p->GetHwnd()), pMainWnd( p), hFont( NULL), intLineHeight( 10)
, intSelected( 0), intCurrent( 0), intScrollPos( 0)
{
}


/******************************************************************************/
// 
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

ListWnd::~ListWnd() 
{
	if( hFont)	DeleteObject( hFont) ;
	if( hFontSmall)	DeleteObject( hFontSmall) ;
	DestroyMenu( hMenuPopup) ;
}


/******************************************************************************/
//		リストの設定
/******************************************************************************/
// 初期化
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::Init()
{
	// フォント作成
	SetFont() ;
	hFontSmall = CreateFont( 9, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH | FF_MODERN, "MS Pゴシック") ;

	hMenuPopup = LoadMenu( Profile::hInstance, MAKEINTRESOURCE( IDR_ITEMPOPUP)) ;
	hMenuPopup = GetSubMenu( hMenuPopup, 0) ;
}


/******************************************************************************/
// スキンの更新
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::SetSkin( const string& strIni)
{
	char pszBuf[ MAX_PATH + 1] ;
	GetPrivateProfileString( "Text", "Normal", "#00FF00", pszBuf, MAX_PATH, strIni.c_str()) ;
	colNormal = GetColor( pszBuf) ;
	GetPrivateProfileString( "Text", "Current", "#FFFFFF", pszBuf, MAX_PATH, strIni.c_str()) ;
	colCurrent = GetColor( pszBuf) ;
	GetPrivateProfileString( "Text", "NormalBG", "#000000", pszBuf, MAX_PATH, strIni.c_str()) ;
	colNormalBG = GetColor( pszBuf) ;
	GetPrivateProfileString( "Text", "SelectedBG", "#0000FF", pszBuf, MAX_PATH, strIni.c_str()) ;
	colSelectedBG = GetColor( pszBuf) ;
}


/******************************************************************************/
// フォント設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::SetFont()
{
	if( hFont)
	{
		DeleteObject( hFont) ;
	}

	int intFontSize = GetPrivateProfileInt( "Winamp", "pe_fontsize", 10, Profile::strWinampIniPath.c_str()) ;

	hFont = CreateFont( intFontSize, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, // DEFAULT_CHARSET
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH | FF_MODERN, Profile::strFont.c_str()) ;
	if(!hFont)
	{
		hFont = CreateFont( intFontSize, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, 
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			VARIABLE_PITCH | FF_MODERN, "ＭＳ Ｐゴシック") ;
	}

	InvalidateRect( m_hWnd, NULL, FALSE) ;
}


/******************************************************************************/
// 選択を１つ下に移動
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::ScrollDown() 
{
	if( intSelected < 0)
	{
		intSelected = intScrollPos ;
		pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
		return ;
	}
	if( intSelected >= vecList.size() - 1)
	{
		return ;
	}

	// ページ内に選択項目がない場合
	if( intSelected == NO_ITEM || intScrollPos > intSelected || intScrollPos + intLineCount - 1 < intSelected)
	{
		intSelected = intScrollPos ;
		pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
		return ;
	}

	intSelected++ ;
	pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
	if( intScrollPos + intLineCount <= intSelected)
	{
		intScrollPos = intSelected - intLineCount + 1 ;
		pMainWnd->InvalidateItem( MainWnd::Item::SCROLLBAR) ;
	}
}


/******************************************************************************/
// 選択を１つ上に移動
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::ScrollUp()
{
	if( intSelected <= 0)
	{
		intSelected = intScrollPos ;
		pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
		return ;
	}

	// ページ内に選択項目がない場合
	if( intScrollPos > intSelected || intScrollPos + intLineCount - 1 < intSelected)
	{
		intSelected = intScrollPos ;
		pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
		return ;
	}

	intSelected-- ;
	pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
	if( intScrollPos > intSelected)
	{
		intScrollPos = intSelected ;
		pMainWnd->InvalidateItem( MainWnd::Item::SCROLLBAR) ;
	}
}


/******************************************************************************/
// ページダウン
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::PageDown() 
{
	if( intScrollRange < 0)
	{
		return ;
	}

	intScrollPos += intLineCount / 2 ;
	intScrollPos = ( intScrollPos < intScrollRange ? intScrollPos : intScrollRange) ;

	pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
	pMainWnd->InvalidateItem( MainWnd::Item::SCROLLBAR) ;
}


/******************************************************************************/
// ページアップ
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::PageUp() 
{
	if( intScrollRange < 0)
	{
		return ;
	}

	intScrollPos -= intLineCount / 2 ;
	intScrollPos = ( intScrollPos > 0 ? intScrollPos : 0) ;

	pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
	pMainWnd->InvalidateItem( MainWnd::Item::SCROLLBAR) ;
}


/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::ScrollTo( int intPos)
{
	if( intScrollRange <= 0)
	{
		return ;
	}

	intPos = ( intPos > intHeight - SCROL_HEIGHT ? intPos = intHeight - SCROL_HEIGHT : intPos) ;
	intPos = ( intPos >= 0 ? intPos : 0) ;
	intScrollPos = intPos * intScrollRange / ( intHeight - SCROL_HEIGHT) ;

	pMainWnd->InvalidateItem( MainWnd::Item::LIST) ;
	pMainWnd->InvalidateItem( MainWnd::Item::SCROLLBAR) ;
}


/******************************************************************************/
// スクロールバーの場所を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

int ListWnd::GetScrollBarPos() const
{
	return ( intScrollRange > 0 ? ( intHeight - SCROL_HEIGHT) * intScrollPos / intScrollRange : 0) ;
}


/******************************************************************************/
//		アイテム設定
/******************************************************************************/
// リストクリア
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::ClearList() 
{
	vecList.clear() ;
	vecTime.clear() ;
	intSelected = NO_ITEM ;
	intCurrent = NO_ITEM ;
	intScrollPos = 0 ;

	HDC hdc = GetDC( m_hWnd) ;
	EraseAll( hdc) ;
	ReleaseDC( m_hWnd, hdc) ;
}


/******************************************************************************/
// アイテム追加
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::AddList( const string& s, DWORD d)
{
	vecList.push_back( s) ;
	vecTime.push_back( d / 1000) ;
	SetSize() ;
	InvalidateItem( vecList.size() - 1) ;
}


/******************************************************************************/
// 選択アイテム変更
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::SetSelectedItem( int i)
{
	if( i != NO_ITEM)
	{
		if( i < 0 || i >= vecList.size())
		{
			return ;
		}
	}

	int intPrev = i ;
	intSelected = i;
	InvalidateItem( intPrev) ;
	if( i != NO_ITEM)
	{
		InvalidateItem( i) ;
	}
}


/******************************************************************************/
// 現在アイテム取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::SetCurrentItem( int i)
{
	if( i != NO_ITEM)
	{
		if( i < 0 || i >= vecList.size())
		{
			return ;
		}
	}

	int intPrev = i ;
	intCurrent = i;
	InvalidateItem( intPrev) ;
	if( i != NO_ITEM)
	{
		InvalidateItem( i) ;
	}
}


/******************************************************************************/
// サイズ変更
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::SetSize()
{
	intWidth	= ( Profile::intBlockX + 6) * 25 ;
	intHeight	= Profile::intBlockY * 29 ;
	intLineCount	= intHeight / intLineHeight ;		// 一画面の行数
	intScrollRange	= ( (int)vecList.size() - intLineCount > 0 ? vecList.size() - intLineCount : 0) ;
}


/******************************************************************************/
//		イベント
/******************************************************************************/
// 左クリック
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT ListWnd::OnLButtonDown( WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	SetSelectedItem( GetItemFromPt( pt)) ;
	return 0 ;
}


/******************************************************************************/
// 右クリック
//============================================================================//
// 更新：03/02/02(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT ListWnd::OnRButtonDown( WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	int i = GetItemFromPt( pt) ;
	if( i != NO_ITEM)
	{
		SetSelectedItem( i) ;
	}

	GetCursorPos( &pt) ;
	DWORD dwID = TrackPopupMenu( hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, 
		pt.x, pt.y, 0, m_hWnd, NULL) ;
	switch( dwID)
	{
		case IDM_PLAY:
			ScreenToClient( m_hWnd, &pt) ;
			OnLButtonDblClk( 0, MAKELPARAM( pt.x, pt.y)) ;
			break ;
		case IDM_OPENMINI:
			if( i != NO_ITEM)
			{
				Controller::GetInstance()->OpenInMiniBrowser( i) ;
			}
			break ;
		case IDM_EXTRACT_HERE:
		case IDM_EXTRACT_SELECT:
		case IDM_EXTRACT_DESKTOP:
			if( i != NO_ITEM)
			{
				Controller::GetInstance()->ExtractDetail( i, dwID) ;
			}
			break ;
	}

	return 0 ;
}


/******************************************************************************/
// 左ダブルクリック
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT ListWnd::OnLButtonDblClk( WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	int intItem = GetItemFromPt( pt) ;

	if( intItem != NO_ITEM)
	{
		Controller::GetInstance()->Go( intItem) ;
	}
	return 0 ;
}


/******************************************************************************/
//		描画
/******************************************************************************/
// 描画
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::DrawList( HDC hdc)
{
	if(Profile::blnCompact)
	{
		DrawCompactText( hdc) ;
		return ;
	}
	
	// リスト
	int i ;

	// 塗りつぶし
	HBRUSH hBrush = CreateSolidBrush( colNormalBG) ;
	HBRUSH hBrushOld = (HBRUSH)SelectObject( hdc, (HGDIOBJ)hBrush) ;
	HPEN hPen = CreatePen( PS_SOLID, 1, colNormalBG);
	HPEN hPenOld = (HPEN)SelectObject( hdc, (HGDIOBJ)hPen) ;
	Rectangle( hdc, LIST_LEFT, LIST_TOP, intWidth - LIST_RIGHT, LIST_TOP + Profile::intBlockY * 29) ;
	DeleteObject( SelectObject( hdc, hBrushOld)) ;
	DeleteObject( SelectObject( hdc, hPenOld)) ;

	if( hFont)
	{
		SelectObject( hdc, hFont) ;
	}
	TEXTMETRIC tm ;
	GetTextMetrics( hdc, &tm) ;
	if( intLineHeight != tm.tmHeight)
	{
		intLineHeight = tm.tmHeight ;
		SetSize() ;
	}

	for( i = 0; i < intLineCount ; i++)
	{
		if( i >= vecList.size())
		{
			break ;
		}
		DrawItem( hdc, i) ;
	}
}


/******************************************************************************/
// コンパクトモードの時の文字列表示
//============================================================================//
// 更新：03/04/11(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::DrawCompactText( HDC hdc)
{
	// アイテムがない場合は表示しない
	if(vecList.size() == 0 || vecTime.size() == 0 || intCurrent < 0)
	{
		return ;
	}

	RECT rc ;
	rc.left		= 5 ;
	rc.right	= intWidth - 60;
	rc.top		= 4 ;
	rc.bottom	= 12 ;

	HFONT hFontOld = NULL ;
	if( hFontSmall)
	{
		SelectObject( hdc, hFontSmall) ;
	}

	SetTextColor( hdc, colCurrent) ;
	SetBkColor( hdc, colNormalBG) ;
	SetBkMode( hdc, TRANSPARENT) ;
	HBITMAP hbmpText = pMainWnd->GetFontBmp() ;
	HDC hdcBmp = CreateCompatibleDC( hdc);
	SelectObject( hdcBmp, hbmpText);

	// 時間描画
	if( vecTime[ intCurrent] / 600 > 0)
	{
		BitBlt( hdc, intWidth - 60 +  5, 4, 5, 6, hdcBmp, ( ( vecTime[ intCurrent] / 60) / 10) * 5, 6, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 60 + 10, 4, 5, 6, hdcBmp, ( ( vecTime[ intCurrent] / 60) % 10) * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 60 + 14, 4, 5, 6, hdcBmp, 12 * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 60 + 18, 4, 5, 6, hdcBmp, ( ( vecTime[ intCurrent] % 60) / 10) * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 60 + 23, 4, 5, 6, hdcBmp, ( ( vecTime[ intCurrent] % 60) % 10) * 5, 6, SRCCOPY) ;

	// 文字列描画
	string str = vecList[ intCurrent] ;
	for( int i = 0; i < str.size(); i++)
	{
		char c = str[ i] ;
		if( c >= 'A' && c <= 'Z')
		{
			BitBlt( hdc, rc.left, 4, 5, 6, hdcBmp, ( c - 'A') * 5, 0, SRCCOPY) ;
			rc.left += 5 ;
		}
		else if( c >= 'a' && c <= 'z')
		{
			BitBlt( hdc, rc.left, 4, 5, 6, hdcBmp, ( c - 'a') * 5, 0, SRCCOPY) ;
			rc.left += 5 ;
		}
		else if( c >= '0' && c <= '9')
		{
			BitBlt( hdc, rc.left, 4, 5, 6, hdcBmp, ( c - '0') * 5, 6, SRCCOPY) ;
			rc.left += 5 ;
		}
		else
		{
			char pszBuf[ 3] ;
			pszBuf[ 0] = str[ i] ;
			rc.top -= 3;
			if( i == str.size() - 1)
			{
				pszBuf[ 1] = '\0';
			}
			else
			{
				pszBuf[ 1] = str[ i + 1] ;
			}
			pszBuf[ 2] = '\0' ;
			// ２バイト文字
			if( IsDBCSLeadByte( *pszBuf))
			{
				DrawText( hdc, pszBuf, -1, &rc, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX) ;
				i++ ;
			}
			// 1バイト文字
			else
			{
				pszBuf[ 1] = '\0' ;
				DrawText( hdc, pszBuf, -1, &rc, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX) ;
			}

			SIZE size ;
			GetTextExtentPoint32( hdc, pszBuf, strlen( pszBuf), &size) ;
			rc.left += size.cx ;
			rc.top += 3;
		}

		if( rc.left > rc.right)
		{
			break ;
		}
	}

	if( hFontOld)
	{
		SelectObject( hdc, hFontOld) ;
	}

	DeleteDC( hdcBmp);
}


/******************************************************************************/
// アイテム描画
//============================================================================//
// 更新：03/01/05(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::DrawItem( HDC hdc, int intCount) 
{
	RECT rc ;
	rc.left		= LIST_LEFT + LIST_XMARGIN ;
	rc.right	= intWidth - LIST_RIGHT - LIST_XMARGIN;
	rc.top		= LIST_TOP + intCount * intLineHeight + LIST_YMARGIN ;
	rc.bottom	= LIST_TOP + ( intCount + 1) * intLineHeight + LIST_YMARGIN ;

	// 背景色変更
	int intItem = intCount + intScrollPos ;
	if( intItem == intSelected)
	{
		HBRUSH hBrush = CreateSolidBrush( colSelectedBG) ;
		HBRUSH hBrushOld = (HBRUSH)SelectObject( hdc, (HGDIOBJ)hBrush) ;
		HPEN hPen = CreatePen( PS_SOLID, 1, colSelectedBG);
		HPEN hPenOld = (HPEN)SelectObject( hdc, (HGDIOBJ)hPen) ;
		Rectangle( hdc, rc.left, rc.top, rc.right, rc.bottom) ;
		DeleteObject( SelectObject( hdc, hBrushOld)) ;
		DeleteObject( SelectObject( hdc, hPenOld)) ;
		SetBkColor( hdc, colSelectedBG) ;
	}
	else
	{
		SetBkColor( hdc, colNormalBG) ;
	}

	// 文字色変更
	if( intItem == intCurrent)
	{
		SetTextColor( hdc, colCurrent) ;
	}
	else
	{
		SetTextColor( hdc, colNormal) ;
	}

	// 描画
	if( vecTime[ intItem] != 0)
	{
		char pszBuf[ 256] ;
		SIZE size ;
		wsprintf( pszBuf, "%d:%02d", vecTime[ intItem] / 60, vecTime[ intItem] % 60) ;
		GetTextExtentPoint32( hdc, pszBuf, strlen( pszBuf), &size) ;

		DrawText( hdc, pszBuf, -1, &rc, DT_RIGHT) ;
		rc.right -= size.cx ;
	}
	DrawText( hdc, vecList[ intItem].c_str(), -1, &rc, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX) ;
}


/******************************************************************************/
// 背景を塗りつぶす
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::EraseAll( HDC)
{
}


/******************************************************************************/
// 無効化
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ListWnd::InvalidateItem( int) 
{
	InvalidateRect( m_hWnd, NULL, FALSE) ;
}



/******************************************************************************/
//		ユーティリティー
/******************************************************************************/
// 場所からアイテム番号を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

int ListWnd::GetItemFromPt( POINT pt)
{
	pt.x -= LIST_LEFT + LIST_XMARGIN ;
	pt.y -= LIST_TOP + LIST_YMARGIN ;

	int i = pt.y / intLineHeight + intScrollPos ;
	return ( i < vecList.size() ? i : NO_ITEM) ;
}


/******************************************************************************/
// 色文字列を COLORREF に変換
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

COLORREF ListWnd::GetColor( const string& s)
{
	if( s.size() >= 7)
	{
		BYTE byte[ 3] ;

		for( int i = 0; i < 3; i++)
		{
			byte[ i] = 0 ;
			for( int j = 0; j < 2; j++)
			{
				byte[ i] <<= 4 ;

				if( '0' <= s[ i * 2 + j + 1] && s[ i * 2 + j + 1] <= '9')
				{
					byte[ i] += s[ i * 2 + j + 1] - '0' ;
				}
				else if( 'a' <= s[ i * 2 + j + 1] && s[ i * 2 + j + 1] <= 'f')
				{
					byte[ i] += s[ i * 2 + j + 1] - 'a' + 10 ;
				}
				else if( 'A' <= s[ i * 2 + j + 1] && s[ i * 2 + j + 1] <= 'F')
				{
					byte[ i] += s[ i * 2 + j + 1] - 'A' + 10 ;
				}
			}
		}

		return RGB( byte[ 0], byte[ 1], byte[ 2]) ;
	}

	return 0 ;
}
