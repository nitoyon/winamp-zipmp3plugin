
// MainWnd.cpp
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "MainWnd.h"
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
, intMin( -1), intSec( -1)
, blnResize( FALSE), blnMove( FALSE), blnClose( FALSE), blnScroll( FALSE)
, intListSelected( 0), intCurrent( 0)
{
	intWidth  = X_BLOCK_CONST + X_BLOCK_SIZE * Profile::intBlockX ;
	intHeight = Y_BLOCK_CONST + Y_BLOCK_SIZE * Profile::intBlockY ;

	intLeftPos[ Item::CLOSE]	= -11 ;		intRightPos[  Item::CLOSE]	= -3 ;
	intTopPos[  Item::CLOSE]	= 3 ;		intBottomPos[ Item::CLOSE]	= 12 ;
	intLeftPos[ Item::COMPACT]	= -20 ;		intRightPos[  Item::COMPACT]	= -12 ;
	intTopPos[  Item::COMPACT]	= 3 ;		intBottomPos[ Item::COMPACT]	= 14 ;
	intLeftPos[ Item::PAGEDOWN]	= -15 ;		intRightPos[  Item::PAGEDOWN]	= -8 ;
	intTopPos[  Item::PAGEDOWN]	= -36 ;		intBottomPos[ Item::PAGEDOWN]	= -32 ;
	intLeftPos[ Item::PAGEUP]	= -15 ;		intRightPos[  Item::PAGEUP]	= -8 ;
	intTopPos[  Item::PAGEUP]	= 3 ;		intBottomPos[ Item::PAGEUP]	= 14 ;
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
	intLeftPos[ Item::LIST]		= LIST_LEFT ;	intRightPos[  Item::LIST]	= -LIST_RIGHT ;
	intTopPos[  Item::LIST]		= LIST_TOP + LIST_MARGIN ;	intBottomPos[ Item::LIST]	= -38 - LIST_MARGIN ;
	intLeftPos[ Item::TIME]		= -89 ;		intRightPos[  Item::TIME]	= -53 ;
	intTopPos[  Item::TIME]		= -17 ;		intBottomPos[ Item::TIME]	= -8 ;
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
// �X�V�F02/12/24(��)
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
	ON_MESSAGE( WM_LBUTTONUP		, OnLButtonUp)
	ON_MESSAGE( WM_MOUSEMOVE		, OnMouseMove)
END_MESSAGE_MAP()


/******************************************************************************/
// �쐬
//============================================================================//
// �X�V�F02/12/24(��)
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

	hFont = CreateFont( 10, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		FIXED_PITCH | FF_MODERN, "MS P�S�V�b�N") ;
	hbmpPlaylist = 0 ;

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
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT MainWnd::OnPaint( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// �X�L���̐ݒ�
	UpdateSkin() ;

	PAINTSTRUCT ps ;
	HDC hdc = BeginPaint (hWnd, &ps) ;

	DrawSkin( hdc) ;	
	DrawList( hdc) ;
	DrawTime( hdc) ;

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
	DeleteObject( hFont) ;
	return 0 ;
}


/******************************************************************************/
// �}�E�X�_�E��
//============================================================================//
// �X�V�F02/12/26(��)
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
			Controller::GetInstance()->Go( intCurrent + ( item == PREV ? -1 : 1)) ;
			break ;

		case PLAY:
			SendMessage( hwndWinamp, WM_COMMAND, WINAMP_BUTTON2, 0) ;
			break ;
		case PAUSE:
			SendMessage( hwndWinamp, WM_COMMAND, WINAMP_BUTTON3, 0) ;
			break ;
		case STOP:
			SendMessage( hwndWinamp, WM_COMMAND, WINAMP_BUTTON4, 0) ;
			break ;
	}
	return 0 ;
}


/******************************************************************************/
// �}�E�X�A�b�v
//============================================================================//
// �X�V�F02/12/23(��)
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

	return 0 ;
}


/******************************************************************************/
// �}�E�X�h���b�O
//============================================================================//
// �X�V�F02/12/24(��)
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
			Profile::intBlockX = intNewBlockX ;
			Profile::intBlockY = intNewBlockY ;
			intWidth  = X_BLOCK_CONST + X_BLOCK_SIZE * Profile::intBlockX ;
			intHeight = Y_BLOCK_CONST + Y_BLOCK_SIZE * Profile::intBlockY ;

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
	return 0 ;
}


