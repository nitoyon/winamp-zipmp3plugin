
// MainWnd.cpp
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "MainWnd.h"
#include "ListWnd.h"
#include "Profile.h"
#include "Controller.h"

#define  IDC_TITLE  100


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// 
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

MainWnd::MainWnd() 
: hbmpPlaylist( NULL), hbmpText( NULL), strSkinName( ""), strSkinPath( "")
, pListWnd( NULL)
, intMin( -1), intSec( -1)
, blnResize( FALSE), blnMove( FALSE), blnClose( FALSE), blnScroll( FALSE)
{
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
	intLeftPos[ Item::SCROLLBAR]	= -15 ;		intRightPos[  Item::SCROLLBAR]	= -8 ;
	intTopPos[  Item::SCROLLBAR]	= LIST_TOP ;	intBottomPos[ Item::SCROLLBAR]	= -38 ;
}


/******************************************************************************/
// 
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

MainWnd::~MainWnd() 
{
}



/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_MESSAGE_MAP( MainWndProc, MainWnd)
	ON_MESSAGE( WM_CREATE			, OnCreate)
	ON_MESSAGE( WM_TIMER			, OnTimer)
	ON_MESSAGE( WM_SIZE			, OnSize)
	ON_MESSAGE( WM_SETFOCUS			, OnFocus)
	ON_MESSAGE( WM_PAINT			, OnPaint)
	ON_MESSAGE( WM_LBUTTONDOWN		, OnLButtonDown)
	ON_MESSAGE( WM_RBUTTONDOWN		, OnRButtonDown)
	ON_MESSAGE( WM_LBUTTONUP		, OnLButtonUp)
	ON_MESSAGE( WM_MOUSEMOVE		, OnMouseMove)
	ON_MESSAGE( WM_LBUTTONDBLCLK		, OnLButtonDblClk)
	ON_MESSAGE( WM_KEYDOWN			, OnKeyDown)
END_MESSAGE_MAP()


/******************************************************************************/
// �쐬
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnCreate( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// �E�C���h�E�쐬
	HINSTANCE hInst = GetModuleHandle( NULL) ;
	hwndList = CreateWindow( "LISTBOX", "", WS_BORDER | WS_VISIBLE | WS_CHILD, 
		0, 0, 0, 0, 
		hWnd, 0, hInst, 0) ;

	pListWnd = new ListWnd( hWnd) ;
	pListWnd->Init() ;
	hbmpPlaylist = 0 ;

	SetBlockSize( Profile::intBlockX, Profile::intBlockY) ;

	MoveWindow( hWnd, Profile::intX, Profile::intY, intWidth, intHeight, FALSE) ;

	// �喃�쐬
	SetTimer( hWnd, 0, 100, NULL) ;

	return 0 ;
}


/******************************************************************************/
// �^�C�}�[
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnTimer( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateSkin() ;

	// �Đ����H
	if( SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_ISPLAYING) == 1)
	{
		// �f�[�^�擾
		ULONG ulMil   = (ULONG)SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) ;
		ULONG ulPos   = (ULONG)SendMessage( hwndWinamp, WM_WA_IPC, 0, IPC_GETLISTPOS) ;
		char* pszFile = (char*)SendMessage( hwndWinamp, WM_WA_IPC, ulPos, IPC_GETPLAYLISTFILE) ;

		Controller* pController = Controller::GetInstance() ;
		pController->SetMp3Pos( pszFile, ulMil) ;
	}
	return 0;
}


/******************************************************************************/
// �z�b�g�L�[
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnHotKey( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ShowWindow( m_hWnd, IsWindowVisible( m_hWnd) ? SW_HIDE : SW_SHOW) ;
	return 0 ;
}


/******************************************************************************/
// �T�C�Y�ύX
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnSize( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return -1 ;
}


/******************************************************************************/
// �t�H�[�J�X
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnFocus( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return 0 ;
}


/******************************************************************************/
// �`��
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnPaint( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps ;
	HDC hdc = BeginPaint (hWnd, &ps) ;

	// ���������̃r�b�g�}�b�v�ɕ`��
	HBITMAP hbmpBk = CreateCompatibleBitmap( hdc, intWidth, intHeight) ;
	HDC hdcBk = CreateCompatibleDC( hdc);
	SelectObject( hdcBk, hbmpBk);
	DrawSkin( hdcBk) ;	
	pListWnd->DrawList( hdcBk) ;
	DrawTime( hdcBk) ;

	// �\�ɕ`��
	BitBlt( hdc, 0, 0, intWidth, intHeight, hdcBk, 0, 0, SRCCOPY) ;
	DeleteObject( hbmpBk) ;
	DeleteDC( hdcBk) ;

	EndPaint (hWnd, &ps) ;
	return 0 ;
}


