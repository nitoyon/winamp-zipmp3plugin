
// MainWnd.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "MainWnd.h"
#include "ListWnd.h"
#include "Profile.h"
#include "Controller.h"
#include "resource.h"
#include <zmouse.h>	// WM_MOUSEWHEEL
#include "main.h"
#include "util\uVersion.h"


/******************************************************************************/
//		透明度設定用
/******************************************************************************/

typedef BOOL(WINAPI *SLWA)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
#ifndef LWA_ALPHA
#define LWA_ALPHA 2
#endif


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// 
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

MainWnd::MainWnd() 
: hbmpPlaylist( NULL), hbmpText( NULL), hbmpTimebar( NULL), strSkinName( "")
, pListWnd( NULL)
, intMin( -1), intSec( -1)
, blnFocus(FALSE), blnResize( FALSE), blnMove( FALSE), blnClose( FALSE), blnScroll( FALSE), blnSnapping( -1)
, blnTimeBarDrag(FALSE)
{
	// 場所
	intLeftPos[ Item::CLOSE]	= -11 ;		intRightPos[  Item::CLOSE]	= -3 ;
	intTopPos[  Item::CLOSE]	= 3 ;		intBottomPos[ Item::CLOSE]	= 12 ;
	intLeftPos[ Item::COMPACT]	= -20 ;		intRightPos[  Item::COMPACT]	= -12 ;
	intTopPos[  Item::COMPACT]	= 3 ;		intBottomPos[ Item::COMPACT]	= 14 ;
	intLeftPos[ Item::PAGEDOWN]	= -15 ;		intRightPos[  Item::PAGEDOWN]	= -8 ;
	intTopPos[  Item::PAGEDOWN]	= -32 ;		intBottomPos[ Item::PAGEDOWN]	= -28 ;
	intLeftPos[ Item::PAGEUP]	= -15 ;		intRightPos[  Item::PAGEUP]	= -8 ;
	intTopPos[  Item::PAGEUP]	= -36 ;		intBottomPos[ Item::PAGEUP]	= -32 ;
	intLeftPos[ Item::PREV]		= -145 ;	intRightPos[  Item::PREV]	= -135 ;
	intTopPos[  Item::PREV]		= -16 ;		intBottomPos[ Item::PREV]	= -8 ;
	intLeftPos[ Item::PLAY]		= -136 ;	intRightPos[  Item::PLAY]	= -126 ;
	intTopPos[  Item::PLAY]		= -16 ;		intBottomPos[ Item::PLAY]	= -8 ;
	intLeftPos[ Item::PAUSE]	= -127 ;	intRightPos[  Item::PAUSE]	= -117 ;
	intTopPos[  Item::PAUSE]	= -16 ;		intBottomPos[ Item::PAUSE]	= -8 ;
	intLeftPos[ Item::STOP]		= -118 ;	intRightPos[  Item::STOP]	= -108 ;
	intTopPos[  Item::STOP]		= -16 ;		intBottomPos[ Item::STOP]	= -8 ;
	intLeftPos[ Item::NEXT]		= -109 ;	intRightPos[  Item::NEXT]	= -99 ;
	intTopPos[  Item::NEXT]		= -16 ;		intBottomPos[ Item::NEXT]	= -8 ;
	intLeftPos[ Item::EJECT]	= -100 ;	intRightPos[  Item::EJECT]	= -90 ;
	intTopPos[  Item::EJECT]	= -16 ;		intBottomPos[ Item::EJECT]	= -8 ;
	intLeftPos[ Item::MOVE]		= 1 ;		intRightPos[  Item::MOVE]	= 0 ;
	intTopPos[  Item::MOVE]		= 1 ;		intBottomPos[ Item::MOVE]	= 0 ;
	intLeftPos[ Item::RESIZE]	= -19 ;		intRightPos[  Item::RESIZE]	= -1 ;
	intTopPos[  Item::RESIZE]	= -19 ;		intBottomPos[ Item::RESIZE]	= -1 ;
	intLeftPos[ Item::TIME]		= -89 ;		intRightPos[  Item::TIME]	= -53 ;
	intTopPos[  Item::TIME]		= -17 ;		intBottomPos[ Item::TIME]	= -8 ;
	intLeftPos[ Item::LIST]		= LIST_LEFT ;	intRightPos[  Item::LIST]	= -LIST_RIGHT ;
	intTopPos[  Item::LIST]		= LIST_TOP ;	intBottomPos[ Item::LIST]	= -38 - LIST_XMARGIN ;
	intLeftPos[ Item::SCROLLBAR]	= -15 ;		intRightPos[  Item::SCROLLBAR]	= -7 ;
	intTopPos[  Item::SCROLLBAR]	= LIST_TOP ;	intBottomPos[ Item::SCROLLBAR]	= -38 ;
	intLeftPos[ Item::TIMEBAR]	= -147 ;	intRightPos[  Item::TIMEBAR]	= -52 ;
	intTopPos[  Item::TIMEBAR]	= -30 ;		intBottomPos[ Item::TIMEBAR]	= -20 ;

	// コンパクト時
	intLeftPos[ Item::COM_LIST]	= 5 ;	intRightPos[  Item::COM_LIST]	= -29 ;
	intTopPos[  Item::COM_LIST]	= 5 ;	intBottomPos[ Item::COM_LIST]	= -4 ;
}


/******************************************************************************/
// 
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

MainWnd::~MainWnd() 
{
}



