
// SettingDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
class ChildDlg;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class SettingDlg
{
private:
	HWND		m_hWnd ;
	HWND		hwndTree;
	HIMAGELIST	hImgList;
	HINSTANCE	hInstance;

	vector<ChildDlg*>	vecChildDlg;
	int	intCurDlg;

public:
// �R���X�g���N�^����уf�X�g���N�^
	SettingDlg(HINSTANCE h);
	~SettingDlg() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnTreeNotify	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;

// ���̑�
	void DisplayDlg(int);
} ;

#endif