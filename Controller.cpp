
// Controller.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
//		��`
/******************************************************************************/

Controller* Controller::pInstance = NULL ;


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// 
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Controller::~Controller() 
{
	if( GetFileAttributes( strPrevTmpPath.c_str()) != -1)
	{
		DeleteFile( strPrevTmpPath.c_str()) ;
	}
}


/******************************************************************************/
// �B��̃C���X�^���X�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
//		�E�C���h�E
/******************************************************************************/
// �E�C���h�E�\���̐؂�ւ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �\��/��\���؂�ւ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
//		�ݒ�
/******************************************************************************/
// �Đ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::Play()
{
	if( !pArchiveFile)
	{
		return ;
	}

	// �����k����Ȃ��ꍇ
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
// �ړ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

	// �W�����v
	HWND hwnd = pMainWnd->GetWinampWindow() ;
	ULONG ulMilisec = pArchiveFile->GetSongHead( u) + intDiff;
	SendMessage( hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;

	// �Ȕԍ��ύX
	pMainWnd->SetCurSong(u, pArchiveFile->GetChildFile(u)->GetPlayLength()) ;

	// �Đ����łȂ��Ȃ�΁A�蓮�Ńt�@�C���ԍ��X�V
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
// ���̋Ȃ֍s��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::GoNext(BOOL blnNext)
{
	if(Profile::intRepeat == REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM)
	{
		// �����_���Đ��̏ꍇ

		// �O�̋Ȃɂ͈ړ��ł��Ȃ�
		if(!blnNext)
		{
			return;
		}

		UINT uiNextSong = GetRandomNextSong();
		if(uiNextSong != -1)
		{
			// �����_���Đ��Ŏ��̋Ȃ�����ꍇ
			Go(uiNextSong);
		}
		else
		{
			// �����łȂ��ꍇ�́A���̋Ȃ�
			SendMessage(pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON5, 0);
		}
	}
	else
	{
		// �����_���Đ��łȂ��ꍇ
		Go(pMainWnd->GetCurSong() + (blnNext ? 1 : -1)) ;
	}
}


/******************************************************************************/
// �𓀂���
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �𓀏ڍ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
		ofn.lpstrFilter = "���ׂẴt�@�C��\0*.*\0MP3(*.mp3)\0*.mp3\0\0";
		ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ;
		ofn.lpstrFile = pszFileBuf;
		ofn.nMaxFile = MAX_PATH ;
		ofn.lpstrTitle = "�ۑ�����t�@�C��������͂��Ă�������" ;
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
// �~�j�u���E�U�[�ŊJ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

	// �t�@�C�����擾
	string str = "zipmp3tmp" ;
	char pszTmpPath[ MAX_PATH + 1] ;
	GetTempPath( MAX_PATH, pszTmpPath) ;
	char pszPath[ MAX_PATH + 1] ;
	GetTempFileName( pszTmpPath, str.c_str(), 0, pszPath) ;

	// ��
	if(Extract( i, pszPath))
	{
		if( GetFileAttributes( strPrevTmpPath.c_str()) != -1)
		{
			DeleteFile( strPrevTmpPath.c_str()) ;
		}
		strPrevTmpPath = pszPath ;

		// �J��
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, (WPARAM)pszPath, IPC_MBOPENREAL) ;
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, (WPARAM)NULL, IPC_MBOPENREAL) ;
	}
}


