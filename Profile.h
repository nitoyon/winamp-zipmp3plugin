
// Profile.h
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F03/05/04(��)
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
	static string strOriginalSkin ;

// �E�C���h�E
	static BOOL blnShowOnlyArchive ;
	static BOOL blnShowOnlyUncompressed;
	static BOOL blnAttachToWinamp;
	static BOOL blnUseTimebar ;

// �t�H���g
	static string	strListFont;
	static int	intListFontSize;
	static string	strCollapseFont;
	static int	intCollapseFontSize;
	static BOOL	blnUseSkinFont;

// �X�L��
	static int	intSkin1;
	static int	intSkin2;
	static string	strSkinDir1;
	static string	strSkinDir2;

	static BOOL blnCountUp ;
	static BOOL blnCompact ;

// MP3
	static BOOL blnUseId3v2;
	static BOOL blnUseCue;
	static BOOL blnUseMp3Cue;

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
	static void	WriteProfileInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const string& s);
	static void	WriteProfileStr(LPTSTR pszSection, LPTSTR pszName, const string&, const string& s) ;
	static void	WriteProfileBln(LPTSTR pszSection, LPTSTR, BOOL, const string&);
	static int	ReadProfileInt(LPTSTR pszSection, LPTSTR pszName, const string& s, int intDefault = 0);
	static string	ReadProfileStr(LPTSTR pszSection, LPTSTR pszName, const string& s, const string& strDefault = "");
	static BOOL	ReadProfileBln(LPTSTR pszSection, LPTSTR pszName, const string& s, BOOL blnDefault = FALSE);
} ;


#endif