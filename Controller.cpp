
// Controller.cpp
//============================================================================//
// �X�V�F02/12/24(��)
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
// �����ݒ�
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::SetMp3Pos( const string& s, ULONG ulMil)
{
	if( strFilePath != s)
	{
		strFilePath = s ;

		// pZipFile �̍X�V
		if( pZipFile)
		{
			delete pZipFile ;
		}
		pZipFile = new ZipFile( strFilePath) ;
		pZipFile->ReadHeader() ;

		// zip �t�@�C���Ȃ�
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
// �t�@�C�����̍X�V
//============================================================================//
// �X�V�F02/12/23(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Controller::Update()
{
	// ���̋Ȃɍs�����Ȃ�
	if( vecSongHeadTime[ ulFileNum + 1] <= ulCurTime)
	{
		// �X�V
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
	// �����߂����Ƃ�
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