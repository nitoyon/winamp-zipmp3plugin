
// FontDlg.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __FONT_DLG_H__
#define  __FONT_DLG_H__

#include "..\include.h"
#include "..\MsgMap.h"
#include "ChildDlg.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class FontDlg : public ChildDlg
{
private:

public:
// �R���X�g���N�^����уf�X�g���N�^
	FontDlg() ;
	~FontDlg() ;

// �K�p
	void DoApply();

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP(FontDlgProc)
	friend int CALLBACK FontFamEnumProc(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* lpntm, int FontType, LPARAM lParam);

	BOOL OnInitDialog	(HWND, WPARAM, LPARAM);
} ;

#endif