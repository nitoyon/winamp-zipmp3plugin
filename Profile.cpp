
// Profile.cpp
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

string	Profile::strPath = "" ;

// �Q�Ɨp
HINSTANCE	Profile::hInstance = 0 ;
string		Profile::strDefaultSkin = "" ;

// �E�C���h�E
BOOL	Profile::blnShowOnlyZip = FALSE ;
BOOL	Profile::blnShowOnlyUncompressedZip = FALSE ;
BOOL	Profile::blnCountUp = FALSE ;
WORD	Profile::wrdHotKey = 0 ;

// ���X�g
string	Profile::strListNormal ;
string	Profile::strListID3 ;
string	Profile::strListCompilation ;
BOOL	Profile::blnListID3 ;
BOOL	Profile::blnListCompilation ;


// �ꏊ
int	Profile::intX = 0 ;
int	Profile::intY = 0 ;
int	Profile::intBlockX = 0 ;
int	Profile::intBlockY = 0 ;


/******************************************************************************/
//		�ۑ��E�ǂ݂Ƃ�
/******************************************************************************/
// �ۑ�
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Save()
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH + 1] ;
		GetModuleFileName( hInstance, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		strDefaultSkin = pszPath ;
		strDefaultSkin = strDefaultSkin.substr( 0, strDefaultSkin.rfind( '\\')) + "\\gen_zipalbum" ;
	}
	const char* pszFile = strPath.c_str() ;

	// �E�C���h�E
	WritePrivateProfileString( "Display", "ShowOnlyZip", blnShowOnlyZip ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Display", "ShowOnlyUncompressedZip", blnShowOnlyUncompressedZip ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Display", "CountUp", blnCountUp ? "yes" : "no", pszFile) ;
	WriteProfile( pszFile, "Display", "DispHotKey", wrdHotKey) ;

	// ���X�g
	WritePrivateProfileString( "List", "Normal", strListNormal.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "ID3", strListID3.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "Compilation", strListCompilation.c_str(), pszFile) ;
	WritePrivateProfileString( "List", "useID3", blnListID3 ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "List", "useCompilation", blnListCompilation ? "yes" : "no", pszFile) ;

	// �ꏊ
	WriteProfile( pszFile, "pos", "x", intX) ;
	WriteProfile( pszFile, "pos", "y", intY) ;
	WriteProfile( pszFile, "pos", "width", intBlockX) ;
	WriteProfile( pszFile, "pos", "height", intBlockY) ;
}


/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Load() 
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH] ;
		GetModuleFileName( hInstance, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;

		strDefaultSkin = pszPath ;
		strDefaultSkin = strDefaultSkin.substr( 0, strDefaultSkin.rfind( '\\')) + "\\gen_zipalbum" ;
	}
	const char* pszFile = strPath.c_str() ;
	char	pszBuf[ MAX_PATH] ;

	// �E�C���h�E
	GetPrivateProfileString( "Display", "ShowOnlyZip", "no", pszBuf, MAX_PATH, pszFile) ;
	blnShowOnlyZip = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "Display", "ShowOnlyUncompressedZip", "no", pszBuf, MAX_PATH, pszFile) ;
	blnShowOnlyUncompressedZip = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "Display", "CountUp", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnCountUp = ( stricmp( pszBuf, "yes") == 0) ;
	wrdHotKey = GetPrivateProfileInt( "Display", "DispHotKey", 0, pszFile) ;

	// ���X�g
	GetPrivateProfileString( "List", "Normal", "%FILE_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListNormal = pszBuf ;
	GetPrivateProfileString( "List", "ID3", "%TRACK_NUMBER2%. %TRACK_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListID3 = pszBuf ;
	GetPrivateProfileString( "List", "Compilation", "%TRACK_NUMBER2%. (%ARTIST_NAME%)%TRACK_NAME%", pszBuf, MAX_PATH, pszFile) ;
	strListCompilation = pszBuf ;
	GetPrivateProfileString( "List", "useID3", "no", pszBuf, MAX_PATH, pszFile) ;
	blnListID3 = ( stricmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "List", "useCompilation", "no", pszBuf, MAX_PATH, pszFile) ;
	blnListCompilation = ( stricmp( pszBuf, "yes") == 0) ;

	// �ꏊ
	intX = GetPrivateProfileInt( "pos", "x", 50, pszFile) ;
	intY = GetPrivateProfileInt( "pos", "y", 30, pszFile) ;
	intBlockX = GetPrivateProfileInt( "pos", "width", 5, pszFile) ;
	intBlockY = GetPrivateProfileInt( "pos", "height", 3, pszFile) ;
}


/******************************************************************************/
//		���[�e�B���e�B�[�֐�
/******************************************************************************/
// ��������������
//============================================================================//
// �X�V�F02/10/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui)
{
	char	pszBuf[ 256] ;
	wsprintf( pszBuf, "%u", ui) ;
	WritePrivateProfileString( pszSection, pszName, pszBuf, s.c_str()) ;
}