/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ定義
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_MESSAGE_MAP( MainWndProc, MainWnd)
	ON_MESSAGE( WM_CREATE			, OnCreate)
	ON_MESSAGE( WM_TIMER			, OnTimer)
	ON_MESSAGE( WM_HOTKEY			, OnHotKey)
	ON_MESSAGE( WM_SIZE			, OnSize)
	ON_MESSAGE( WM_SETFOCUS			, OnFocus)
	ON_MESSAGE( WM_KILLFOCUS		, OnKillFocus)
	ON_MESSAGE( WM_PAINT			, OnPaint)
	ON_MESSAGE( WM_LBUTTONDOWN		, OnLButtonDown)
	ON_MESSAGE( WM_RBUTTONDOWN		, OnRButtonDown)
	ON_MESSAGE( WM_LBUTTONUP		, OnLButtonUp)
	ON_MESSAGE( WM_MOUSEMOVE		, OnMouseMove)
	ON_MESSAGE( WM_LBUTTONDBLCLK		, OnLButtonDblClk)
	ON_MESSAGE( WM_MOUSEWHEEL		, OnMouseWheel)
	ON_MESSAGE( WM_KEYDOWN			, OnKeyDown)
	ON_MESSAGE( WM_SYSKEYDOWN		, OnSysKeyDown)
END_MESSAGE_MAP()


/******************************************************************************/
// 作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnCreate( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_CAPTION);

	// ウインドウ作成
	HINSTANCE hInst = GetModuleHandle( NULL) ;
	pListWnd = new ListWnd( this) ;
	pListWnd->Init() ;
	hbmpPlaylist = 0 ;

	SetBlockSize( Profile::intBlockX, Profile::intBlockY) ;
	if(Profile::blnCompact)
	{
		MoveWindow( hWnd, Profile::intX, Profile::intY, intWidth, Y_COMPACT_HEIGHT, FALSE) ;
	}
	else
	{
		MoveWindow( hWnd, Profile::intX, Profile::intY, intWidth, intHeight, FALSE) ;
	}

	// 大麻作成
	SetTimer( hWnd, 0, 100, NULL) ;

	// スキン読み込み
	UpdateSkin( TRUE) ;

	// ウインドウハンドル取得
	hwndWinampEQ = FindWindow( "Winamp EQ", NULL) ;
	hwndWinampPE = FindWindow( "Winamp PE", NULL) ;
	hwndWinampMB = FindWindow( "Winamp MB", NULL) ;

	// 透明度設定
	SetTransparency();

	// 情報ウインドウ表示
	if(Profile::blnInfoVisible)Controller::GetInstance()->DisplayInfoWnd();
	return 0 ;
}


/******************************************************************************/
// タイマー
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnTimer( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateSkin(FALSE) ;

	// 再生中？
	if( SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_ISPLAYING) == 1)
	{
		// データ取得
		ULONG ulMil   = (ULONG)SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) ;
		ULONG ulPos   = (ULONG)SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_GETLISTPOS) ;
		char* pszFile = (char*)SendMessage( hwndWinamp, WM_WA_IPC, ulPos, IPC_GETPLAYLISTFILE) ;

		Controller* pController = Controller::GetInstance() ;
		pController->SetMp3Pos( pszFile ? pszFile : "", ulMil) ;
	}
	return 0;
}


/******************************************************************************/
// ホットキー
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnHotKey( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ShowWindow( m_hWnd, IsWindowVisible( m_hWnd) ? SW_HIDE : SW_SHOW) ;
	return 0 ;
}


/******************************************************************************/
// サイズ変更
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnSize( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return -1 ;
}


/******************************************************************************/
// フォーカス
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnFocus( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	blnFocus = TRUE ;
	InvalidateRect(hWnd, NULL, FALSE);
	return 0 ;
}


/******************************************************************************/
// フォーカスを失う
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnKillFocus( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	blnFocus = FALSE ;
	InvalidateRect(hWnd, NULL, FALSE);
	return 0 ;
}


/******************************************************************************/
// 描画
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnPaint( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps ;
	HDC hdc = BeginPaint (hWnd, &ps) ;
	HBITMAP hbmpBk;
	HDC hdcBk;
	HBITMAP hbmpOld;

	if(Profile::blnCompact)
	{
		// メモリ内のビットマップ作成
		hbmpBk = CreateCompatibleBitmap( hdc, intWidth, Y_COMPACT_HEIGHT) ;
		hdcBk = CreateCompatibleDC( hdc);
		hbmpOld = (HBITMAP)SelectObject( hdcBk, hbmpBk);

		// 描画
		DrawSkinCompact( hdcBk) ;
		pListWnd->DrawList( hdcBk) ;
		DrawTime( hdcBk) ;
	}
	else
	{
		// メモリ内のビットマップ作成
		hbmpBk = CreateCompatibleBitmap( hdc, intWidth, intHeight) ;
		hdcBk = CreateCompatibleDC( hdc);
		hbmpOld = (HBITMAP)SelectObject( hdcBk, hbmpBk);

		// 描画
		DrawSkinNormal( hdcBk) ;
		pListWnd->DrawList( hdcBk) ;
		DrawTime( hdcBk) ;
	}

	// 表に描画
	BitBlt( hdc, 0, 0, intWidth, intHeight, hdcBk, 0, 0, SRCCOPY) ;

	// 破棄
	SelectObject( hdcBk, hbmpOld) ;
	DeleteDC( hdcBk) ;
	DeleteObject( hbmpBk) ;

	EndPaint (hWnd, &ps) ;
	return 0 ;
}


/******************************************************************************/
// 破壊
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnDestroy( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0) ;
	delete pListWnd ;
	return 0 ;
}


