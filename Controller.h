
// Controller.h
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __CONTROLLER_H__
#define  __CONTROLLER_H__

#include "include.h"


/******************************************************************************/
//		��`
/******************************************************************************/

class ZipFile ;
class MainWnd ;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class Controller
{
private:
	static Controller* pInstance ;
	MainWnd*	pMainWnd ;

	string		strFilePath ;
	ZipFile*	pZipFile ;
	ULONG		ulCurTime ;
	ULONG		ulFileNum ;
	ULONG		ulCurSongLength ;
	vector<ULONG>	vecSongHeadTime ;
	ULONG		ulDisplayTime ;

// �R���X�g���N�^����уf�X�g���N�^
private:
	Controller() ;
public:
	~Controller() ;
	static Controller* GetInstance() ;

// �E�C���h�E
	void SetWindow( MainWnd* p){ pMainWnd = p ;} ;

// �ݒ�
	void Go( UINT) ;
	void SetMp3Pos( const string&, ULONG) ;
private:
	void UpdateFileInfo( const string&) ;
} ;

#endif