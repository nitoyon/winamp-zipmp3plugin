
// InfoWnd.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "InfoWnd.h"
#include "Profile.h"
#include "ArchiveFile.h"
#include "Controller.h"
#include "MainWnd.h"
#include "util\uTstring.h"


/******************************************************************************/
//		��`
/******************************************************************************/

BOOL InfoWnd::blnInit = FALSE;
#define  INFOWND_CLASS	"info wnd"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

InfoWnd::InfoWnd() 
: m_hWnd(NULL)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

InfoWnd::~InfoWnd() 
{
}


/******************************************************************************/
// ������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

HWND InfoWnd::Create()
{
	if(m_hWnd)
	{
		return m_hWnd;
	}

	if(!blnInit)
	{
		WNDCLASS wc;
		memset(&wc,0,sizeof(wc));
		wc.lpfnWndProc = (WNDPROC)InfoWndProc;
		wc.hInstance = Profile::hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW) ;
		wc.style = CS_DBLCLKS ;
		wc.lpszClassName = INFOWND_CLASS;

		if(!RegisterClass(&wc)) 
		{
			return NULL;
		}
		blnInit = TRUE;
	}

	HWND h = Controller::GetInstance()->GetWindow()->GetHwnd();
	HWND m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, INFOWND_CLASS, "���ݍĐ����̃A���o���̏ڍ�", WS_OVERLAPPEDWINDOW, 
		Profile::intInfoX, Profile::intInfoY, Profile::intInfoWidth, Profile::intInfoHeight, 
		h, NULL, Profile::hInstance, (LPVOID)this);
	ShowWindow(m_hWnd, SW_SHOW);

	if (!m_hWnd) 
	{
		MessageBox(NULL, "���E�C���h�E���쐬�ł��܂���ł���", "�G���[", MB_OK);
		return NULL;
	}

	return m_hWnd;
}


/******************************************************************************/
// ����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void InfoWnd::Close()
{
	if(m_hWnd)
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
}


/******************************************************************************/
//		�A�[�J�C�u�t�@�C���֌W
/******************************************************************************/
// �A�[�J�C�u�t�@�C����ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void InfoWnd::SetArchiveFile(ArchiveFile* p)
{
	if(p == NULL) return;
	pArchiveFile = p;
	ListView_DeleteAllItems(hwndList);

	int i;
	for(i = 0; i < pArchiveFile->GetChildFileCount(); i++)
	{
		File* pFile = pArchiveFile->GetChildFile(i);
		string s = pFile->GetFileName();
		LVITEM item;
		item.mask	= LVIF_TEXT | LVIF_PARAM;
		item.iItem	= i;
		item.iSubItem	= 0;
		item.pszText	= (LPTSTR)s.c_str();
		item.lParam	= 0;
		ListView_InsertItem(hwndList, &item);

		if(pFile->HasID3Tag())
		{
			ID3Tag tag = pFile->GetID3Tag();
			s = tag.strTrackName;		ListView_SetItemText(hwndList, i, 1, (LPTSTR)s.c_str());
			s = tag.strArtistName;		ListView_SetItemText(hwndList, i, 2, (LPTSTR)s.c_str());
			s = tag.strAlbumName;		ListView_SetItemText(hwndList, i, 3, (LPTSTR)s.c_str());
			s = Int2Str(tag.intTrackNum);	ListView_SetItemText(hwndList, i, 4, (LPTSTR)s.c_str());
			s = Int2Str(tag.intYear);	ListView_SetItemText(hwndList, i, 5, (LPTSTR)s.c_str());
			s = tag.strComment;		ListView_SetItemText(hwndList, i, 6, (LPTSTR)s.c_str());
		}
	}

	for(i = 0; i < 7; i++)
	{
		ListView_SetColumnWidth(hwndList, i, LVSCW_AUTOSIZE);
	}
}


/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_MESSAGE_MAP( InfoWndProc, InfoWnd)
	ON_MESSAGE( WM_CREATE			, OnCreate)
	ON_MESSAGE( WM_SIZE			, OnSize)
	ON_MESSAGE( WM_DESTROY			, OnDestroy)
	ON_MESSAGE( WM_CLOSE			, OnClose)
	ON_MESSAGE( WM_MOVE			, OnMove)
END_MESSAGE_MAP()


/******************************************************************************/
// �쐬��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// ���X�g�E�C���h�E�쐬
	hwndList = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT, 
		0, 0, 0, 0, hWnd, 0, Profile::hInstance, NULL); 

	LVCOLUMN lvc;
	lvc.mask	= LVCF_WIDTH | LVCF_TEXT;
	lvc.cx		= 100;
	lvc.pszText	= "�t�@�C����";
	ListView_InsertColumn(hwndList, 0, &lvc);
	lvc.pszText	= "�g���b�N��";
	ListView_InsertColumn(hwndList, 1, &lvc);
	lvc.pszText	= "�A�[�e�B�X�g��";
	ListView_InsertColumn(hwndList, 2, &lvc);
	lvc.pszText	= "�A���o����";
	ListView_InsertColumn(hwndList, 3, &lvc);
	lvc.cx		= 40;
	lvc.pszText	= "�g���b�N�ԍ�";
	ListView_InsertColumn(hwndList, 4, &lvc);
	lvc.pszText	= "�N";
	ListView_InsertColumn(hwndList, 5, &lvc);
	lvc.cx		= 100;
	lvc.pszText	= "�R�����g";
	ListView_InsertColumn(hwndList, 6, &lvc);

	Profile::blnInfoVisible = TRUE;
	return 0;
}


/******************************************************************************/
// �p����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = NULL;
	return 0;
}


/******************************************************************************/
// ����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	Profile::blnInfoVisible = FALSE;
	DestroyWindow(hWnd);
	return 0;
}


/******************************************************************************/
// �T�C�Y�ύX
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	Profile::intInfoX	= rect.left;
	Profile::intInfoY	= rect.top;
	Profile::intInfoWidth	= rect.right	- rect.left;
	Profile::intInfoHeight	= rect.bottom	- rect.top;
	return 0;
}


/******************************************************************************/
// �T�C�Y�ύX
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	MoveWindow(hwndList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	OnMove(hWnd, 0, 0);
	return 0;
}