/******************************************************************************/
// マウスダウン
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnLButtonDown( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	Item item = GetItem( pt) ;

	switch( item)
	{
		case CLOSE :
			::ShowWindow( hWnd, SW_HIDE) ;
			break ;

		case COMPACT :
			ToggleCompact();
			break ;

		case MOVE :
		{
			blnMove = TRUE ;
			RECT rc ;
			GetWindowRect( hWnd, &rc) ;

			ptOffsetMove.x = pt.x ;
			ptOffsetMove.y = pt.y ;
			SetCapture( hWnd) ;
			break ;
		}

		case TIME :
			Profile::blnCountUp = !Profile::blnCountUp ;
			InvalidateItem( Item::TIME) ;
			break ;
	}

	if( Profile::blnCompact)
	{
		return 0;
	}

	switch( item)
	{
		case RESIZE :
			blnResize = TRUE ;
			SetCapture( hWnd) ;
			break ;

		case PREV:
		case NEXT:
			Controller::GetInstance()->GoNext(item == NEXT) ;
			break ;

		case PLAY:
			Controller::GetInstance()->Play() ;
			break ;
		case PAUSE:
			SendMessage( hwndWinamp, WM_COMMAND, WINAMP_BUTTON3, 0) ;
			break ;
		case STOP:
			SendMessage( hwndWinamp, WM_COMMAND, WINAMP_BUTTON4, 0) ;
			break ;

		case LIST:
			return pListWnd->OnLButtonDown( wParam, lParam) ;
		case PAGEUP:
			pListWnd->PageUp() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case PAGEDOWN:
			pListWnd->PageDown() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case SCROLLBAR:
			pListWnd->ScrollTo( pt.y - LIST_TOP) ;
			blnScroll = TRUE ;
			SetCapture( hWnd) ;
			InvalidateItem( SCROLLBAR) ;
			break ;

		case TIMEBAR:
			blnTimeBarDrag = TRUE ;
			SetCapture( hWnd) ;

			POINT pt ;
			GetCursorPos( &pt) ;
			RECT rect ;
			GetWindowRect( hWnd, &rect) ;
			pt.x -= rect.left + intWidth + intLeftPos[ Item::TIMEBAR] ;
			intTimeBarDrag = ( pt.x < 0 ? 0 : ( pt.x >= 95 - 30 ? 95 - 30 : pt.x)) ;

			InvalidateItem( TIMEBAR) ;

			break ;
		}
	return 0 ;
}


/******************************************************************************/
// 右クリック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnRButtonDown( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt = {0, 0};
	if(lParam != -1)
	{
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
	}
	else
	{
		ClientToScreen(hWnd, &pt);
	}
	Item item = GetItem( pt) ;

	// リストで右クリックだとリストに任せる
	if( item == LIST)
	{
		pListWnd->OnRButtonDown( wParam, lParam) ;
		return 0;
	}

	// メニュー準備
	HMENU hMenuPopupMain = LoadMenu(Profile::hInstance, MAKEINTRESOURCE(IDR_ITEMPOPUP)) ;
	HMENU hMenuPopup = GetSubMenu( hMenuPopupMain, 1) ;
	if(lParam != -1)GetCursorPos(&pt) ;
	HMENU hMenuMove = GetSubMenu(hMenuPopup, 8);
	for(int i = 0; i < pListWnd->GetSize(); i++)
	{
		if(i == 0)
		{
			AppendMenu(hMenuMove, MF_SEPARATOR, 0, NULL);
		}
		string strItemName = pListWnd->GetItemName(i);
		strItemName += (i >= 9 ? "(&0)" : string("(&") + (char)('1' + i) + ")");
		AppendMenu(hMenuMove, MF_STRING, IDM_5NEXT + 1 + i, strItemName.c_str());
		if(GetCurSong() == i)
		{
			CheckMenuRadioItem(hMenuMove, IDM_5NEXT + 1, IDM_5NEXT + 1 + i, IDM_5NEXT + 1 + i, MF_BYCOMMAND);
		}
	}

	// メニュー表示
	CheckMenuRadioItem(hMenuPopup, IDM_NORMAL, IDM_ALBUMRANDOM, Profile::intRepeat + IDM_NORMAL, MF_BYCOMMAND);
	DWORD dwID = TrackPopupMenu(hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, 
		pt.x, pt.y, 0, m_hWnd, NULL) ;
	DestroyMenu(hMenuPopupMain);
	DestroyMenu(hMenuPopup);
	DestroyMenu(hMenuMove);

	// 実行
	switch(dwID)
	{
		case IDM_SETTING:
			config() ;
			break;
		case IDM_NORMAL:
		case IDM_SONGREPEAT:
		case IDM_ALBUMREPEAT:
		case IDM_ENDLESSRANDOM:
		case IDM_ALBUMRANDOM:
			Profile::intRepeat = dwID - IDM_NORMAL;
			break;

		case IDM_PREV:
		case IDM_NEXT:
			Controller::GetInstance()->GoNext(item == NEXT);
			break;
		case IDM_5PREV:
		case IDM_5NEXT:
			SendMessage(hwndWinamp, WM_COMMAND, (dwID == IDM_5PREV ? WINAMP_REW5S : WINAMP_FFWD5S), 0);
			break;
	}

	if(dwID >= IDM_5NEXT + 1)
	{
		Controller* pController = Controller::GetInstance() ;
		pController->Go(dwID - IDM_5NEXT - 1);
	}

	return 0 ;
}


