
// Controller.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Controller.h"
#include "ArchiveFile.h"
#include "MainWnd.h"
#include "InfoWnd.h"
#include "Profile.h"
#include "util\uPath.h"
#include "resource.h"
#include "util\uDebug.h"
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
// 概要：なし。
// 補足：なし。
//============================================================================//

Controller::Controller() 
: pMainWnd(NULL), pInfoWnd(NULL)
, strFilePath( ""), strPrevTmpPath( ""), pArchiveFile( NULL), blnUseHotKey( FALSE)
, blnAlbumSoonEnds(FALSE), uiLastChangedTickCount(0)
, blnIsProcessingSetMp3Pos(FALSE)
{
	pInfoWnd = new InfoWnd();
}


/******************************************************************************/
// デストラクタ
//============================================================================//
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
	switch( pArchiveFile->GetStatus())
	{
		case ArchiveFile::Status::UNCOMPRESSED :
			b = TRUE ;
			break ;
		case ArchiveFile::Status::COMPRESSED :
			b = ( Profile::blnShowOnlyArchive && !Profile::blnShowOnlyUncompressed)
			 || ( !Profile::blnShowOnlyArchive) ;
			break ;
		default :
			b = !Profile::blnShowOnlyArchive ;
			break ;
	}
	ShowWindow( pMainWnd->GetHwnd(), b ? SW_SHOW : SW_HIDE) ;
}


/******************************************************************************/
// 表示/非表示切り替え
//============================================================================//
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
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::Play()
{
	if( !pArchiveFile)
	{
		return ;
	}

	// 無圧縮じゃない場合
	if(pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, 0, IPC_JUMPTOTIME) ;
		return ;
	}

	ULONG ulMilisec = pArchiveFile->GetSongHead( pMainWnd->GetCurSong()) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
}


