
// Controller.h
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __CONTROLLER_H__
#define  __CONTROLLER_H__

#include "include.h"


/******************************************************************************/
//		��`
/******************************************************************************/

class ZipFile ;
class MainWnd ;
class File ;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class Controller
{
private:
	static Controller* pInstance ;
	MainWnd*	pMainWnd ;

	string		strFilePath ;
	string		strPrevTmpPath ;
	ZipFile*	pZipFile ;
	ULONG		ulCurSongLength ;
	vector<ULONG>	vecSongHeadTime ;
	ULONG		ulDisplayTime ;
	BOOL		blnUseHotKey ;

// �R���X�g���N�^����уf�X�g���N�^
private:
	Controller() ;
public:
	~Controller() ;
	static Controller* GetInstance() ;

// �E�C���h�E
	void SetWindow( MainWnd* p){ pMainWnd = p ;} ;
	void SetVisiblity( BOOL, BOOL = TRUE) ;
	void ToggleVisiblity() ;

// ����
	void Go( UINT) ;
	void Play() ;
	void SetMp3Pos( const string&, ULONG) ;
	BOOL Extract( UINT, const string&) ;
	void OpenInMiniBrowser( UINT) ;
private:
	void UpdateFileInfo( const string&) ;
	string GetDisplayStr( File*) ;
} ;

#endif