/******************************************************************************/
// マウスアップ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnLButtonUp( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( blnResize || blnMove)
	{
		blnResize = FALSE ;
		blnMove = FALSE ;
		ReleaseCapture() ;
	}
	else if( blnScroll)
	{
		blnScroll = FALSE ;
		ReleaseCapture() ;
		InvalidateItem( Item::SCROLLBAR) ;
	}
	else if( blnTimeBarDrag)
	{
		blnTimeBarDrag = FALSE;
		ReleaseCapture() ;
		InvalidateItem( Item::TIMEBAR) ;

		Controller* pController = Controller::GetInstance() ;
		pController->Go(pListWnd->GetCurrentItem(), pListWnd->GetCurrentLength() * 1000 * ( intTimeBarDrag - 15) / ( 95 - 30)) ;
	}

	return 0 ;
}


/******************************************************************************/
// マウスドラッグ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnMouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( blnResize)
	{
		POINT pt ;
		GetCursorPos( &pt) ;
		RECT rect ;
		GetWindowRect( hWnd, &rect) ;
		pt.x -= rect.left ;
		pt.y -= rect.top ;

		int intNewBlockX = ( pt.x >= 0 ? ( pt.x - X_BLOCK_CONST) / X_BLOCK_SIZE + 1 : XMIN_BLOCK) ;
		intNewBlockX = ( intNewBlockX >= XMIN_BLOCK ? intNewBlockX : XMIN_BLOCK) ;
		int intNewBlockY = ( pt.y >= 0 ? ( pt.y - Y_BLOCK_CONST) / Y_BLOCK_SIZE + 1 : YMIN_BLOCK) ;
		intNewBlockY = ( intNewBlockY >= YMIN_BLOCK ? intNewBlockY : YMIN_BLOCK) ;

		if( intNewBlockX != Profile::intBlockX || intNewBlockY != Profile::intBlockY)
		{
			SetBlockSize( intNewBlockX, intNewBlockY) ;

			RECT rc ;
			GetWindowRect( hWnd, &rc) ;
			MoveWindow( hWnd, rc.left, rc.top, intWidth, intHeight, TRUE) ;
			InvalidateRect( hWnd, NULL, FALSE) ;
		}
	}
	else if( blnMove)
	{
		POINT pt ;
		GetCursorPos( &pt) ;
		pt.x -= ptOffsetMove.x ;
		pt.y -= ptOffsetMove.y ;
		if(Profile::blnAttachToWinamp)
		{
			pt = CalcSnappedPos( pt) ;
		}
		Profile::intX = pt.x ;
		Profile::intY = pt.y ;
		SetWindowPos( hWnd, 0, Profile::intX, Profile::intY, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ;
	}
	else  if( blnScroll)
	{
		pListWnd->ScrollTo( HIWORD( lParam) - LIST_TOP) ;
	}
	else if( blnTimeBarDrag)
	{
		POINT pt ;
		GetCursorPos( &pt) ;
		RECT rect ;
		GetWindowRect( hWnd, &rect) ;
		pt.x -= rect.left + intWidth + intLeftPos[ Item::TIMEBAR] ;
		intTimeBarDrag = ( pt.x < 15 ? 15 : ( pt.x >= 95 - 15 ? 95 - 15 : pt.x)) ;

		InvalidateItem( Item::TIMEBAR) ;
	}
	return 0 ;
}


/******************************************************************************/
// ダブルクリック
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnLButtonDblClk( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	Item item = GetItem( pt) ;

	if( item == Item::LIST)
	{
		pListWnd->OnLButtonDblClk( wParam, lParam) ;
	}
	else if( pt.y < Y_COMPACT_HEIGHT)
	{
		ToggleCompact();
	}
	return 0 ;
}


/******************************************************************************/
// マウスホイール
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnMouseWheel( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( (short)HIWORD( wParam) > 0)
	{
		pListWnd->PageUp() ;
	}
	else
	{
		pListWnd->PageDown() ;
	}

	return 0 ;
}


/******************************************************************************/
// キーダウン
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case VK_UP:
			pListWnd->ScrollUp() ;
			break ;
		case VK_DOWN:
			pListWnd->ScrollDown() ;
			break ;
		case VK_PRIOR:
			pListWnd->PageUp() ;
			break ;
		case VK_NEXT:
			pListWnd->PageDown() ;
			break ;
		case VK_RETURN:
			Controller::GetInstance()->Go( pListWnd->GetSelectedItem()) ;
			break ;
		case VK_APPS:
			OnRButtonDown(hWnd, 0, -1); 
			break;

		case VK_RIGHT:
		case VK_LEFT:
			SendMessage(hwndWinamp, WM_COMMAND, (wParam == VK_LEFT ? WINAMP_REW5S : WINAMP_FFWD5S), 0);
			break;

		case VK_OEM_COMMA:
		case VK_OEM_PERIOD:
			Controller::GetInstance()->GoNext(wParam == VK_OEM_PERIOD) ;
			break ;
	}

	return 0 ;
}


/******************************************************************************/
// システムキーダウン
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT MainWnd::OnSysKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	if( wParam == 'M' && lParam & 2 << 28)
	{
		Controller::GetInstance()->ToggleVisiblity() ;
		return 0 ;
	}
	else if( wParam == '3' && lParam & 2 << 28)
	{
		if(Profile::blnInfoVisible)
		{
			Controller::GetInstance()->CloseInfoWnd();
		}
		else
		{
			Controller::GetInstance()->DisplayInfoWnd();
		}
		return 0 ;
	}
	else if(wParam == VK_F4 && lParam & 2 << 28)
	{
		SendMessage(hwndWinamp, WM_CLOSE, 0, 0);
		return 0 ;
	}
	return -1 ;
}