/******************************************************************************/
// 移動
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::Go( UINT u, int intDiff)
{
	if( !pArchiveFile)
	{
		return ;
	}
	if( u < 0 || u >= pArchiveFile->GetChildFileCount())
	{
		return;
	}

	// ジャンプ
	HWND hwnd = pMainWnd->GetWinampWindow() ;
	ULONG ulMilisec = pArchiveFile->GetSongHead( u) + intDiff;
	SendMessage( hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;

	// 曲番号変更
	pMainWnd->SetCurSong(u, pArchiveFile->GetChildFile(u)->GetPlayLength()) ;

	// 再生中でないならば、手動でファイル番号更新
	if( SendMessage( hwnd, WM_WA_IPC, 0, IPC_ISPLAYING) != 1)
	{
		ULONG ulCurFileNum = pArchiveFile->GetSongIndex( ulMilisec) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum)
		{
			SendMessage(hwnd, WM_COMMAND, WINAMP_BUTTON2, 0) ;
			SendMessage(hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
		}
	}
}


/******************************************************************************/
// 次の曲へ行く
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::GoNext(BOOL blnNext)
{
	if(Profile::intRepeat == REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM)
	{
		// ランダム再生の場合

		// 前の曲には移動できない
		if(!blnNext)
		{
			return;
		}

		UINT uiNextSong = GetRandomNextSong();
		if(uiNextSong != -1)
		{
			// ランダム再生で次の曲がある場合
			Go(uiNextSong);
		}
		else
		{
			// そうでない場合は、次の曲へ
			SendMessage(pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON5, 0);
		}
	}
	else
	{
		// ランダム再生でない場合
		Go(pMainWnd->GetCurSong() + (blnNext ? 1 : -1)) ;
	}
}


/******************************************************************************/
// 解凍する
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//
BOOL Controller::Extract( UINT ui, const string& strPath)
{
	File* pFile = pArchiveFile->GetChildFile( ui) ;
	if( !pFile)
	{
		return FALSE ;
	}

	if( pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		return FALSE ;
	}


	FILE* fExtract = fopen( strPath.c_str(), "wb") ;
	FILE* fZip = fopen( pFile->GetArchivePath().c_str(), "rb") ;
	if( fExtract && fZip)
	{
		ULONG ulHead = pFile->GetStartPoint() ;
		ULONG ulFileLength = pFile->GetEndPoint() - ulHead;

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
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Controller::ExtractDetail( UINT ui, UINT uiMsg)
{
	if( pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		return FALSE ;
	}

	string strOutPath ;

	if( uiMsg == IDM_EXTRACT_HERE)
	{
		strOutPath = GetDirName(pArchiveFile->GetChildFile(ui)->GetArchivePath()) ;
		strOutPath += pArchiveFile->GetChildFile(ui)->GetFileName() ;
	}
	else if( uiMsg == IDM_EXTRACT_SELECT)
	{
		OPENFILENAME	ofn ;
		char		pszFileBuf[ MAX_PATH + 1];
		strncpy(pszFileBuf,  pArchiveFile->GetChildFile(ui)->GetFileName().c_str(), MAX_PATH + 1);

		ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = pMainWnd->GetHwnd() ;
		ofn.lpstrFilter = "すべてのファイル\0*.*\0MP3(*.mp3)\0*.mp3\0\0";
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
		strOutPath = string( pszBuf) + "\\" + pArchiveFile->GetChildFile( ui)->GetFileName() ;
	}

	return Extract( ui, strOutPath) ;
}


/******************************************************************************/
// ミニブラウザーで開く
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::OpenInMiniBrowser( UINT i)
{
	if( pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		return ;
	}

	File* pFile = pArchiveFile->GetChildFile( i) ;
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
	if(Extract( i, pszPath))
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
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if(blnIsProcessingSetMp3Pos) return;
	blnIsProcessingSetMp3Pos = TRUE;
	ULONG ulCurFileNum;

	// デバッグ用の出力
	TRACE1("%2d\n", ulMil);

	if(strFilePath != s && s != "")
	{
		// ファイル名が変わったとき
		if((Profile::intRepeat == REPEAT_SONG || Profile::intRepeat == REPEAT_ALBUM) 
		&& blnAlbumSoonEnds)
		{
			blnAlbumSoonEnds = FALSE;

			// 単曲リピートやアルバム内リピートの場合。
			// ストップして再生する
			// 単曲リピートの場合は最後の曲、アルバム内リピートの場合は先頭の曲に移動することになる
			HWND hwndWinamp = pMainWnd->GetWinampWindow();
			SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON4, 0);
			SendMessage(hwndWinamp, WM_WA_IPC, intCurListPos, IPC_SETPLAYLISTPOS);
			Go(pMainWnd->GetCurSong());
			SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON2, 0);
			goto cleanup;
		}
		else if((Profile::intRepeat == REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM)
		      && blnAlbumSoonEnds)
		{
			blnAlbumSoonEnds = FALSE;

			// ランダム再生の場合
			UINT uiNextSong = GetRandomNextSong();
			HWND hwndWinamp = pMainWnd->GetWinampWindow();
			if(uiNextSong != -1)
			{
				SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON4, 0);
				SendMessage(hwndWinamp, WM_WA_IPC, intCurListPos, IPC_SETPLAYLISTPOS);
				uiLastChangedTickCount = GetTickCount();
				Go(uiNextSong);
				TRACE1("album random goto : %2d\n", uiNextSong);
				SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON2, 0);
				SendMessage(hwndWinamp, WM_WA_IPC, pArchiveFile->GetSongHead(uiNextSong), IPC_JUMPTOTIME) ;
				goto cleanup;
			}

			// 次の曲がない場合は、そのまま曲情報更新する
		}

		// そのまま曲情報更新
		UpdateFileInfo(s) ;
		SetVisiblity( TRUE) ;

		if(pArchiveFile->GetStatus() == ArchiveFile::Status::UNCOMPRESSED 
		&& (Profile::intRepeat == REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM))
		{
			// ランダム再生の場合
			UINT uiNextSong = GetRandomNextSong();
			HWND hwndWinamp = pMainWnd->GetWinampWindow();
			if(uiNextSong == -1)
			{
				// 次の曲がない場合は、プレイリストの次の曲へ
				SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON5, 0);
				goto cleanup;
			}

			Go(uiNextSong);
			goto cleanup;
		}
	}

	// 無圧縮の場合
	if(pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		// 無圧縮以外の場合
		if(Profile::blnShowOnlyArchive)
		{
			SetVisiblity( FALSE) ;
		}
		goto cleanup;
	}

	// 単曲リピートの準備
	if(Profile::intRepeat != 0)
	{
		HWND hwndWinamp = pMainWnd->GetWinampWindow();
		ULONG ulAlbumLength = SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
		blnAlbumSoonEnds = (ulAlbumLength * 1000 - ulMil < 30 * 1000);	// アルバムがもうすぐ終わるかどうかを確認

		if(blnAlbumSoonEnds)
		{
			intCurListPos = SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETLISTPOS);
		}
	}

	// ファイル番号更新
	ulCurFileNum = pArchiveFile->GetSongIndex(ulMil) ;
	if(pMainWnd->GetCurSong() != ulCurFileNum && ulCurFileNum < pArchiveFile->GetChildFileCount())
	{
		// 曲が変わったとき
		TRACE2("album song changed : %d -> %d\n", pMainWnd->GetCurSong(), ulCurFileNum);
		if(Profile::intRepeat == REPEAT_SONG)
		{
			// 単曲リピート
			// pMainWnd の曲番号に飛ぶ。１つ前のはず。
			Go(pMainWnd->GetCurSong());
		}
		else if(Profile::intRepeat== REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM)
		{
			// ランダム再生中
			// 再生の位置変更までにかかる誤差を飲み込むため
			UINT uiCurTick = GetTickCount();
			if(uiCurTick - uiLastChangedTickCount > 1000) // TickCount が一周するのはまぁ無視。49.7日連続起動に一回だし、大丈夫でしょう
			{
				// ランダム再生中に次の曲へ移った
				UINT uiNextSong = GetRandomNextSong();
				HWND hwndWinamp = pMainWnd->GetWinampWindow();
				if(uiNextSong == -1)
				{
					// 次の曲がない場合は、プレイリストの次の曲へ
					SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON5, 0);
					goto cleanup;
				}

				Go(uiNextSong);
			}
		}
		else
		{
			// 普通に再生中
			pMainWnd->SetCurSong(ulCurFileNum, pArchiveFile->GetChildFile(ulCurFileNum)->GetPlayLength()) ;
		}
	}

	// 表示時刻更新
	{
		ULONG u = pArchiveFile->GetSongTime(ulCurFileNum, ulMil) ;
		u /= 1000 ;
		if(ulDisplayTime != u)
		{
			pMainWnd->SetTime(u / 60, u % 60) ;
			ulDisplayTime = u ;
		}
	}

