
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

class ZipFile ;
class MainWnd ;
class File ;


/******************************************************************************/
//		クラス定義
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