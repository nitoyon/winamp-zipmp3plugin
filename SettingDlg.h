
// SettingDlg.h
//============================================================================//
// �X�V�F03/05/04(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class SettingDlg
{
private:
	HWND	m_hWnd ;
	static HWND hwndStatic ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	SettingDlg() ;
	~SettingDlg() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( LinkStaticProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnCheck		( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnNormal	( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnID3		( HWND, WPARAM, LPARAM) ;
	BOOL OnUrlClicked	( HWND, WPARAM, LPARAM) ;
	BOOL OnBmpClicked	( HWND, WPARAM, LPARAM) ;
	BOOL OnSetCursor	( HWND, WPARAM, LPARAM) ;
	BOOL OnCtlColorStatic	( HWND, WPARAM, LPARAM) ;

// ���̑�
	void Validiate() ;
	void VarMenu( vector<string>&, int, int) ;
} ;

#endif