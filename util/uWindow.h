
// uWindow.h
// �E�C���h�E���������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __WINDOW_H__
#define  __WINDOW_H__

#include  "uTstring.h"
#include  <windows.h>


/******************************************************************************/
// 		�v���g�^�C�v�錾
/******************************************************************************/

tstring GetWindowString(HWND hwnd);
void SetWindowString(HWND hwnd, const tstring& str);
tstring GetDlgString(HWND hwnd, UINT uiID);
void SetDlgString(HWND hwnd, UINT uiID, const tstring& s);
VOID CenterWindow(HWND hwnd);
RECT GetChildRect(HWND hwndParent, HWND hwndChild);

#endif