cleanup:
	blnIsProcessingSetMp3Pos = FALSE;
}


/******************************************************************************/
// ファイル情報の更新
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;

	// pArchiveFile の更新
	if( pArchiveFile)
	{
		pMainWnd->ClearList() ;
		delete pArchiveFile ;
	}
	pArchiveFile = new ArchiveFile(strFilePath) ;
	pArchiveFile->ReadHeader() ;

	// zip ファイルなら
	switch( pArchiveFile->GetStatus())
	{
		case ArchiveFile::Status::OPEN_ERROR :
			pMainWnd->AddList( "ファイルを開けませんでした") ;
			break ;

		case ArchiveFile::Status::NO_HEADER:
			pMainWnd->AddList( "ヘッダ情報を読み取れませんでした") ;
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ArchiveFile::Status::INVALID_HEADER :
			pMainWnd->AddList( "ヘッダ情報の読みとりに失敗しました。") ;
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ArchiveFile::Status::UNCOMPRESSED :
		case ArchiveFile::Status::COMPRESSED :
		{
			// ランダム再生用のリスト計算
			CreateRandomSongList();

			// コンピレーションアルバムかどうかの判断
			BOOL blnCompi = FALSE ;
			if( Profile::blnListCompilation)
			{
				string strArtist = "" ;
				for( int i = 0; i < pArchiveFile->GetChildFileCount(); i++)
				{
					File* pFile = pArchiveFile->GetChildFile( i) ;
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
			for( int i = 0; i < pArchiveFile->GetChildFileCount(); i++)
			{
				File* pFile = pArchiveFile->GetChildFile( i) ;
				
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
			pMainWnd->SetCurSong( 0, i > 0 ? pArchiveFile->GetChildFile( 0)->GetPlayLength() : 0) ;
			break ;
		}
	}

	// ファイル情報ウインドウを更新
	if(pInfoWnd)	pInfoWnd->SetArchiveFile(pArchiveFile);
}


/******************************************************************************/
//		ランダム再生用リスト
/******************************************************************************/
// ランダム再生用のリスト作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::CreateRandomSongList()
{
	vecRandomSongList.clear();

	for(int i = 0; i < pArchiveFile->GetChildFileCount(); i++)
	{
		// 曲の長さがある場合のみランダム再生用のリストに追加する
		File* pFile = pArchiveFile->GetChildFile(i);
		if(pFile && pFile->GetPlayLength() != 0)
		{
			vecRandomSongList.push_back(i);
		}
	}
}


/******************************************************************************/
// ランダム再生のときに次の曲を取得する
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT Controller::GetRandomNextSong()
{
	// リストが空の時
	if(vecRandomSongList.size() == 0)
	{
		if(Profile::intRepeat == REPEAT_ENDLESSRANDOM)
		{
			// 永久ランダムの場合は、リストの作り直し
			CreateRandomSongList();
		}
		else
		{
			// 一巡で次への場合は、-1 を返す
			return -1;
		}
	}

	// リストの中から１つ選択
	int intIndex = ((double)rand() / (double)RAND_MAX) * (double)vecRandomSongList.size();
	int intRet = vecRandomSongList[intIndex];
	vecRandomSongList.erase(vecRandomSongList.begin() + intIndex);
	uiLastChangedTickCount = GetTickCount();
	return intRet;
}


/******************************************************************************/
//		情報ウインドウ
/******************************************************************************/
// 情報ウインドウを表示/更新
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::DisplayInfoWnd()
{
	if(pInfoWnd)
	{
		pInfoWnd->Create();
		pInfoWnd->SetArchiveFile(pArchiveFile);
	}
}


/******************************************************************************/
// 情報ウインドウを非表示にする
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Controller::CloseInfoWnd()
{
	if(pInfoWnd)
	{
		pInfoWnd->Close();
	}
}