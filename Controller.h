
// Controller.h
//============================================================================//
// 更新：02/12/26(木)
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


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Controller
{
private:
	static Controller* pInstance ;
	MainWnd*	pMainWnd ;

	string		strFilePath ;
	ZipFile*	pZipFile ;
	ULONG		ulCurTime ;
	ULONG		ulFileNum ;
	ULONG		ulCurSongLength ;
	vector<ULONG>	vecSongHeadTime ;
	ULONG		ulDisplayTime ;

// コンストラクタおよびデストラクタ
private:
	Controller() ;
public:
	~Controller() ;
	static Controller* GetInstance() ;

// ウインドウ
	void SetWindow( MainWnd* p){ pMainWnd = p ;} ;

// 設定
	void Go( UINT) ;
	void SetMp3Pos( const string&, ULONG) ;
private:
	void UpdateFileInfo( const string&) ;
} ;

#endif