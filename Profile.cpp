
// Profile.cpp
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/24(��)
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

// ���X�g
BOOL	Profile::blnShowOnlyZip = FALSE ;
BOOL	Profile::blnCountUp = FALSE ;

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
// �X�V�F02/12/24(��)
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

	// ���X�g
	WritePrivateProfileString( "Display", "ShowOnlyZip", blnShowOnlyZip ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Display", "CountUp", blnCountUp ? "yes" : "no", pszFile) ;

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

	// �\��
	GetPrivateProfileString( "Display", "ShowOnlyZip", "no", pszBuf, MAX_PATH, pszFile) ;
	blnShowOnlyZip = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "Display", "CountUp", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnCountUp = ( strcmp( pszBuf, "yes") == 0) ;

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