/******************************************************************************/
//		描画
/******************************************************************************/
// 現在のスキン読みとり
//============================================================================//
// 概要：スキンが変更されていれば、スキン情報を更新する。
// 補足：なし。
//============================================================================//

void MainWnd::UpdateSkin( BOOL blnForce)
{
	char pszBuf[ MAX_PATH + 1] ;

	// スキンのパス取得
	string strCurSkinName = (char*)SendMessage( hwndWinamp, WM_WA_IPC,(WPARAM)pszBuf, IPC_GETSKIN);
	string strSkinPath = pszBuf;

	// winamp がスキンを使っているか
	BOOL bln1 = (strSkinPath == "");

	// plugin のスキン
	if(bln1 && Profile::intSkin1 == 0 || !bln1 && Profile::intSkin2 == 1)
	{
		if(blnForce || strSkinName != strCurSkinName)
		{
			UnloadSkin();
			LoadPluginSkin();
			InvalidateRect( m_hWnd, NULL, TRUE);
		}
	}
	// 外部スキン
	else if(bln1 && Profile::intSkin1 == 1 || !bln1 && Profile::intSkin2 == 2)
	{
		if(blnForce || strSkinName != strCurSkinName)
		{
			UnloadSkin();
			LoadPathSkin(bln1 ? Profile::strSkinDir1 : Profile::strSkinDir2);
			InvalidateRect( m_hWnd, NULL, TRUE);
		}
	}
	// Winamp と同じスキン
	else
	{
		if(blnForce || strSkinName != strCurSkinName)
		{
			UnloadSkin();
			strSkinName = strCurSkinName;
			LoadPathSkin(strSkinPath);
			InvalidateRect( m_hWnd, NULL, TRUE);
		}
	}
}


/******************************************************************************/
// スキンのビットマップをアンロードする
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::UnloadSkin()
{
	// オブジェクトの削除
	if( hbmpPlaylist)
	{
		DeleteObject( hbmpPlaylist) ;
		hbmpPlaylist = NULL ;
	}
	if( hbmpText)
	{
		DeleteObject( hbmpText) ;
		hbmpText = NULL ;
	}
	if( hbmpTimebar)
	{
		DeleteObject( hbmpTimebar) ;
		hbmpTimebar = NULL ;
	}
}