/******************************************************************************/
// �j��
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnDestroy( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0) ;
	pListWnd->Destroy() ;
	return 0 ;
}


/******************************************************************************/
// �}�E�X�_�E��
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

		case RESIZE :
			blnResize = TRUE ;
			SetCapture( hWnd) ;
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

		case PREV:
		case NEXT:
			Controller::GetInstance()->Go( pListWnd->GetCurrentItem() + ( item == PREV ? -1 : 1)) ;
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
		}
	return 0 ;
}


/******************************************************************************/
// �E�N���b�N
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnRButtonDown( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	Item item = GetItem( pt) ;

	if( item == LIST)
	{
		pListWnd->OnRButtonDown( wParam, lParam) ;
		return 0 ;
	}

	return -1 ;	
}


/******************************************************************************/
// �}�E�X�A�b�v
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

	return 0 ;
}


/******************************************************************************/
// �}�E�X�h���b�O
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
			InvalidateRect( hWnd, FALSE, TRUE) ;
		}
	}
	else if( blnMove)
	{
		POINT pt ;
		GetCursorPos( &pt) ;
		Profile::intX = pt.x - ptOffsetMove.x ;
		Profile::intY = pt.y - ptOffsetMove.y ;
		SetWindowPos( hWnd, 0, Profile::intX, Profile::intY, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ;
	}
	else  if( blnScroll)
	{
		pListWnd->ScrollTo( HIWORD( lParam) - LIST_TOP) ;
		InvalidateItem( SCROLLBAR) ;
	}
	return 0 ;
}


/******************************************************************************/
// �_�u���N���b�N
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnLButtonDblClk( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { LOWORD( lParam), HIWORD( lParam)} ;
	Item item = GetItem( pt) ;

	if( item == Item::LIST)
	{
		pListWnd->OnLButtonDblClk( wParam, lParam) ;
	}
	return 0 ;
}


/******************************************************************************/
// �L�[�_�E��
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch( wParam)
	{
		case VK_UP:
			pListWnd->ScrollUp() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case VK_DOWN:
			pListWnd->ScrollDown() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case VK_PRIOR:
			pListWnd->PageUp() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case VK_NEXT:
			pListWnd->PageDown() ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
		case VK_RETURN:
			Controller::GetInstance()->Go( pListWnd->GetSelectedItem()) ;
			InvalidateRect( hWnd, NULL, FALSE) ;
			break ;
	}

	return 0 ;
}


