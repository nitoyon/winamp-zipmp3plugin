
// InfoWnd.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "InfoWnd.h"
#include "Profile.h"
#include "ArchiveFile.h"


/******************************************************************************/
//		定義
/******************************************************************************/

BOOL InfoWnd::blnInit = FALSE;
#define  INFOWND_CLASS	"info wnd"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

InfoWnd::InfoWnd() 
: m_hWnd(NULL)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

InfoWnd::~InfoWnd() 
{
}


/******************************************************************************/
// 初期化
//============================================================================//
// 概要：なし。
// 補足：なし。
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
		MessageBox(NULL, "情報ウインドウを作成できませんでした", "エラー", MB_OK);
		return NULL;
	}

	return m_hWnd;
}


/******************************************************************************/
//		アーカイブファイル関係
/******************************************************************************/
// アーカイブファイルを設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void InfoWnd::SetArchiveFile(ArchiveFile* p)
{
	pArchiveFile = p;
}


/******************************************************************************/
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ定義
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BEGIN_MESSAGE_MAP( InfoWndProc, InfoWnd)
	ON_MESSAGE( WM_CREATE			, OnCreate)
	ON_MESSAGE( WM_SIZE			, OnSize)
	ON_MESSAGE( WM_DESTROY			, OnDestroy)
//	ON_MESSAGE( WM_KEYDOWN			, OnKeyDown)
//	ON_MESSAGE( WM_SYSKEYDOWN		, OnSysKeyDown)
END_MESSAGE_MAP()


/******************************************************************************/
// 作成時
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT InfoWnd::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// リストウインドウ作成
	hwndList = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT, 
		0, 0, 0, 0, hWnd, 0, Profile::hInstance, NULL); 

	LVCOLUMN lvc;
	lvc.mask	= LVCF_WIDTH | LVCF_TEXT;
	lvc.cx		= 50;
	lvc.pszText	= "ファイル名";
	ListView_InsertColumn(hwndList, 0, &lvc);
	lvc.pszText	= "トラック名";
	ListView_InsertColumn(hwndList, 1, &lvc);
	lvc.pszText	= "アーティスト名";
	ListView_InsertColumn(hwndList, 2, &lvc);
	lvc.pszText	= "アルバム名";
	ListView_InsertColumn(hwndList, 3, &lvc);
	lvc.pszText	= "トラック番号";
	ListView_InsertColumn(hwndList, 4, &lvc);
	lvc.pszText	= "年";
	ListView_InsertColumn(hwndList, 5, &lvc);
	lvc.pszText	= "コメント";
	ListView_InsertColumn(hwndList, 6, &lvc);

	return 0;
}


/******************************************************************************/
// 廃棄時
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT InfoWnd::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = NULL;
	return 0;
}


/******************************************************************************/
// サイズ変更
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

LRESULT InfoWnd::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	MoveWindow(hwndList, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	return 0;
}