
// Controller.cpp
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Controller.h"
#include "ZipFile.h"
#include "MainWnd.h"
#include "Profile.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/

Controller* Controller::pInstance = NULL ;


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// 
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Controller::Controller() 
: strFilePath( ""), pZipFile( NULL)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Controller::~Controller() 
{
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
//		�ݒ�
/******************************************************************************/
// Go
//============================================================================//
// �X�V�F02/12/24(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �����ݒ�
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

		// �t�@�C���ԍ��X�V
		ULONG ulCurFileNum = pZipFile->GetSongIndex( ulCurTime) ;
		if( ulCurFileNum != ulFileNum)
		{
			ulFileNum = ulCurFileNum ;
			pMainWnd->SetCurSong( ulFileNum) ;
		}

		// �\�������X�V
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
// �t�@�C�����̍X�V
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::UpdateFileInfo( const string& s)
{
	strFilePath = s ;
	ulFileNum = 0 ;
	pMainWnd->SetCurSong( 0) ;

	// pZipFile �̍X�V
	if( pZipFile)
	{
		delete pZipFile ;
	}
	pZipFile = new ZipFile( strFilePath) ;
	pZipFile->ReadHeader() ;

	// zip �t�@�C���Ȃ�
	pMainWnd->ClearList() ;
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
			for( int i = 0; i < pZipFile->GetChildFileCount(); i++)
			{
				pMainWnd->AddList( pZipFile->GetFileName( i)) ;
			}
			break ;
		}
	}
}