/******************************************************************************/
//		�`��
/******************************************************************************/
// ���݂̃X�L���ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/26(��)
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
		string strIni = ( strSkinPath + "\\Pledit.txt") ;
		GetPrivateProfileString( "Text", "Normal", "#ffffff", pszBuf, MAX_PATH, strIni.c_str()) ;
		colNormal = GetColor( pszBuf) ;
		GetPrivateProfileString( "Text", "Current", "#ffffff", pszBuf, MAX_PATH, strIni.c_str()) ;
		colCurrent = GetColor( pszBuf) ;
		GetPrivateProfileString( "Text", "NormalBG", "#ffffff", pszBuf, MAX_PATH, strIni.c_str()) ;
		colNormalBG = GetColor( pszBuf) ;
		GetPrivateProfileString( "Text", "SelectedBG", "#ffffff", pszBuf, MAX_PATH, strIni.c_str()) ;
		colSelectedBG = GetColor( pszBuf) ;

		InvalidateRect( m_hWnd, NULL, TRUE) ;
	}
}


/******************************************************************************/
// �X�L���\��
//============================================================================//
// �X�V�F02/12/24(��)
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

	DeleteDC( hdcBmp);
}


/******************************************************************************/
// ���X�g�\��
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::DrawList( HDC hdc)
{
	// ���X�g
	RECT rc ;
	int i ;
	rc.top		= LIST_TOP ;
	rc.left		= LIST_LEFT + LIST_MARGIN ;
	rc.right	= intWidth - LIST_RIGHT ;

	// �h��Ԃ�
	HBRUSH hBrush = CreateSolidBrush( colNormalBG) ;
	HBRUSH hBrushOld = (HBRUSH)SelectObject( hdc, (HGDIOBJ)hBrush) ;
	HPEN hPen = CreatePen( PS_SOLID, 1, colNormalBG);
	HPEN hPenOld = (HPEN)SelectObject( hdc, (HGDIOBJ)hPen) ;
	Rectangle( hdc, LIST_LEFT, LIST_TOP, intWidth - LIST_RIGHT, LIST_TOP + Profile::intBlockY * 29) ;
	DeleteObject( SelectObject( hdc, hBrushOld)) ;
	DeleteObject( SelectObject( hdc, hPenOld)) ;

	SelectObject( hdc, hFont) ;

	TEXTMETRIC tm ;
	GetTextMetrics( hdc, &tm) ;
	for( i = 0; i < 3 * Profile::intBlockY - 1 ; i++)
	{
		if( i >= vecList.size())
		{
			break ;
		}

		rc.top		= LIST_TOP + i * tm.tmHeight + LIST_MARGIN ;
		rc.bottom	= LIST_TOP + ( i + 1) * tm.tmHeight + LIST_MARGIN ;

		// �w�i�F�ύX
		if( i == intListSelected)
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
		// �����F�ύX
		if( i == intCurrent)
		{
			SetTextColor( hdc, colCurrent) ;
		}
		else
		{
			SetTextColor( hdc, colNormal) ;
		}

		DrawText( hdc, vecList[ i].c_str(), -1, &rc, DT_LEFT | DT_END_ELLIPSIS) ;
	}
}


/******************************************************************************/
// ���Ԃ�\��
//============================================================================//
// �X�V�F02/12/24(��)
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
// ������̐F�𐔒l��
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

COLORREF MainWnd::GetColor( const string& s)
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


/******************************************************************************/
//		�ݒ�
/******************************************************************************/
// �\���؂�ւ�
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::ShowWindow( BOOL b) const
{
	::ShowWindow( m_hWnd, b ? SW_SHOW : SW_HIDE) ;
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
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::SetCurSong( int i)
{
	if( i < 0 || i >= vecList.size())
	{
		return ;
	}

	intCurrent = i ;
	InvalidateItem( Item::LIST) ;
}


/******************************************************************************/
// ���X�g�N���A
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::ClearList()
{
	vecList.clear() ;
	RECT rc = { LIST_LEFT, LIST_TOP, intWidth - LIST_RIGHT, LIST_TOP + Profile::intBlockY * 29} ;
	InvalidateRect( m_hWnd, &rc, TRUE) ;
}


/******************************************************************************/
// ���X�g�ǉ�
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void MainWnd::AddList( const string& s)
{
	vecList.push_back( s) ;

	RECT rc = { LIST_LEFT, LIST_TOP, intWidth - LIST_RIGHT, LIST_TOP + Profile::intBlockY * 29} ;
	InvalidateRect( m_hWnd, &rc, TRUE) ;
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