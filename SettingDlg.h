
// SettingDlg.h
//============================================================================//
// �X�V�F02/12/29(��)
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
	HWND	hwndHotKey ;
	static HWND hwndStatic ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	SettingDlg() ;
	~SettingDlg() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnCheck		( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnNormal	( HWND, WPARAM, LPARAM) ;
	BOOL OnBtnID3		( HWND, WPARAM, LPARAM) ;

// ���̑�
	void Validiate() ;
	void VarMenu( vector<string>&, int, int) ;
} ;

#endif