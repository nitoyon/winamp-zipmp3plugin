
// Controller.cpp
//============================================================================//
// �X�V�F02/12/28(�y)
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
: strFilePath( ""), pZipFile( NULL), blnUseHotKey( FALSE)
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
// �z�b�g�L�[�̓o�^
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT Controller::SetHotKey( WORD wHotKey)
{
	if( blnUseHotKey)
	{
		UnregisterHotKey( pMainWnd->GetHwnd(), HOTKEY_SHOW) ;
	}

	// �`���ϊ�
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
// �����ݒ�
//============================================================================//
// �X�V�F02/12/27(��)
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

		ulMil ;

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
