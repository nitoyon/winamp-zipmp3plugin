
// Controller.cpp
//============================================================================//
// 更新：02/12/28(土)
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
: strFilePath( ""), pZipFile( NULL), blnUseHotKey( FALSE)
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
//		ウインドウ
/******************************************************************************/
// ウインドウ表示の切り替え
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetVisiblity( BOOL blnShow, BOOL blnForce)
{
	if( blnShow == FALSE)
	{
		ShowWindow( pMainWnd->GetHwnd(), SW_HIDE) ;
		return ;
	}

	if( blnForce)
	{
		ShowWindow( pMainWnd->GetHwnd(), SW_SHOW) ;
		return ;
	}

	BOOL b = TRUE ;
	switch( pZipFile->GetStatus())
	{
		case ZipFile::Status::UNCOMPRESSED :
			b = TRUE ;
			break ;
		case ZipFile::Status::COMPRESSED :
			b = ( Profile::blnShowOnlyZip && !Profile::blnShowOnlyUncompressedZip)
			 || ( !Profile::blnShowOnlyZip) ;
			break ;
		default :
			b = !Profile::blnShowOnlyZip ;
			break ;
	}
	ShowWindow( pMainWnd->GetHwnd(), b ? SW_SHOW : SW_HIDE) ;
}


/******************************************************************************/
// ホットキーの登録
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT Controller::SetHotKey( WORD wHotKey)
{
	if( blnUseHotKey)
	{
		UnregisterHotKey( pMainWnd->GetHwnd(), HOTKEY_SHOW) ;
	}

	// 形式変換
	UINT uModifiers = (UINT)HIBYTE(wHotKey);
	UINT uVirtual   = (UINT)LOBYTE(wHotKey);
	UINT uMod = 0 ;
	if( uModifiers & HOTKEYF_ALT)		uMod |= MOD_ALT ;
	if( uModifiers & HOTKEYF_CONTROL)	uMod |= MOD_CONTROL ;
	if( uModifiers & HOTKEYF_SHIFT)		uMod |= MOD_SHIFT ;

	HWND h = pMainWnd->GetHwnd() ;
	blnUseHotKey = RegisterHotKey( pMainWnd->GetHwnd(), HOTKEY_SHOW, uMod, uVirtual) ;
	return blnUseHotKey ;
}


/******************************************************************************/
//		設定
/******************************************************************************/
// 再生
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::Play()
{
	ULONG ulMilisec = pZipFile->GetSongHead( pMainWnd->GetCurSong()) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
}


/******************************************************************************/
// 移動
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

	HWND hwnd = pMainWnd->GetWinampWindow() ;
	ULONG ulMilisec = pZipFile->GetSongHead( u) ;
	SendMessage( hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;

	// 再生中でないならば、手動でファイル番号更新
	if( SendMessage( hwnd, WM_WA_IPC, 0, IPC_ISPLAYING) != 1)
	{
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulMilisec) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum)
		{
			pMainWnd->SetCurSong( ulCurFileNum) ;
		}
	}
}


/******************************************************************************/
// 時刻設定
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s)
	{
		UpdateFileInfo( s) ;
		SetVisiblity( TRUE) ;
	}

	if( pZipFile->GetStatus() == ZipFile::Status::UNCOMPRESSED)
	{

		ulMil ;

		// ファイル番号更新
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulMil) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum)
		{
			pMainWnd->SetCurSong( ulCurFileNum) ;
		}

		// 表示時刻更新
		ULONG u = pZipFile->GetSongTime( ulCurFileNum, ulMil) ;
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
			SetVisiblity( FALSE) ;
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

	// pZipFile の更新
	if( pZipFile)
	{
		pMainWnd->ClearList() ;
		delete pZipFile ;
	}
	pZipFile = new ZipFile( strFilePath) ;
	pZipFile->ReadHeader() ;

	// zip ファイルなら
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
				File* pFile = pZipFile->GetChildFile( i) ;
				pMainWnd->AddList( pFile->GetDisplayStr( 
					pFile->HasID3Tag() &&  Profile::blnListID3 ? Profile::strListID3 : Profile::strListNormal
				)) ;
			}
			break ;
		}
	}
}
