
// Controller.cpp
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Controller.h"
#include "ZipFile.h"
#include "MainWnd.h"
#include "Profile.h"
#include <wininet.h>


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
: strFilePath( ""), strPrevTmpPath( ""), pZipFile( NULL), blnUseHotKey( FALSE)
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
	if( !pZipFile)
	{
		return ;
	}
	ULONG ulMilisec = pZipFile->GetSongHead( pMainWnd->GetCurSong()) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_COMMAND, WINAMP_BUTTON2, 0) ;
	SendMessage( pMainWnd->GetWinampWindow(), WM_WA_IPC, ulMilisec, IPC_JUMPTOTIME) ;
}


/******************************************************************************/
// �ړ�
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

	// �Đ����łȂ��Ȃ�΁A�蓮�Ńt�@�C���ԍ��X�V
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
// �𓀂���
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �~�j�u���E�U�[�ŊJ��
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::OpenInMiniBrowser( UINT i)
{
	File* pFile = pZipFile->GetChildFile( i) ;
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
	if( strFilePath != s)
	{
		UpdateFileInfo( s) ;
		SetVisiblity( TRUE) ;
	}

	if( pZipFile->GetStatus() == ZipFile::Status::UNCOMPRESSED)
	{
		// �t�@�C���ԍ��X�V
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulMil) ;
		if( pMainWnd->GetCurSong() != ulCurFileNum)
		{
			pMainWnd->SetCurSong( ulCurFileNum) ;
		}

		// �\�������X�V
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
// �t�@�C�����̍X�V
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;

	// pZipFile �̍X�V
	if( pZipFile)
	{
		pMainWnd->ClearList() ;
		delete pZipFile ;
	}
	pZipFile = new ZipFile( strFilePath) ;
	pZipFile->ReadHeader() ;

	// zip �t�@�C���Ȃ�
	switch( pZipFile->GetStatus())
	{
		case ZipFile::Status::OPEN_ERROR :
			pMainWnd->AddList( "�t�@�C�����J���܂���ł���") ;
			break ;

		case ZipFile::Status::NOT_ZIP :
			pMainWnd->AddList( "ZIP �t�@�C���ł͂���܂���") ;
			break ;

		case ZipFile::Status::INVALID_HEADER :
			pMainWnd->AddList( "ZIP �t�@�C���̃w�b�_���̓ǂ݂Ƃ�Ɏ��s���܂����B") ;
			pMainWnd->AddList( "������ ZIP �t�@�C���ł͂Ȃ����A���̃v���O�C�����w�{���\��������܂��B") ;
			break ;

		case ZipFile::Status::UNCOMPRESSED :
		case ZipFile::Status::COMPRESSED :
		{
			// �R���s���[�V�����A���o�����ǂ����̔��f
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

			// ���X�g�ɒǉ����Ă���
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
			break ;
		}
	}
}
