
// Controller.h
//============================================================================//
// �X�V�F03/05/04(��)
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
	MainWnd* GetWindow() const{ return pMainWnd ;}
	void SetVisiblity( BOOL, BOOL = TRUE) ;
	void ToggleVisiblity() ;

// ����
	void Go( UINT, int intDiff = 0) ;
	void Play() ;
	void SetMp3Pos( const string&, ULONG) ;
	BOOL Extract( UINT, const string&) ;
	BOOL ExtractDetail( UINT, UINT) ;
	void OpenInMiniBrowser( UINT) ;
private:
	void UpdateFileInfo( const string&) ;
	string GetDisplayStr( File*) ;
} ;

#endif