
// Profile.h
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  REPEAT_NORMAL		0
#define  REPEAT_SONG		1
#define  REPEAT_ALBUM		2
#define  REPEAT_ENDLESSRANDOM	3
#define  REPEAT_RANDOM		4


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
	static string strPluginDir ;

// �Đ����@
	static int	intRepeat;

// �E�C���h�E
	static BOOL	blnShowOnlyArchive ;
	static BOOL	blnShowOnlyUncompressed;
	static BOOL	blnAttachToWinamp;
	static BOOL	blnUseTimebar ;
	static int	intTransparency;

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

// DLL
	static vector<string>	vecHeaderDll;
	static vector<BOOL>	vecUseHeaderDll;

// �ꏊ
	static int intX ;
	static int intY ;
	static int intBlockX ;
	static int intBlockY ;
	static int intInfoX ;
	static int intInfoY ;
	static int intInfoWidth ;
	static int intInfoHeight ;
	static BOOL blnInfoVisible;

// �ǂݏ���
	static void Save() ;
	static void Load() ;
} ;


#endif