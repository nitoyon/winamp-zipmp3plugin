
// Controller.cpp
//============================================================================//
// 更新：03/04/20(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Controller.h"
#include "ZipFile.h"
#include "MainWnd.h"
#include "Profile.h"
#include "util.h"
#include "resource.h"
#include <wininet.h>
#include <shlobj.h>


/******************************************************************************/
//		定義
/******************************************************************************/

Controller* Controller::pInstance = NULL ;


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller::Controller() 
: strFilePath( ""), strPrevTmpPath( ""), pZipFile( NULL), blnUseHotKey( FALSE)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller::~Controller() 
{
	if( GetFileAttributes( strPrevTmpPath.c_str()) != -1)
	{
		DeleteFile( strPrevTmpPath.c_str()) ;
	}
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
// 表示/非表示切り替え
//============================================================================//
// 更新：02/12/30(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::ToggleVisiblity()
{
	HWND hwnd = pMainWnd->GetHwnd() ;
	if( IsWindowVisible( hwnd))
	{
		ShowWindow( hwnd, SW_HIDE) ;
	}
	else
	{
		ShowWindow( hwnd, SW_SHOW) ;
		SetFocus( hwnd) ;
	}
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
	if( !pZipFile)
	{
		return ;
	}

	// 無圧縮じゃない場合
	if(pZipFile->GetStatus() != ZipFile::Status::UNCOMPRESSED)
	{
		SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, 0, IPC_JUMPTOTIME) ;
		return ;
	}

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
	if( !pZipFile)
	{
		return ;
	}
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
			pMainWnd->SetCurSong( ulCurFileNum, pZipFile->GetChildFile( ulCurFileNum)->GetPlayLength()) ;
		}
	}
}


/******************************************************************************/
// 解凍する
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
//============================================================================//
BOOL Controller::Extract( UINT ui, const string& strPath)
{
	File* pFile = pZipFile->GetChildFile( ui) ;
	if( !pFile)
	{
		return FALSE ;
	}

	if( pZipFile->GetStatus() != ZipFile::Status::UNCOMPRESSED)
	{
		return FALSE ;
	}


	FILE* fExtract = fopen( strPath.c_str(), "wb") ;
	FILE* fZip = fopen( pFile->GetZipPath().c_str(), "rb") ;
	if( fExtract && fZip)
	{
		ULONG ulHead = pFile->GetFileHead() ;
		ZipChildHeader zch = pFile->GetHeader() ;
		ULONG ulFileLength = zch.ulCompressedSize ;

#define BUF_SIZE 4048
		BYTE pbyte[ BUF_SIZE] ;
		if( fseek( fZip, ulHead, SEEK_SET) == 0)
		{
			while( ulFileLength > 0)
			{
				ULONG ulSize = ( BUF_SIZE < ulFileLength ? BUF_SIZE : ulFileLength) ;
				fread( pbyte, sizeof( BYTE), ulSize, fZip) ;
				fwrite( pbyte, sizeof( BYTE), ulSize, fExtract) ;

				ulFileLength -= ulSize ;
			}
		}
	}
	fclose( fZip) ;
	fclose( fExtract) ;

	return TRUE ;
}


/******************************************************************************/
// 解凍詳細
//============================================================================//
// 更新：03/02/02(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Controller::ExtractDetail( UINT ui, UINT uiMsg)
{
	string strOutPath ;

	if( uiMsg == IDM_EXTRACT_HERE)
	{
		strOutPath = GetDirName( pZipFile->GetZipPath()) ;
		strOutPath += pZipFile->GetChildFile( ui)->GetFileName() ;
	}
	else if( uiMsg == IDM_EXTRACT_SELECT)
	{
		OPENFILENAME	ofn ;
		char		pszFileBuf[ MAX_PATH + 1] ;

		ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = pMainWnd->GetHwnd() ;
		ofn.lpstrFilter = "MP3(*.mp3)\0*.mp3\0すべてのファイル\0*.*\0\0";
		ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ;
		ofn.lpstrFile = pszFileBuf;
		ofn.nMaxFile = MAX_PATH ;
		ofn.lpstrTitle = "保存するファイル名を入力してください" ;
		if( !GetSaveFileName( &ofn))
		{
			DWORD d = CommDlgExtendedError();
			return 0 ;
		}
		strOutPath = pszFileBuf ;
	}
	else if( uiMsg == IDM_EXTRACT_DESKTOP)
	{
		char pszBuf[ MAX_PATH] ;
		SHGetSpecialFolderPath( NULL, pszBuf, CSIDL_DESKTOP, 0) ;
		strOutPath = string( pszBuf) + "\\" + pZipFile->GetChildFile( ui)->GetFileName() ;
	}

	return Extract( ui, strOutPath) ;
}


