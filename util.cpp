
// util.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "util.h"


/******************************************************************************/
// ushort
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

USHORT makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG  makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}


/******************************************************************************/
// �E�C���h�E�̃^�C�g�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string GetWindowString( HWND hwnd)
{
	int intSize = GetWindowTextLength( hwnd) ;
	char* pszBuf = new char[ intSize + 1] ;

	GetWindowText( hwnd, pszBuf, intSize + 1) ;
	string s = pszBuf ;
	delete[] pszBuf ;

	return s ;
}


/******************************************************************************/
// �E�C���h�E�̏ꏊ���擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

RECT GetChildRect(HWND hwndParent, HWND hwndChild)
{
	RECT rcChild;
	RECT rect;
	
	GetWindowRect(hwndChild, &rcChild);

	POINT pt = {rcChild.left, rcChild.top};
	ScreenToClient(hwndParent, &pt);

	rect.left	= pt.x;
	rect.top	= pt.y;
	rect.right	= rcChild.right - rcChild.left + pt.x;
	rect.bottom	= rcChild.bottom - rcChild.top + pt.y;

	return rect;
}


/******************************************************************************/
// �t���p�X����f�B���N�g���擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string GetDirName( const string& strPath)
{
	int	intLastYen = 0 ;
	char	pszFile[ MAX_PATH] ;
	char*	pszPointer = pszFile ;
	strcpy( pszFile, strPath.c_str()) ;

	for( int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if( IsDBCSLeadByte( *pszPointer))
		{
			// �Q�o�C�g�����Ȃ�Q�i��
			i++ ;
			continue ;
		}

		if( *pszPointer == '\\' || *pszPointer == '/')
		{
			intLastYen = i ;
		}
	}

	if( intLastYen > 0)
	{
		return strPath.substr( 0, intLastYen + 1) ;	// Yen ���܂߂ĕԂ�
	}
	else
	{
		return "" ;
	}
}


/******************************************************************************/
// �t���p�X����t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string GetFileName( const string& strPath)
{
	string strDirName = GetDirName( strPath) ;
	return strPath.substr( strDirName.size()) ;
}


/******************************************************************************/
// NT �n���ǂ������擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL IsNT()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/******************************************************************************/
// ���W���[�o�[�W�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DWORD GetOsMajorVersion()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwOSVersionInfoSize;
}


/******************************************************************************/
// �E�C���h�E�𒆐S�ɕ\��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

VOID CenterWindow(HWND hwnd)
{
	RECT    rect;
	LONG    dx, dy;
	LONG    dxParent, dyParent;
	LONG    Style;

	// Get window rect
	GetWindowRect(hwnd, &rect);

	dx = rect.right  - rect.left;
	dy = rect.bottom - rect.top;

	// Get parent rect
	Style = GetWindowLong(hwnd, GWL_STYLE);
	if ((Style & WS_CHILD) == 0)
	{
		// Return the desktop windows size (size of main screen)
		dxParent = GetSystemMetrics(SM_CXSCREEN);
		dyParent = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		HWND    hwndParent;
		RECT    rectParent;

		hwndParent = GetParent(hwnd);
		if(hwndParent == NULL)
		{
			hwndParent = GetDesktopWindow();
		}

		GetWindowRect(hwndParent, &rectParent);

		dxParent = rectParent.right - rectParent.left;
		dyParent = rectParent.bottom - rectParent.top;
	}

	// Centre the child in the parent
	rect.left = (dxParent - dx) / 2;
	rect.top  = (dyParent - dy) / 3;

	// Move the child into position
	SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);

	SetForegroundWindow(hwnd);
}
