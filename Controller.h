
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

class ArchiveFile;
class MainWnd;
class InfoWnd;
class File;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class Controller
{
private:
	static Controller* pInstance ;
	MainWnd*	pMainWnd ;
	InfoWnd*	pInfoWnd ;

	string		strFilePath ;
	string		strPrevTmpPath ;
	ArchiveFile*	pArchiveFile ;
	ULONG		ulCurSongLength ;
	ULONG		ulDisplayTime ;
	BOOL		blnUseHotKey ;
	BOOL		blnAlbumSoonEnds;
	int		intCurListPos;

	vector<UINT>	vecRandomSongList;
	UINT		uiLastChangedTickCount;
	BOOL		blnIsProcessingSetMp3Pos;

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
	void GoNext(BOOL) ;
	void Play() ;
	void SetMp3Pos( const string&, ULONG) ;
	BOOL Extract( UINT, const string&) ;
	BOOL ExtractDetail( UINT, UINT) ;
	void OpenInMiniBrowser( UINT) ;
private:
	void UpdateFileInfo( const string&) ;
	string GetDisplayStr( File*) ;

// �����_���Đ��p���X�g
	void CreateRandomSongList();
	UINT GetRandomNextSong();

public:
// ���E�C���h�E
	void DisplayInfoWnd();
} ;

#endif