/******************************************************************************/
// ミニブラウザーで開く
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::OpenInMiniBrowser( UINT i)
{
	File* pFile = pZipFile->GetChildFile( i) ;
	if( !pFile)
	{
		return ;
	}

	// ファイル名取得
	string str = "zipmp3tmp" ;
	char pszTmpPath[ MAX_PATH + 1] ;
	GetTempPath( MAX_PATH, pszTmpPath) ;
	char pszPath[ MAX_PATH + 1] ;
	GetTempFileName( pszTmpPath, str.c_str(), 0, pszPath) ;

	// 解凍
	if( Extract( i, pszPath))
	{
		if( GetFileAttributes( strPrevTmpPath.c_str()) != -1)
		{
			DeleteFile( strPrevTmpPath.c_str()) ;
		}
		strPrevTmpPath = pszPath ;

		// 開く
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, (WPARAM)pszPath, IPC_MBOPENREAL) ;
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, (WPARAM)NULL, IPC_MBOPENREAL) ;
	}
}


/******************************************************************************/
// 時刻設定
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s && s != "")
	{
		UpdateFileInfo( s) ;
		SetVisiblity( TRUE) ;
	}

	if( pZipFile->GetStatus() == ZipFile::Status::UNCOMPRESSED)
	{
		// ファイル番号更新
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulMil) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum && ulCurFileNum < pZipFile->GetChildFileCount())
		{
			pMainWnd->SetCurSong( ulCurFileNum, pZipFile->GetChildFile( ulCurFileNum)->GetPlayLength()) ;
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
// 更新：03/04/20(日)
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
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ZipFile::Status::INVALID_HEADER :
			pMainWnd->AddList( "ZIP ファイルのヘッダ情報の読みとりに失敗しました。") ;
			pMainWnd->AddList( "正しい ZIP ファイルではないか、このプラグインがヘボい可能性があります。") ;
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ZipFile::Status::UNCOMPRESSED :
		case ZipFile::Status::COMPRESSED :
		{
			// コンピレーションアルバムかどうかの判断
			BOOL blnCompi = FALSE ;
			if( Profile::blnListCompilation)
			{
				string strArtist = "" ;
				for( int i = 0; i < pZipFile->GetChildFileCount(); i++)
				{
					File* pFile = pZipFile->GetChildFile( i) ;
					if( !pFile->HasID3Tag())
					{
						continue ;
					}

					string s = pFile->GetID3Tag().strArtistName ;
					if( strArtist == "")
					{
						strArtist = s ;
					}
					else if( strArtist != s)
					{
						blnCompi = TRUE ;
						break ;
					}
				}
			}

			// リストに追加していく
			for( int i = 0; i < pZipFile->GetChildFileCount(); i++)
			{
				File* pFile = pZipFile->GetChildFile( i) ;
				
				if( blnCompi && pFile->HasID3Tag())
				{
					pMainWnd->AddList( pFile->GetDisplayStr( Profile::strListCompilation), pFile->GetPlayLength()) ;
				}
				else if( Profile::blnListID3 && pFile->HasID3Tag())
				{
					pMainWnd->AddList( pFile->GetDisplayStr( Profile::strListID3), pFile->GetPlayLength()) ;
				}
				else
				{
					pMainWnd->AddList( pFile->GetFileName(), pFile->GetPlayLength()) ;
				}
			}
			pMainWnd->SetCurSong( 0, i > 0 ? pZipFile->GetChildFile( 0)->GetPlayLength() : 0) ;
			break ;
		}
	}
}