/******************************************************************************/
//		�`��
/******************************************************************************/
// ���݂̃X�L���ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�X�L�����ύX����Ă���΁A�X�L�������X�V����B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::UpdateSkin()
{
	char pszBuf[ MAX_PATH + 1] ;
	string s = (char*)SendMessage( hwndWinamp, WM_WA_IPC,(WPARAM)pszBuf, IPC_GETSKIN) ;
	if( s != strSkinName || s == "")
	{
		if( s == "")
		{
			strSkinPath = Profile::strDefaultSkin ;
		}
		else
		{
			strSkinPath = pszBuf ;
		}

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

		// �r�b�g�}�b�v�̃��[�h
		string strBmp ;
		DWORD  dw ;
		strBmp = ( strSkinPath + "\\Pledit.bmp") ;
		dw = GetFileAttributes( strBmp.c_str()) ;
		if( dw != -1 && ( dw & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			hbmpPlaylist = (HBITMAP)LoadImage( NULL, strBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE) ;
		}
		strBmp = ( strSkinPath + "\\text.bmp") ;
		dw = GetFileAttributes( strBmp.c_str()) ;
		if( dw != -1 && ( dw & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			hbmpText = (HBITMAP)LoadImage( NULL, strBmp.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE) ;
		}

		// �e�L�X�g�J���[
		pListWnd->SetSkin( strSkinPath + "\\Pledit.txt") ;

		InvalidateRect( m_hWnd, NULL, TRUE) ;
	}
}


/******************************************************************************/
// �X�L���\��
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::DrawSkin( HDC hdc)
{
	if( !hbmpPlaylist)
	{
		return ;
	}

	HDC hdcBmp = CreateCompatibleDC( hdc);
	SelectObject( hdcBmp, hbmpPlaylist);

	int i ;
	int intCenterX = ( intWidth - 100) / 2 ;

	// �w�b�_
	BitBlt( hdc,   0,   0,  25, 20, hdcBmp,   0, 0, SRCCOPY) ;	// ���[
	BitBlt( hdc,  25,   0,  25, 20, hdcBmp,  127, 0, SRCCOPY) ;
	for( i = 0; i < Profile::intBlockX / 2; i++)
	{
		BitBlt( hdc,  intCenterX - 25 * ( i + 1),   0,  25, 20, hdcBmp, 127, 0, SRCCOPY) ;
	}
	BitBlt( hdc, intCenterX, 0, 100, 20, hdcBmp,  26, 0, SRCCOPY) ;	// �v���C���X�g�̕���
	BitBlt( hdc, intCenterX + 100,   0,  25, 20, hdcBmp,  127, 0, SRCCOPY) ;
	for( i = 0; i < Profile::intBlockX / 2; i++)
	{
		BitBlt( hdc, intWidth - 25 * ( i + 2),   0,  25, 20, hdcBmp, 127, 0, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 25,   0,  25, 20, hdcBmp, 153, 0, SRCCOPY) ;	// �E�[

	// ����
	for( i = 0; i < Profile::intBlockY; i++)
	{
		BitBlt( hdc,   0,  20 + 29 * i,  25, 29, hdcBmp,   0, 42, SRCCOPY) ;
		BitBlt( hdc, intWidth - 25,  20 + 29 * i,  25, 29, hdcBmp,   26, 42, SRCCOPY) ;
	}

	// �t�b�^
	BitBlt( hdc,   0,  intHeight - 38, 125,  38, hdcBmp,   0, 72, SRCCOPY) ;
	for( i = 0; i < ( intWidth - 275) / 25; i++)
	{
		BitBlt( hdc, 125 + i * 25, intHeight - 38, 25, 38, hdcBmp, 179, 0, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 150,  intHeight - 38, 150,  38, hdcBmp, 126, 72, SRCCOPY) ;

	// �X�N���[���o�[
	BitBlt( hdc,  intWidth - 15, 20 + pListWnd->GetScrollBarPos(), 8, 18, hdcBmp, 52 + ( blnScroll ? 9 : 0), 53, SRCCOPY) ;

	DeleteDC( hdcBmp);
}


/******************************************************************************/
// ���Ԃ�\��
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::DrawTime( HDC hdc)
{
	if( intMin == -1 || intSec == -1 || intSec > 60)
	{
		return ;
	}

	HDC hdcBmp = CreateCompatibleDC( hdc);
	SelectObject( hdcBmp, hbmpText);

	if( !Profile::blnCountUp)
	{
		BitBlt( hdc, intWidth - 82     , intHeight - 15, 5, 6, hdcBmp, 15 * 5, 6, SRCCOPY) ;
	}
	BitBlt( hdc, intWidth - 82 +  5, intHeight - 15, 5, 6, hdcBmp, ( intMin / 10) * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 82 + 10, intHeight - 15, 5, 6, hdcBmp, ( intMin % 10) * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 82 + 18, intHeight - 15, 5, 6, hdcBmp, ( intSec / 10) * 5, 6, SRCCOPY) ;
	BitBlt( hdc, intWidth - 82 + 23, intHeight - 15, 5, 6, hdcBmp, ( intSec % 10) * 5, 6, SRCCOPY) ;

	DeleteDC( hdcBmp);
}


/******************************************************************************/
//		�ݒ�
/******************************************************************************/
// �T�C�Y�ύX
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �����ݒ�
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::SetTime( int _intMin, int _intSec)
{
	intMin = _intMin ;
	intSec = _intSec ;
	InvalidateItem( Item::TIME) ;
}


/******************************************************************************/
// �J�����g�A�C�e����ύX
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::SetCurSong( int i)
{
	pListWnd->SetCurrentItem( i) ;
}


/******************************************************************************/
// ���X�g�N���A
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::ClearList()
{
	pListWnd->ClearList() ;
}


/******************************************************************************/
// ���X�g�ǉ�
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::AddList( const string& s)
{
	pListWnd->AddList( s) ;
}


/******************************************************************************/
// ���݂̋Ȃ��擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int MainWnd::GetCurSong() const
{
	return pListWnd->GetCurrentItem() ;
}


/******************************************************************************/
//		���[�e�B���e�B
/******************************************************************************/
// �ꏊ���A�C�e��
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
			return (Item)i;
		}
	}

	return Item::MOVE ;
}


/******************************************************************************/
// �A�C�e�����ĕ`��
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::InvalidateItem( MainWnd::Item item)
{
	if( item == Item::MOVE)
	{
		return ;
	}

	RECT rc ;
	rc.left   = ( intLeftPos[   item] >= 0 ? intLeftPos[   item] : intWidth + intLeftPos[ item]) ;
	rc.right  = ( intRightPos[  item] >= 0 ? intRightPos[  item] : intWidth + intRightPos[ item]) ;
	rc.top    = ( intTopPos[    item] >= 0 ? intTopPos[    item] : intHeight + intTopPos[ item]) ;
	rc.bottom = ( intBottomPos[ item] >= 0 ? intBottomPos[ item] : intHeight + intBottomPos[ item]) ;

	InvalidateRect( m_hWnd, &rc, TRUE) ;
}