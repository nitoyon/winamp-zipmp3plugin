
// Controller.cpp
//============================================================================//
// 更新：02/12/24(火)
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
/*	if( u <= 0 || u >= pZipFile->GetChildFileCount())
	{
		return;
	}

	ulFileNum = u ;
	for( int i = 0; i <= ulFileNum; i++)
	{
		ULONG ul = vecSongHeadTime[ i + 1] ;
		if( vecSongHeadTime[ i + 1] == 0)
		{
			vecSongHeadTime[ i + 1] = vecSongHeadTime[ i] + pZipFile->GetChildFile( i)->GetPlayLength() ;
		}
	}
	ulCurSongLength = vecSongHeadTime[ ulFileNum + 1] - vecSongHeadTime[ ulFileNum] ;
	ulCurTime = vecSongHeadTime[ ulFileNum] ;

	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, ulCurTime, IPC_JUMPTOTIME) ;*/
}


/******************************************************************************/
// 時刻設定
//============================================================================//
// 更新：02/12/23(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s)
	{
		strFilePath = s ;

		// pZipFile の更新
		if( pZipFile)
		{
			delete pZipFile ;
		}
		pZipFile = new ZipFile( strFilePath) ;
		pZipFile->ReadHeader() ;

		// zip ファイルなら
		if( pZipFile->GetChildFileCount() != 0)
		{
			vecSongHeadTime.clear() ;
			vecSongHeadTime.assign( pZipFile->GetChildFileCount() + 1) ;
		
			pMainWnd->ClearList() ;
			vecSongHeadTime[ 0] = 0 ;
			for( int i = 0; i < pZipFile->GetChildFileCount(); i++)
			{
				pMainWnd->AddList( pZipFile->GetFileName( i)) ;
				vecSongHeadTime[ i + 1] = vecSongHeadTime[ i] + pZipFile->GetChildFile( i)->GetPlayLength() ;
			}

			ulFileNum = 0 ;
			ulCurSongLength = pZipFile->GetChildFile( ulFileNum)->GetPlayLength() ;
		}
	}

	if( pZipFile->GetChildFileCount() > 0)
	{
		if( Profile::blnShowOnlyZip)
		{
			pMainWnd->ShowWindow( TRUE) ;
		}
		
		ulCurTime = ulMil ;
		Update() ;

		ULONG u = ( pMainWnd->IsTimeCountup() ? ulCurTime - vecSongHeadTime[ ulFileNum] : vecSongHeadTime[ ulFileNum + 1] - ulCurTime) ;
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

void Controller::Update()
{
	// 次の曲に行ったなら
	if( vecSongHeadTime[ ulFileNum + 1] <= ulCurTime)
	{
		// 更新
		while( TRUE)
		{
			if( ulFileNum + 1 >= pZipFile->GetChildFileCount())
			{
				break ;
			}
			ulFileNum++ ;

			ulCurSongLength = pZipFile->GetChildFile( ulFileNum)->GetPlayLength() ;
			vecSongHeadTime[ ulFileNum + 1] = ulCurSongLength + vecSongHeadTime[ ulFileNum] ;

			if( vecSongHeadTime[ ulFileNum + 1] > ulCurTime)
			{
				pMainWnd->SetCurSong( ulFileNum) ;
				break ;
			}
		}
	}
	// 巻き戻ったとき
	else if( vecSongHeadTime[ ulFileNum] > ulCurTime)
	{
		ULONG ulTmpFileNum = ulFileNum ;
		for( UINT i = 0; i < ulTmpFileNum; i++)
		{
			if( vecSongHeadTime[ i] <= ulCurTime)
			{
				ulFileNum = i ;
			}
		}

		ulCurSongLength = vecSongHeadTime[ ulFileNum + 1] - vecSongHeadTime[ ulFileNum] ;
		pMainWnd->SetCurSong( ulFileNum) ;
	}
}