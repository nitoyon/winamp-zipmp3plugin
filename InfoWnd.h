
// InfoWnd.h
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __INFO_WND_H__
#define  __INFO_WND_H__

#include "include.h"
#include "MsgMap.h"
class ArchiveFile;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class InfoWnd
{
private:
	HWND		m_hWnd;
	HWND		hwndList;
	static BOOL	blnInit;

	ArchiveFile*	pArchiveFile;

public:
// �R���X�g���N�^����уf�X�g���N�^
	InfoWnd() ;
	~InfoWnd() ;

// �\��
	HWND Create();
	void Close();

// �E�C���h�E�n���h��
	HWND GetHwnd() const{return m_hWnd;}

// �A�[�J�C�u�t�@�C���֌W
	void SetArchiveFile(ArchiveFile*);

// ���b�Z�[�W�n���h��
private:
	DECLARE_MESSAGE_MAP( InfoWndProc)

	LRESULT OnCreate	( HWND, WPARAM, LPARAM) ;
	LRESULT OnSize		( HWND, WPARAM, LPARAM) ;
	LRESULT OnMove		( HWND, WPARAM, LPARAM) ;
	LRESULT OnDestroy	( HWND, WPARAM, LPARAM) ;
	LRESULT OnClose		( HWND, WPARAM, LPARAM) ;
	LRESULT OnChangePos	( HWND, WPARAM, LPARAM) ;
} ;

#endif