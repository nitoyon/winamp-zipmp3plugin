
// InfoWnd.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "InfoWnd.h"
#include "Profile.h"
#include "ArchiveFile.h"


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
	}

	HWND m_hWnd = CreateWindow(INFOWND_CLASS, NULL,	WS_OVERLAPPEDWINDOW, 
		0, 0, 300, 100, NULL, NULL, Profile::hInstance, (LPVOID)this);
	ShowWindow(m_hWnd, SW_SHOW);

	if (!m_hWnd) 
	{
		MessageBox(NULL, "���E�C���h�E���쐬�ł��܂���ł���", "�G���[", MB_OK);
		return NULL;
	}

	return m_hWnd;
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
	pArchiveFile = p;
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
//	ON_MESSAGE( WM_KEYDOWN			, OnKeyDown)
//	ON_MESSAGE( WM_SYSKEYDOWN		, OnSysKeyDown)
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
	lvc.cx		= 50;
	lvc.pszText	= "�t�@�C����";
	ListView_InsertColumn(hwndList, 0, &lvc);
	lvc.pszText	= "�g���b�N��";
	ListView_InsertColumn(hwndList, 1, &lvc);
	lvc.pszText	= "�A�[�e�B�X�g��";
	ListView_InsertColumn(hwndList, 2, &lvc);
	lvc.pszText	= "�A���o����";
	ListView_InsertColumn(hwndList, 3, &lvc);
	lvc.pszText	= "�g���b�N�ԍ�";
	ListView_InsertColumn(hwndList, 4, &lvc);
	lvc.pszText	= "�N";
	ListView_InsertColumn(hwndList, 5, &lvc);
	lvc.pszText	= "�R�����g";
	ListView_InsertColumn(hwndList, 6, &lvc);

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
// �T�C�Y�ύX
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT InfoWnd::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	MoveWindow(hwndList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	return 0;
}