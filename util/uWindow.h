
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
#include  <commctrl.h>


/******************************************************************************/
// 		�v���g�^�C�v�錾
/******************************************************************************/

// ��ʓI�ȃE�C���h�E
tstring GetWindowString(HWND hwnd);
void SetWindowString(HWND hwnd, const tstring& str);
tstring GetDlgString(HWND hwnd, UINT uiID);
void SetDlgString(HWND hwnd, UINT uiID, const tstring& s);
tstring GetWindowClass(HWND hwnd);

VOID CenterWindow(HWND hwnd);
RECT GetChildRect(HWND hwndParent, HWND hwndChild);

// �R���g���[��
tstring GetComboBoxString(HWND hwndCombo, int intIndex);
tstring GetListBoxString(HWND hwndList, int intIndex);
tstring GetTreeString(HWND hwndTree, HTREEITEM hitem);
tstring GetListViewString(HWND hWnd, int intItem, int);

#endif