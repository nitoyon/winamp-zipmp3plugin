
// Controller.cpp
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Controller.h"
#include "ArchiveFile.h"
#include "MainWnd.h"
#include "Profile.h"
#include "util.h"
#include "resource.h"
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
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Controller::Controller() 
: strFilePath( ""), strPrevTmpPath( ""), pArchivePath( NULL), blnUseHotKey( FALSE)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/31(��)
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
// �X�V�F02/12/23(��)
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
// �X�V�F02/12/28(�y)
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
	switch( pArchivePath->GetStatus())
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
// �X�V�F02/12/30(��)
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
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::Play()
{
	if( !pArchivePath)
	{
		return ;
	}

	// �����k����Ȃ��ꍇ
	if(pArchivePath->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
		SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, 0, IPC_JUMPTOTIME) ;
		return ;
	}

	ULONG ulMilisec = pArchivePath->GetSongHead( pMainWnd->GetCurSong()) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
}


/******************************************************************************/
// �ړ�
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::Go( UINT u, int intDiff)
{
	if( !pArchivePath)
	{
		return ;
	}
	if( u < 0 || u >= pArchivePath->GetChildFileCount())
	{
		return;
	}

	HWND hwnd = pMainWnd->GetWinampWindow() ;
	ULONG ulMilisec = pArchivePath->GetSongHead( u) + intDiff;
	SendMessage( hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;

	// �Đ����łȂ��Ȃ�΁A�蓮�Ńt�@�C���ԍ��X�V
	if( SendMessage( hwnd, WM_WA_IPC, 0, IPC_ISPLAYING) != 1)
	{
		ULONG ulCurFileNum = pArchivePath->GetSongIndex( ulMilisec) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum)
		{
			SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
			SendMessage( hwnd, WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
		}
	}
}


/******************************************************************************/
// �𓀂���
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//
BOOL Controller::Extract( UINT ui, const string& strPath)
{
	File* pFile = pArchivePath->GetChildFile( ui) ;
	if( !pFile)
	{
		return FALSE ;
	}

	if( pArchivePath->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
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
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Controller::ExtractDetail( UINT ui, UINT uiMsg)
{
	if( pArchivePath->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		return FALSE ;
	}

	string strOutPath ;

	if( uiMsg == IDM_EXTRACT_HERE)
	{
		strOutPath = GetDirName(pArchivePath->GetChildFile(ui)->GetArchivePath()) ;
		strOutPath += pArchivePath->GetChildFile(ui)->GetFileName() ;
	}
	else if( uiMsg == IDM_EXTRACT_SELECT)
	{
		OPENFILENAME	ofn ;
		char		pszFileBuf[ MAX_PATH + 1] ;

		ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = pMainWnd->GetHwnd() ;
		ofn.lpstrFilter = "MP3(*.mp3)\0*.mp3\0���ׂẴt�@�C��\0*.*\0\0";
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
		strOutPath = string( pszBuf) + "\\" + pArchivePath->GetChildFile( ui)->GetFileName() ;
	}

	return Extract( ui, strOutPath) ;
}


/******************************************************************************/
// �~�j�u���E�U�[�ŊJ��
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::OpenInMiniBrowser( UINT i)
{
	if( pArchivePath->GetStatus() != ArchiveFile::Status::UNCOMPRESSED)
	{
		return ;
	}

	File* pFile = pArchivePath->GetChildFile( i) ;
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
	if( Extract( i, pszPath))
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
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s && s != "")
	{
		UpdateFileInfo( s) ;
		SetVisiblity( TRUE) ;
	}

	if( pArchivePath->GetStatus() == ArchiveFile::Status::UNCOMPRESSED)
	{
		// �t�@�C���ԍ��X�V
		ULONG ulCurFileNum = pArchivePath->GetSongIndex( ulMil) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum && ulCurFileNum < pArchivePath->GetChildFileCount())
		{
			pMainWnd->SetCurSong( ulCurFileNum, pArchivePath->GetChildFile( ulCurFileNum)->GetPlayLength()) ;
		}

		// �\�������X�V
		ULONG u = pArchivePath->GetSongTime( ulCurFileNum, ulMil) ;
		u /= 1000 ;
		if( ulDisplayTime != u)
		{
			pMainWnd->SetTime( u / 60, u % 60) ;
			ulDisplayTime = u ;
		}
	}
	else
	{
		if( Profile::blnShowOnlyArchive)
		{
			SetVisiblity( FALSE) ;
		}
	}
}


/******************************************************************************/
// �t�@�C�����̍X�V
//============================================================================//
// �X�V�F03/04/20(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;

	// pArchivePath �̍X�V
	if( pArchivePath)
	{
		pMainWnd->ClearList() ;
		delete pArchivePath ;
	}
	pArchivePath = new ArchiveFile(strFilePath) ;
	pArchivePath->ReadHeader() ;

	// zip �t�@�C���Ȃ�
	switch( pArchivePath->GetStatus())
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
			// �R���s���[�V�����A���o�����ǂ����̔��f
			BOOL blnCompi = FALSE ;
			if( Profile::blnListCompilation)
			{
				string strArtist = "" ;
				for( int i = 0; i < pArchivePath->GetChildFileCount(); i++)
				{
					File* pFile = pArchivePath->GetChildFile( i) ;
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
			for( int i = 0; i < pArchivePath->GetChildFileCount(); i++)
			{
				File* pFile = pArchivePath->GetChildFile( i) ;
				
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
			pMainWnd->SetCurSong( 0, i > 0 ? pArchivePath->GetChildFile( 0)->GetPlayLength() : 0) ;
			break ;
		}
	}
}
