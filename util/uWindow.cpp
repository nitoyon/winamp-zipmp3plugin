
// uWindow.cpp
// �E�C���h�E���������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include  "uWindow.h"


/******************************************************************************/
//		��ʓI�ȃE�C���h�E
/******************************************************************************/
// �E�C���h�E�̃^�C�g�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetWindowString(HWND hwnd)
{
	int intSize = GetWindowTextLength(hwnd);
	TCHAR* pszBuf = new TCHAR[intSize + 1];

	GetWindowText(hwnd, pszBuf, intSize + 1);
	tstring s = pszBuf;
	delete[] pszBuf;

	return s ;
}


/******************************************************************************/
// �E�C���h�E�̃^�C�g����ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void SetWindowString(HWND hwnd, const tstring& str)
{
	SetWindowText(hwnd, str.c_str());
}


/******************************************************************************/
// �_�C�A���O�̃^�C�g���擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetDlgString(HWND hwnd, UINT uiID)
{
	HWND h = GetDlgItem(hwnd, uiID);
	return GetWindowString(h);
}


/******************************************************************************/
// �_�C�A���O�̃^�C�g���ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void SetDlgString(HWND hwnd, UINT uiID, const tstring& s)
{
	HWND h = GetDlgItem(hwnd, uiID);
	SetWindowString(h, s);
}


/******************************************************************************/
// �N���X�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetWindowClass(HWND hwnd)
{
	int intSize = 2;
	PTSTR pszBuf;

	while(1)
	{
		pszBuf = new TCHAR[intSize];
		int i = GetClassName(hwnd, pszBuf, intSize);
		if(i != intSize - 1)
		{
			break;
		}

		intSize *= 2;
		delete[] pszBuf;
	}

	tstring s = pszBuf;
	delete[] pszBuf;
	return s;
}


/******************************************************************************/
// �E�C���h�E�𒆐S�ɕ\��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

VOID CenterWindow(HWND hwnd)
{
	RECT    rect;
	LONG    dx, dy;
	LONG    dxParent, dyParent;
	LONG    Style;

	// Get window rect
	GetWindowRect(hwnd, &rect);

	dx = rect.right  - rect.left;
	dy = rect.bottom - rect.top;

	// Get parent rect
	Style = GetWindowLong(hwnd, GWL_STYLE);
	if ((Style & WS_CHILD) == 0)
	{
		// Return the desktop windows size (size of main screen)
		dxParent = GetSystemMetrics(SM_CXSCREEN);
		dyParent = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		HWND    hwndParent;
		RECT    rectParent;

		hwndParent = GetParent(hwnd);
		if(hwndParent == NULL)
		{
			hwndParent = GetDesktopWindow();
		}

		GetWindowRect(hwndParent, &rectParent);

		dxParent = rectParent.right - rectParent.left;
		dyParent = rectParent.bottom - rectParent.top;
	}

	// Centre the child in the parent
	rect.left = (dxParent - dx) / 2;
	rect.top  = (dyParent - dy) / 3;

	// Move the child into position
	SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);
}


/******************************************************************************/
// �E�C���h�E�̐e�E�C���h�E���̏ꏊ���擾
//============================================================================//
// �T�v�F�l���̃N���C�A���g���W��Ԃ��B
// �⑫�F�Ȃ��B
//============================================================================//

RECT GetChildRect(HWND hwndParent, HWND hwndChild)
{
	RECT rcChild;
	RECT rect;
	
	GetWindowRect(hwndChild, &rcChild);

	POINT pt = {rcChild.left, rcChild.top};
	ScreenToClient(hwndParent, &pt);

	rect.left	= pt.x;
	rect.top	= pt.y;
	rect.right	= rcChild.right  - rcChild.left + pt.x;
	rect.bottom	= rcChild.bottom - rcChild.top  + pt.y;

	return rect;
}


/******************************************************************************/
//		�R���g���[��
/******************************************************************************/
// ���X�g�{�b�N�X�̃e�L�X�g�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetListBoxString(HWND hwndList, int intIndex)
{
	UINT uiLen = (UINT)SendMessage(hwndList, LB_GETTEXTLEN, intIndex, 0);
	PTSTR pszBuf = new TCHAR[uiLen + 1];
	SendMessage(hwndList, LB_GETTEXT, intIndex, (LPARAM)pszBuf);

	tstring s = pszBuf;
	delete[] pszBuf;
	return s;
}


/******************************************************************************/
// �R���{�{�b�N�X�̃e�L�X�g�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetComboBoxString(HWND hwndCombo, int intIndex)
{
	int intTxtLen = SendMessage(hwndCombo, CB_GETLBTEXTLEN, intIndex, 0);
	tstring strRet = TEXT("");

	if(intTxtLen != CB_ERR)
	{
		TCHAR* pszBuf = new TCHAR[intTxtLen + 1];
		if(SendMessage(hwndCombo, CB_GETLBTEXT, intIndex, (LPARAM)pszBuf) != CB_ERR)
		{
			strRet = pszBuf;
		}
		delete[] pszBuf;
	}

	return strRet;
}


/******************************************************************************/
// �c���[�r���[�̃e�L�X�g�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetTreeString(HWND hwndTree, HTREEITEM hitem)
{
	DWORD	dwSize = 64;
	TVITEM	ti;
	LPTSTR	pszBuf = NULL;

	while(1)
	{
		pszBuf = new TCHAR[dwSize];
		ti.mask = TVIF_TEXT ;
		ti.hItem = hitem;
		ti.pszText = pszBuf;
		ti.cchTextMax = dwSize;
		if(TreeView_GetItem(hwndTree, &ti))
		{
			ti.pszText[dwSize - 1] = TEXT('\0');
			if((UINT)lstrlen(ti.pszText) == dwSize -1)
			{
				dwSize *= 2;
				delete[] pszBuf;
				pszBuf = NULL;
			}
			else
			{
				// �擾�ł������̏���
				break;
			}
		}
		else
		{
			// �f�[�^�擾���s�B�n���h�������������H
			break;
		}
	}

	tstring	s = TEXT("");
	if(pszBuf)
	{
		s = pszBuf;;
		delete[] pszBuf;
	}

	return s;
}


/******************************************************************************/
// ���X�g�r���[�̃e�L�X�g�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetListViewString(HWND hWnd, int intItem, int intSubItem)
{
	DWORD	dwSize = 256;
	LPTSTR	pszBuf;
	while(1)
	{
		pszBuf = new TCHAR[dwSize];
		ListView_GetItemText(hWnd, intItem, intSubItem, pszBuf, dwSize);
		pszBuf[dwSize - 1] = TEXT('\0');
		if(lstrlen(pszBuf) != dwSize - 1)
		{
			break;
		}

		delete[] pszBuf;
		dwSize *= 2;
	}

	tstring s = pszBuf;
	delete[] pszBuf;

	return s;
}