/******************************************************************************/
// �����ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if(blnIsProcessingSetMp3Pos) return;
	blnIsProcessingSetMp3Pos = TRUE;
	ULONG ulCurFileNum;

	// �f�o�b�O�p�̏o��
	TRACE1("%2d\n", ulMil);

	if(strFilePath != s && s != "")
	{
		// �t�@�C�������ς�����Ƃ�
		if((Profile::intRepeat == REPEAT_SONG || Profile::intRepeat == REPEAT_ALBUM) 
		&& blnAlbumSoonEnds)
		{
			blnAlbumSoonEnds = FALSE;

			// �P�ȃ��s�[�g��A���o�������s�[�g�̏ꍇ�B
			// �X�g�b�v���čĐ�����
			// �P�ȃ��s�[�g�̏ꍇ�͍Ō�̋ȁA�A���o�������s�[�g�̏ꍇ�͐擪�̋ȂɈړ����邱�ƂɂȂ�
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

			// �����_���Đ��̏ꍇ
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

			// ���̋Ȃ��Ȃ��ꍇ�́A���̂܂܋ȏ��X�V����
		}

		// ���̂܂܋ȏ��X�V
		UpdateFileInfo(s) ;
		SetVisiblity( TRUE) ;

		if(pArchiveFile->GetStatus() == ArchiveFile::Status::UNCOMPRESSED 
		&& (Profile::intRepeat == REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM))
		{
			// �����_���Đ��̏ꍇ
			UINT uiNextSong = GetRandomNextSong();
			HWND hwndWinamp = pMainWnd->GetWinampWindow();
			if(uiNextSong == -1)
			{
				// ���̋Ȃ��Ȃ��ꍇ�́A�v���C���X�g�̎��̋Ȃ�
				SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON5, 0);
				goto cleanup;
			}

			Go(uiNextSong);
			goto cleanup;
		}
	}

	// �����k�̏ꍇ
	if(pArchiveFile->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		// �����k�ȊO�̏ꍇ
		if(Profile::blnShowOnlyArchive)
		{
			SetVisiblity( FALSE) ;
		}
		goto cleanup;
	}

	// �P�ȃ��s�[�g�̏���
	if(Profile::intRepeat != 0)
	{
		HWND hwndWinamp = pMainWnd->GetWinampWindow();
		ULONG ulAlbumLength = SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
		blnAlbumSoonEnds = (ulAlbumLength * 1000 - ulMil < 30 * 1000);	// �A���o�������������I��邩�ǂ������m�F

		if(blnAlbumSoonEnds)
		{
			intCurListPos = SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETLISTPOS);
		}
	}

	// �t�@�C���ԍ��X�V
	ulCurFileNum = pArchiveFile->GetSongIndex(ulMil) ;
	if(pMainWnd->GetCurSong() != ulCurFileNum && ulCurFileNum < pArchiveFile->GetChildFileCount())
	{
		// �Ȃ��ς�����Ƃ�
		TRACE2("album song changed : %d -> %d\n", pMainWnd->GetCurSong(), ulCurFileNum);
		if(Profile::intRepeat == REPEAT_SONG)
		{
			// �P�ȃ��s�[�g
			// pMainWnd �̋Ȕԍ��ɔ�ԁB�P�O�̂͂��B
			Go(pMainWnd->GetCurSong());
		}
		else if(Profile::intRepeat== REPEAT_ENDLESSRANDOM || Profile::intRepeat == REPEAT_RANDOM)
		{
			// �����_���Đ���
			// �Đ��̈ʒu�ύX�܂łɂ�����덷�����ݍ��ނ���
			UINT uiCurTick = GetTickCount();
			if(uiCurTick - uiLastChangedTickCount > 1000) // TickCount ���������̂͂܂������B49.7���A���N���Ɉ�񂾂��A���v�ł��傤
			{
				// �����_���Đ����Ɏ��̋Ȃֈڂ���
				UINT uiNextSong = GetRandomNextSong();
				HWND hwndWinamp = pMainWnd->GetWinampWindow();
				if(uiNextSong == -1)
				{
					// ���̋Ȃ��Ȃ��ꍇ�́A�v���C���X�g�̎��̋Ȃ�
					SendMessage(hwndWinamp, WM_COMMAND, WINAMP_BUTTON5, 0);
					goto cleanup;
				}

				Go(uiNextSong);
			}
		}
		else
		{
			// ���ʂɍĐ���
			pMainWnd->SetCurSong(ulCurFileNum, pArchiveFile->GetChildFile(ulCurFileNum)->GetPlayLength()) ;
		}
	}

	// �\�������X�V
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
// �t�@�C�����̍X�V
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;

	// pArchiveFile �̍X�V
	if( pArchiveFile)
	{
		pMainWnd->ClearList() ;
		delete pArchiveFile ;
	}
	pArchiveFile = new ArchiveFile(strFilePath) ;
	pArchiveFile->ReadHeader() ;

	// zip �t�@�C���Ȃ�
	switch( pArchiveFile->GetStatus())
	{
		case ArchiveFile::Status::OPEN_ERROR :
			pMainWnd->AddList( "�t�@�C�����J���܂���ł���") ;
			break ;

		case ArchiveFile::Status::NO_HEADER:
			pMainWnd->AddList( "�w�b�_����ǂݎ��܂���ł���") ;
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ArchiveFile::Status::INVALID_HEADER :
			pMainWnd->AddList( "�w�b�_���̓ǂ݂Ƃ�Ɏ��s���܂����B") ;
			pMainWnd->SetCurSong( 0, 0) ;
			break ;

		case ArchiveFile::Status::UNCOMPRESSED :
		case ArchiveFile::Status::COMPRESSED :
		{
			// �����_���Đ��p�̃��X�g�v�Z
			CreateRandomSongList();

			// �R���s���[�V�����A���o�����ǂ����̔��f
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

			// ���X�g�ɒǉ����Ă���
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

	// �t�@�C�����E�C���h�E���X�V
	if(pInfoWnd)	pInfoWnd->SetArchiveFile(pArchiveFile);
}


/******************************************************************************/
//		�����_���Đ��p���X�g
/******************************************************************************/
// �����_���Đ��p�̃��X�g�쐬
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::CreateRandomSongList()
{
	vecRandomSongList.clear();

	for(int i = 0; i < pArchiveFile->GetChildFileCount(); i++)
	{
		// �Ȃ̒���������ꍇ�̂݃����_���Đ��p�̃��X�g�ɒǉ�����
		File* pFile = pArchiveFile->GetChildFile(i);
		if(pFile && pFile->GetPlayLength() != 0)
		{
			vecRandomSongList.push_back(i);
		}
	}
}


/******************************************************************************/
// �����_���Đ��̂Ƃ��Ɏ��̋Ȃ��擾����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT Controller::GetRandomNextSong()
{
	// ���X�g����̎�
	if(vecRandomSongList.size() == 0)
	{
		if(Profile::intRepeat == REPEAT_ENDLESSRANDOM)
		{
			// �i�v�����_���̏ꍇ�́A���X�g�̍�蒼��
			CreateRandomSongList();
		}
		else
		{
			// �ꏄ�Ŏ��ւ̏ꍇ�́A-1 ��Ԃ�
			return -1;
		}
	}

	// ���X�g�̒�����P�I��
	int intIndex = ((double)rand() / (double)RAND_MAX) * (double)vecRandomSongList.size();
	int intRet = vecRandomSongList[intIndex];
	vecRandomSongList.erase(vecRandomSongList.begin() + intIndex);
	uiLastChangedTickCount = GetTickCount();
	return intRet;
}


/******************************************************************************/
//		���E�C���h�E
/******************************************************************************/
// ���E�C���h�E��\��/�X�V
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// ���E�C���h�E���\���ɂ���
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::CloseInfoWnd()
{
	if(pInfoWnd)
	{
		pInfoWnd->Close();
	}
}