/******************************************************************************/
// 特定のパスのスキンをロードする
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::LoadPathSkin(const string& strSkinPath)
{
	// ビットマップのロード
	DWORD  dw ;

	// プレイリスト
	string strBmp = strSkinPath + "\\Pledit.bmp";
	dw = GetFileAttributes( strBmp.c_str()) ;
	if(hbmpPlaylist == NULL && dw != -1 && ( dw & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	{
		hbmpPlaylist = (HBITMAP)LoadImage( NULL, strBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE) ;
	}

	// テキスト
	strBmp = ( strSkinPath + "\\text.bmp") ;
	dw = GetFileAttributes( strBmp.c_str()) ;
	if(hbmpText == NULL && dw != -1 && ( dw & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	{
		hbmpText = (HBITMAP)LoadImage( NULL, strBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE) ;
	}

	// タイムバー
	strBmp = ( strSkinPath + "\\Posbar.bmp") ;
	dw = GetFileAttributes( strBmp.c_str()) ;
	if(hbmpTimebar == NULL && dw != -1 && ( dw & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
	{
		hbmpTimebar = (HBITMAP)LoadImage( NULL, strBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE) ;
	}

	// 色
	pListWnd->SetSkin( strSkinPath + "\\Pledit.txt") ;

	// ロードできなかったプラグインは、DLL のもので補う
	if(hbmpPlaylist == NULL || hbmpText == NULL || hbmpTimebar == NULL)
	{
		LoadPluginSkin();
	}
}


/******************************************************************************/
// プラグインのスキンをロードする
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::LoadPluginSkin()
{
	if(hbmpPlaylist == NULL || hbmpText == NULL || hbmpTimebar == NULL)
	{
		pListWnd->SetSkin("");
	}

	if( hbmpPlaylist == NULL)
	{
		hbmpPlaylist = (HBITMAP)LoadImage( Profile::hInstance, MAKEINTRESOURCE( IDB_PLEDIT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR) ;
	}
	if( hbmpText == NULL)
	{
		hbmpText = (HBITMAP)LoadImage( Profile::hInstance, MAKEINTRESOURCE( IDB_TEXT), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR) ;
	}
	if( hbmpTimebar == NULL)
	{
		hbmpTimebar = (HBITMAP)LoadImage( Profile::hInstance, MAKEINTRESOURCE( IDB_POSBAR), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR) ;
	}
}


/******************************************************************************/
// 通常状態のスキンを表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::DrawSkinNormal( HDC hdc)
{
	if( !hbmpPlaylist)
	{
		return ;
	}

	HDC hdcBmp = CreateCompatibleDC( hdc);
	SelectObject( hdcBmp, hbmpPlaylist);

	int i ;
	int intCenterX = ( intWidth - 100) / 2 ;

	// ヘッダ
	BitBlt( hdc,   0,   0,  25, 20, hdcBmp,   0,  blnFocus ? 0 : 21, SRCCOPY) ;	// 左端
	BitBlt( hdc,  25,   0,  25, 20, hdcBmp,  127, blnFocus ? 0 : 21, SRCCOPY) ;
	for( i = 0; i < Profile::intBlockX / 2; i++)
	{
		BitBlt( hdc,  intCenterX - 25 * ( i + 1),   0,  25, 20, hdcBmp, 127, blnFocus ? 0 : 21, SRCCOPY) ;
	}
	BitBlt( hdc, intCenterX,         0, 100, 20, hdcBmp,   26, blnFocus ? 0 : 21, SRCCOPY) ;	// プレイリストの文字
	BitBlt( hdc, intCenterX + 100,   0,  25, 20, hdcBmp,  127, blnFocus ? 0 : 21, SRCCOPY) ;
	for( i = 0; i < Profile::intBlockX / 2; i++)
	{
		BitBlt( hdc, intWidth - 25 * ( i + 2),   0,  25, 20, hdcBmp, 127, blnFocus ? 0 : 21, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 25,   0,  25, 20, hdcBmp, 153, blnFocus ? 0 : 21, SRCCOPY) ;	// 右端

	// 中央
	for( i = 0; i < Profile::intBlockY; i++)
	{
		BitBlt( hdc,   0,  20 + 29 * i,  25, 29, hdcBmp,   0, 42, SRCCOPY) ;
		BitBlt( hdc, intWidth - 25,  20 + 29 * i,  25, 29, hdcBmp,   26, 42, SRCCOPY) ;
	}

	// フッタ
	BitBlt( hdc,   0,  intHeight - 38, 125,  38, hdcBmp,   0, 72, SRCCOPY) ;
	for( i = 0; i < ( intWidth - 275) / 25; i++)
	{
		BitBlt( hdc, 125 + i * 25, intHeight - 38, 25, 38, hdcBmp, 179, 0, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 150,  intHeight - 38, 150,  38, hdcBmp, 126, 72, SRCCOPY) ;

	// スクロールバー
	BitBlt( hdc,  intWidth - 15, 20 + pListWnd->GetScrollBarPos(), 8, 18, hdcBmp, 52 + ( blnScroll ? 9 : 0), 53, SRCCOPY) ;

	DeleteDC( hdcBmp);
}


/******************************************************************************/
// コンパクト状態のスキンを表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::DrawSkinCompact( HDC hdc)
{
	if( !hbmpPlaylist)
	{
		return ;
	}

	HDC hdcBmp = CreateCompatibleDC( hdc);
	SelectObject( hdcBmp, hbmpPlaylist);

	int i ;

	// ヘッダ
	BitBlt( hdc,   0,   0,  25, 14, hdcBmp,   72, 42, SRCCOPY) ;	// 左端
	BitBlt( hdc,  25,   0,  25, 14, hdcBmp,   72, 57, SRCCOPY) ;
	for( i = 0; i < Profile::intBlockX + 3; i++)
	{
		BitBlt( hdc,  25 + 25 * i,   0,  25, 14, hdcBmp, 72, 57, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 50,   0,  50, 14, hdcBmp, 99, 42 + (blnFocus ? 0 : 15), SRCCOPY) ;	// 右端

	DeleteDC( hdcBmp);
}


/******************************************************************************/
// 時間を表示
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::DrawTime( HDC hdc)
{
	if( intMin == -1 || intSec == -1 || intSec > 60)
	{
		return ;
	}

	HDC hdcFont = CreateCompatibleDC( hdc);
	HDC hdcPos = CreateCompatibleDC( hdc);
	SelectObject( hdcFont, hbmpText);
	SelectObject( hdcPos,  hbmpTimebar);

	if(Profile::blnCompact)
	{
/*		// タイマー
		if( !Profile::blnCountUp)
		{
			BitBlt( hdc, intWidth - 60     , 4, 5, 6, hdcBmp, 15 * 5, 6, SRCCOPY) ;
		}
		BitBlt( hdc, intWidth - 60 +  5, 4, 5, 6, hdcFont, ( intMin / 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 60 + 10, 4, 5, 6, hdcFont, ( intMin % 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 60 + 14, 4, 5, 6, hdcFont, 12 * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 60 + 18, 4, 5, 6, hdcFont, ( intSec / 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 60 + 23, 4, 5, 6, hdcFont, ( intSec % 10) * 5, 6, SRCCOPY) ;*/
	}
	else if(dwCurSongLength != 0)
	{
		// 時間表示
		if( !Profile::blnCountUp)
		{
			BitBlt( hdc, intWidth - 82     , intHeight - 15, 5, 6, hdcFont, 15 * 5, 6, SRCCOPY) ;
		}
		BitBlt( hdc, intWidth - 82 +  5, intHeight - 15, 5, 6, hdcFont, ( intMin / 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 82 + 10, intHeight - 15, 5, 6, hdcFont, ( intMin % 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 82 + 18, intHeight - 15, 5, 6, hdcFont, ( intSec / 10) * 5, 6, SRCCOPY) ;
		BitBlt( hdc, intWidth - 82 + 23, intHeight - 15, 5, 6, hdcFont, ( intSec % 10) * 5, 6, SRCCOPY) ;

		// バー表示
		if( Profile::blnUseTimebar)
		{
			BitBlt( hdc, intWidth - 147, intHeight - 30, 95, 10, hdcPos, 0, 0, SRCCOPY) ;
			UINT uiPos = 95 - 30 ;
			if( Profile::blnCountUp)
			{
				uiPos = uiPos * ( intMin * 60 + intSec) * 1000 / dwCurSongLength ;
			}
			else
			{
				uiPos = uiPos * ( dwCurSongLength - ( intMin * 60 + intSec) * 1000) / dwCurSongLength ;
			}

			// ドラッグ中
			if( !blnTimeBarDrag)
			{
				BitBlt( hdc, intWidth - 147 + uiPos, intHeight - 30, 29, 10, hdcPos, 248, 0, SRCCOPY) ;
			}
			else
			{
				BitBlt( hdc, intWidth - 147 + intTimeBarDrag - 15, intHeight - 30, 29, 10, hdcPos, 278, 0, SRCCOPY) ;
			}
		}
	}

	DeleteDC( hdcFont);
	DeleteDC( hdcPos);
}


/******************************************************************************/
//		設定
/******************************************************************************/
// サイズ変更
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::SetBlockSize( int intNewBlockX, int intNewBlockY)
{
	Profile::intBlockX = intNewBlockX ;
	Profile::intBlockY = intNewBlockY ;
	intWidth  = X_BLOCK_CONST + X_BLOCK_SIZE * Profile::intBlockX ;
	intHeight = Y_BLOCK_CONST + Y_BLOCK_SIZE * Profile::intBlockY ;

	if( pListWnd)
	{
		pListWnd->SetSize() ;
	}
}


/******************************************************************************/
// 文字設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::SetTime( int _intMin, int _intSec)
{
	intMin = _intMin ;
	intSec = _intSec ;
	InvalidateItem( Item::TIME) ;
	InvalidateItem( Item::TIMEBAR) ;
}


/******************************************************************************/
// カレントアイテムを変更
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::SetCurSong( int i, DWORD dw)
{
	pListWnd->SetCurrentItem( i) ;
	dwCurSongLength = dw ;
}


/******************************************************************************/
// リストクリア
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::ClearList()
{
	pListWnd->ClearList() ;
}


/******************************************************************************/
// リスト追加
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::AddList( const string& s, DWORD dwLength)
{
	pListWnd->AddList( s, dwLength) ;
}


/******************************************************************************/
// コンパクトモードの切り替え
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::ToggleCompact()
{
	Profile::blnCompact = !Profile::blnCompact;
	RECT rect ;
	GetWindowRect( m_hWnd, &rect) ;
	if(Profile::blnCompact)
	{
		MoveWindow( m_hWnd, rect.left, rect.top, intWidth, Y_COMPACT_HEIGHT, TRUE) ;
		intHeight = Y_COMPACT_HEIGHT;
		InvalidateRect( m_hWnd, NULL, TRUE) ;
	}
	else
	{
		intHeight = Y_BLOCK_CONST + Y_BLOCK_SIZE * Profile::intBlockY ;
		MoveWindow( m_hWnd, rect.left, rect.top, intWidth, intHeight, TRUE) ;
		InvalidateRect( m_hWnd, NULL, TRUE) ;
	}
}


/******************************************************************************/
// 透明度の設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::SetTransparency() const
{
	if(IsNT() && GetOsMajorVersion() >= 5)
	{
		int intTrans = -1;

		// 独自の透明度
		if(0 <= Profile::intTransparency && Profile::intTransparency <= Profile::intTransparency)
		{
			intTrans = 100 - Profile::intTransparency;
		}
		// SexyFont から読み取り
		else if(Profile::intTransparency == -1)
		{
			HKEY hkey;
			if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\YunaSoft\\SexyFont Plug-in\\Settings\\", NULL, KEY_READ, &hkey) == ERROR_SUCCESS)
			{
				DWORD dwData = 0;
				DWORD dwSize = sizeof(DWORD);
				DWORD dwType;
				LONG ret = RegQueryValueEx(hkey, "AlphaWinamp", NULL, &dwType, (LPBYTE)&dwData, &dwSize);
				if(ret == ERROR_SUCCESS && dwData == 1)
				{
					DWORD dwSize = sizeof(DWORD);
					LONG ret = RegQueryValueEx(hkey, "OpacityWinamp", NULL, &dwType, (LPBYTE)&dwData, &dwSize);

					if(ret == ERROR_SUCCESS && dwType == REG_DWORD && 0 <= dwData || dwData <= 100)
					{
						intTrans = 100 - Profile::intTransparency;
						RegCloseKey(hkey);
						return;
					}
				}
				RegCloseKey(hkey);
			}
		}
		else
		{
			intTrans = 100;
		}

		// 読み取り失敗時
		HINSTANCE hUser32Dll = LoadLibrary("user32.dll");
		if(hUser32Dll)
		{
			SLWA dllSetLayeredWindowAttributes;
			if(dllSetLayeredWindowAttributes = (SLWA)GetProcAddress(hUser32Dll,"SetLayeredWindowAttributes"))
			{
				dllSetLayeredWindowAttributes(m_hWnd, 0, intTrans * 255 / 100, LWA_ALPHA);
			}
			FreeLibrary(hUser32Dll);
		}
	}
}


/******************************************************************************/
//		取得
/******************************************************************************/
// 現在の曲を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int MainWnd::GetCurSong() const
{
	return pListWnd->GetCurrentItem() ;
}


/******************************************************************************/
// スナップ中か取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL MainWnd::IsSnapping()
{
	// 起動時の情報がないとき
	if( blnSnapping == -1)
	{
		POINT pt = { Profile::intX, Profile::intY} ;
		CalcSnappedPos( pt) ;
	}
	return blnSnapping ;
}


/******************************************************************************/
//		ユーティリティ
/******************************************************************************/
// 場所→アイテム
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

MainWnd::Item MainWnd::GetItem( POINT pt)
{
	for( int i = 0; i < Item::LAST; i++)
	{
		if( ( intLeftPos[   i] >= 0 ? pt.x >= intLeftPos[   i] : pt.x >= intWidth + intLeftPos[ i])
		 && ( intRightPos[  i] >= 0 ? pt.x <= intRightPos[  i] : pt.x <= intWidth + intRightPos[ i])
		 && ( intTopPos[    i] >= 0 ? pt.y >= intTopPos[    i] : pt.y >= intHeight + intTopPos[ i])
		 && ( intBottomPos[ i] >= 0 ? pt.y <= intBottomPos[ i] : pt.y <= intHeight + intBottomPos[ i]))
		{
			if( i == Item::COM_LIST)
			{
				continue ;
			}
			return (Item)i;
		}
	}

	return Item::MOVE ;
}


/******************************************************************************/
// アイテムを再描画
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void MainWnd::InvalidateItem( MainWnd::Item item)
{
	if( item == Item::MOVE)
	{
		return ;
	}

	if( item == Item::LIST && Profile::blnCompact)
	{
		item = Item::COM_LIST;
	}

	RECT rc ;
	rc.left   = ( intLeftPos[   item] >= 0 ? intLeftPos[   item] : intWidth + intLeftPos[ item]) ;
	rc.right  = ( intRightPos[  item] >= 0 ? intRightPos[  item] : intWidth + intRightPos[ item]) ;
	rc.top    = ( intTopPos[    item] >= 0 ? intTopPos[    item] : intHeight + intTopPos[ item]) ;
	rc.bottom = ( intBottomPos[ item] >= 0 ? intBottomPos[ item] : intHeight + intBottomPos[ item]) ;

	// コンパクト時の時間
/*	if( Profile::blnCompact && item == Item::TIME)
	{
		rc.left		= intWidth - 60;
		rc.top		= 4;
		rc.right	= intWidth - 60 + 28;
		rc.bottom	= 10;
	}*/

	InvalidateRect( m_hWnd, &rc, TRUE) ;
}


/******************************************************************************/
// 移動時の位置補正
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

POINT MainWnd::CalcSnappedPos( POINT pt)
{
	int intIsSnap = GetPrivateProfileInt( "Winamp", "snap", 1, Profile::strWinampIniPath.c_str()) ;
	int intSnap = GetPrivateProfileInt( "Winamp", "snaplen", 10, Profile::strWinampIniPath.c_str()) ;
	if( intIsSnap != 1)
	{
		intSnap = 0 ;
	}

	RECT rcWnd = { pt.x, pt.y, pt.x + intWidth, pt.y + intHeight} ;
	HWND hwnd[] = { hwndWinamp, hwndWinampEQ, hwndWinampPE, hwndWinampMB} ;
	RECT rcWinamp ;
	GetWindowRect( hwndWinamp, &rcWinamp) ;
	blnSnapping = FALSE ;
	for( int intIndex = 0; intIndex < sizeof( hwnd) / sizeof( HWND); intIndex++)
	{
		if( IsWindowVisible( hwnd[ intIndex]))
		{
			RECT rc ;
			GetWindowRect( hwnd[ intIndex], &rc) ;

			// 角のチェック
			int i ;
			for( i = 0; i < 4; i++)
			{
				POINT ptCorner = { i / 2 ? rc.right : rc.left, i % 2 ? rc.bottom : rc.top} ;
				for( int j = 0; j < 4; j++)
				{
					if( i == j)
					{
						continue ;
					}

					POINT ptWnd = { j / 2 ? rcWnd.right : rcWnd.left, j % 2 ? rcWnd.bottom : rcWnd.top} ;

					if( ptCorner.x - intSnap <= ptWnd.x && ptWnd.x <= ptCorner.x + intSnap
					 && ptCorner.y - intSnap <= ptWnd.y && ptWnd.y <= ptCorner.y + intSnap)
					{
						blnSnapping = TRUE ;
						pt.x += ptCorner.x - ptWnd.x ;
						pt.y += ptCorner.y - ptWnd.y ;
						ptOffsetSnap.x = pt.x - rcWinamp.left ;
						ptOffsetSnap.y = pt.y - rcWinamp.top ;
						return pt ;
					}
				}
			}

			// 縁のチェック
			BOOL blnY = rc.top <= rcWnd.top && rcWnd.top <= rc.bottom || rc.top <= rcWnd.bottom && rcWnd.bottom <= rc.bottom ;
			BOOL blnX = rc.left <= rcWnd.left && rcWnd.left <= rc.right || rc.left <= rcWnd.right && rcWnd.right <= rc.right ;

			// 右辺
			if( rc.right - intSnap <= rcWnd.left && rcWnd.left <= rc.right + intSnap && blnY)
			{
				blnSnapping = TRUE ;
				pt.x = rc.right ;
				ptOffsetSnap.x = pt.x - rcWinamp.left ;
				ptOffsetSnap.y = pt.y - rcWinamp.top ;
			}

			// 左辺
			if( rc.left - intSnap <= rcWnd.right && rcWnd.right <= rc.left + intSnap && blnY)
			{
				blnSnapping = TRUE ;
				pt.x = rc.left - intWidth ;
				ptOffsetSnap.x = pt.x - rcWinamp.left ;
				ptOffsetSnap.y = pt.y - rcWinamp.top ;
			}

			// 上辺
			if( rc.top - intSnap <= rcWnd.bottom && rcWnd.bottom <= rc.top + intSnap && blnX)
			{
				blnSnapping = TRUE ;
				pt.y = rc.top - intHeight ;
				ptOffsetSnap.x = pt.x - rcWinamp.left ;
				ptOffsetSnap.y = pt.y - rcWinamp.top ;
			}

			// 下辺
			if( rc.bottom - intSnap <= rcWnd.top && rcWnd.top <= rc.bottom + intSnap && blnX)
			{
				blnSnapping = TRUE ;
				pt.y = rc.bottom ;
				ptOffsetSnap.x = pt.x - rcWinamp.left ;
				ptOffsetSnap.y = pt.y - rcWinamp.top ;
			}
		}
	}

	return pt ;
}
