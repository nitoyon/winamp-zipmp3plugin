
// Controller.cpp
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Controller.h"
#include "ZipFile.h"
#include "MainWnd.h"
#include "Profile.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/

Controller* Controller::pInstance = NULL ;


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/23(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller::Controller() 
: strFilePath( ""), pZipFile( NULL)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/23(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller::~Controller() 
{
}


/******************************************************************************/
// 唯一のインスタンス取得
//============================================================================//
// 更新：02/12/23(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller* Controller::GetInstance()
{
	if( !pInstance)
	{
		pInstance = new Controller() ;
	}

	return pInstance ;
}


/******************************************************************************/
//		設定
/******************************************************************************/
// Go
//============================================================================//
// 更新：02/12/24(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::Go( UINT u)
{
	if( u < 0 || u >= pZipFile->GetChildFileCount())
	{
		return;
	}

	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, pZipFile->GetSongHead( u), IPC_JUMPTOTIME) ;
}


/******************************************************************************/
// 時刻設定
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s)
	{
		UpdateFileInfo( s) ;

		if( Profile::blnShowOnlyZip)
		{
			pMainWnd->ShowWindow( 
				pZipFile->GetStatus() == ZipFile::Status::UNCOMPRESSED || pZipFile->GetStatus() == ZipFile::Status::COMPRESSED ? 
					SW_SHOW : SW_HIDE) ;
		}
	}

	if( pZipFile->GetStatus() == ZipFile::Status::UNCOMPRESSED)
	{

		ulCurTime = ulMil ;

		// ファイル番号更新
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulCurTime) ;
		if( ulCurFileNum != ulFileNum)
		{
			ulFileNum = ulCurFileNum ;
			pMainWnd->SetCurSong( ulFileNum) ;
		}

		// 表示時刻更新
		ULONG u = pZipFile->GetSongTime( ulCurFileNum, ulCurTime) ;
		u /= 1000 ;
		if( ulDisplayTime != u)
		{
			pMainWnd->SetTime( u / 60, u % 60) ;
			ulDisplayTime = u ;
		}
	}
	else
	{
		if( Profile::blnShowOnlyZip)
		{
			pMainWnd->ShowWindow( FALSE) ;
		}
	}
}


/******************************************************************************/
// ファイル情報の更新
//============================================================================//
// 更新：02/12/23(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;
	ulFileNum = 0 ;
	pMainWnd->SetCurSong( 0) ;

	// pZipFile の更新
	if( pZipFile)
	{
		delete pZipFile ;
	}
	pZipFile = new ZipFile( strFilePath) ;
	pZipFile->ReadHeader() ;

	// zip ファイルなら
	pMainWnd->ClearList() ;
	switch( pZipFile->GetStatus())
	{
		case ZipFile::Status::OPEN_ERROR :
			pMainWnd->AddList( "ファイルを開けませんでした") ;
			break ;

		case ZipFile::Status::NOT_ZIP :
			pMainWnd->AddList( "ZIP ファイルではありません") ;
			break ;

		case ZipFile::Status::INVALID_HEADER :
			pMainWnd->AddList( "ZIP ファイルのヘッダ情報の読みとりに失敗しました。") ;
			pMainWnd->AddList( "正しい ZIP ファイルではないか、このプラグインがヘボい可能性があります。") ;
			break ;

		case ZipFile::Status::UNCOMPRESSED :
		case ZipFile::Status::COMPRESSED :
		{
			for( int i = 0; i < pZipFile->GetChildFileCount(); i++)
			{
				pMainWnd->AddList( pZipFile->GetFileName( i)) ;
			}
			break ;
		}
	}
}