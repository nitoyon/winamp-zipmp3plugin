
// Profile.h
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/30(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class Profile
{
private:
	static string strPath ;

public:
// �Q�Ɨp
	static HINSTANCE hInstance ;
	static string strWinampIniPath ;

// �\��
	static BOOL blnShowOnlyZip ;
	static BOOL blnShowOnlyUncompressedZip ;
	static BOOL blnCountUp ;
	static WORD wrdHotKey ;

// ���X�g
	static string strListNormal ;
	static string strListID3 ;
	static string strListCompilation ;
	static BOOL blnListID3 ;
	static BOOL blnListCompilation ;
	
// �ꏊ
	static int intX ;
	static int intY ;
	static int intBlockX ;
	static int intBlockY ;

// �ǂݏ���
	static void Save() ;
	static void Load() ;

private:
	static void WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui) ;
} ;


#endif