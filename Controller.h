
// Controller.h
//============================================================================//
// 更新：03/05/04(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __CONTROLLER_H__
#define  __CONTROLLER_H__

#include "include.h"


/******************************************************************************/
//		定義
/******************************************************************************/

class ArchiveFile;
class MainWnd;
class InfoWnd;
class File;


/******************************************************************************/
//		クラス定義
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

// コンストラクタおよびデストラクタ
private:
	Controller() ;
public:
	~Controller() ;
	static Controller* GetInstance() ;

// ウインドウ
	void SetWindow( MainWnd* p){ pMainWnd = p ;} ;
	MainWnd* GetWindow() const{ return pMainWnd ;}
	void SetVisiblity( BOOL, BOOL = TRUE) ;
	void ToggleVisiblity() ;

// 動作
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

// ランダム再生用リスト
	void CreateRandomSongList();
	UINT GetRandomNextSong();

public:
// 情報ウインドウ
	void DisplayInfoWnd();
} ;

#endif