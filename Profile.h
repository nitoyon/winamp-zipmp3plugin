
// Profile.h
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/26(��)
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
	static string strDefaultSkin ;

// �\��
	static BOOL blnShowOnlyZip ;
	static BOOL blnCountUp ;

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