
// uProcess.cpp
// プロセスのラップ関数群
//============================================================================//
// 概要：なし。
// 補足：UNICODE 文字列には非対応。
//============================================================================//

#include "uProcess.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  CLOSE_HANDLE(HDL)	if(HDL){CloseHandle(HDL);HDL = NULL;}


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

Process::Process(UINT uiFlag)
: hProcess(NULL)
, blnStdIn(FALSE), blnStdOut(FALSE), blnStdErr(FALSE)
, hStdInW(NULL), hStdInR(NULL)
, hStdOutW(NULL), hStdOutR(NULL)
, hStdErrW(NULL), hStdErrR(NULL)
{
	if(uiFlag & FLAG::STDIN)
	{
		blnStdIn = TRUE;
	}
	if(uiFlag & FLAG::STDOUT)
	{
		blnStdOut = TRUE;
	}
	if(uiFlag & FLAG::STDERR)
	{
		blnStdErr = TRUE;
	}
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

Process::~Process()
{
	Close();
}


/******************************************************************************/
//		起動
/******************************************************************************/
// プロセス作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Process::Create(LPCSTR pszCmdLine, STARTUPINFOA* psi)
{
	STARTUPINFOA si = SetStartupInfo(psi);
	PROCESS_INFORMATION	pi;

	// プロセス作成
	LPSTR pszCmd = (LPSTR)new CHAR[strlen(pszCmdLine) + 1];
	strcpy(pszCmd, pszCmdLine);
	BOOL blnResult = CreateProcessA(NULL, pszCmd, NULL, NULL, TRUE, 
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	delete[] pszCmd;
	if(!blnResult)
	{
		DWORD dwErr = GetLastError();
		CloseAllIoHandles();
		SetLastError(dwErr);
		return FALSE;
	}

	CloseHandle(pi.hThread);
	CloseIoHandleAfterCreate();
	hProcess = pi.hProcess;

	return TRUE;
}


/******************************************************************************/
// プロセス作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Process::CreateAsUser(HANDLE hToken, LPCSTR pszCmdLine, STARTUPINFOA* psi)
{
	STARTUPINFOA si = SetStartupInfo(psi);
	PROCESS_INFORMATION	pi;

	// プロセス作成
	LPSTR pszCmd = (LPSTR)new CHAR[strlen(pszCmdLine) + 1];
	if(pszCmdLine != 0)
	{
		strcpy(pszCmd, pszCmdLine);
	}
	else
	{
		pszCmd[0] = L'\0';
	}
	BOOL blnResult = CreateProcessAsUserA(hToken, NULL, pszCmd, NULL, NULL, TRUE, 
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	delete[] pszCmd;
	if(!blnResult)
	{
		DWORD dwErr = GetLastError();
		CloseAllIoHandles();
		SetLastError(dwErr);
		return FALSE;
	}

	CloseHandle(pi.hThread);
	CloseIoHandleAfterCreate();
	hProcess = pi.hProcess;

	return TRUE;
}


/******************************************************************************/
// ハンドルを閉じる
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Process::Close()
{
	if(hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	CloseAllIoHandles();
}


/******************************************************************************/
// プロセスのハンドルを返す
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

HANDLE Process::GetHandle() const
{
	return hProcess;
}


/******************************************************************************/
// プロセスのハンドルを返す
//============================================================================//
// 概要：なし。
// 補足：ハンドルを複製して返す。
//============================================================================//

HANDLE Process::GetDupHandle() const
{
	if(!hProcess)
	{
		return 0;
	}

	HANDLE hRet;
	DuplicateHandle(GetCurrentProcess(), hProcess, GetCurrentProcess(), &hRet, NULL, TRUE, DUPLICATE_SAME_ACCESS);
	return hRet;
}


/******************************************************************************/
// STARTUPINFO の設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

STARTUPINFOA Process::SetStartupInfo(STARTUPINFOA* psi)
{
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb		= sizeof(STARTUPINFO);
	si.dwFlags	= STARTF_USESTDHANDLES;
	si.hStdInput	= GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput	= GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError	= GetStdHandle(STD_ERROR_HANDLE);
	if(psi != NULL)
	{
		si.dwFlags	|= psi->dwFlags;
		si.lpReserved	= psi->lpReserved;
		si.lpDesktop	= psi->lpDesktop;
		si.lpTitle	= psi->lpTitle;
		si.dwX		= psi->dwX;
		si.dwY		= psi->dwY;
		si.dwXSize	= psi->dwXSize;
		si.dwYSize	= psi->dwYSize;
		si.wShowWindow	= psi->wShowWindow;
	}

	// 標準入力設定
	if(blnStdIn)
	{
		CreateStdIn();
		si.hStdInput = hStdInR;
	}
	if(blnStdOut)
	{
		CreateStdOut();
		si.hStdOutput = hStdOutW;
	}
	if(blnStdErr)
	{
		CreateStdErr();
		si.hStdError = hStdErrW;
	}

	return si;
}


/******************************************************************************/
//		標準入出力
/******************************************************************************/
// 標準入力に文字列を流し込む
//============================================================================//
// 概要：なし。
// 補足：流し込んだら標準入力を閉じる。
//============================================================================//

BOOL Process::SetStdIn(LPCSTR pszStdIn)
{
	DWORD	dwResult;
	BOOL	blnResult;
	blnResult = WriteFile(hStdInW, pszStdIn, strlen(pszStdIn) * sizeof(CHAR), &dwResult, NULL);

	CloseHandle(hStdInW);
	hStdInW = NULL;

	return blnResult;
}


/******************************************************************************/
// 標準出力の読みとり
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BYTE* Process::GetStdOut(DWORD* pdwSize, BOOL blnAddNull)
{
	// サイズの読みとり
	DWORD dwStdOut = 0;
	PeekNamedPipe(hStdOutR, NULL, 0, NULL, &dwStdOut, NULL);
	if(dwStdOut == 0)
	{
		if(blnAddNull)
		{
			BYTE* pByte = new BYTE[1];
			*pByte = '\0';
			return pByte;
		}
		return NULL;
	}

	// サイズ確保＋内容読みとり
	BYTE* pByte = new BYTE[dwStdOut + (blnAddNull ? 1 : 0)];
	if(ReadFile(hStdOutR, pByte, dwStdOut, pdwSize, NULL))
	{
		// 末尾に NULL を追加
		if(blnAddNull)
		{
			pByte[dwStdOut] = '\0';
			*pdwSize++;
		}

		return pByte;
	}

	delete[] pByte;
	return NULL;
}


/******************************************************************************/
// 標準エラー読みとり
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BYTE* Process::GetStdErr(DWORD* pdwSize, BOOL blnAddNull)
{
	// サイズの読みとり
	DWORD dwStdErr = 0;
	PeekNamedPipe(hStdErrR, NULL, 0, NULL, &dwStdErr, NULL);
	if(dwStdErr == 0)
	{
		if(blnAddNull)
		{
			BYTE* pByte = new BYTE[1];
			*pByte = '\0';
			return pByte;
		}
		return NULL;
	}

	// サイズ確保＋内容読みとり
	BYTE* pByte = new BYTE[dwStdErr + (blnAddNull ? 1 : 0)];
	if(ReadFile(hStdErrR, pByte, dwStdErr, pdwSize, NULL))
	{
		// 末尾に NULL を追加
		if(blnAddNull)
		{
			pByte[dwStdErr] = '\0';
			*pdwSize++;
		}

		return pByte;
	}

	delete[] pByte;
	return NULL;
}


/******************************************************************************/
// 標準入力のハンドルを準備
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Process::CreateStdIn()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength		= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle	= TRUE;

	CreatePipe(&hStdInR, &hStdInW, &sa, 0);

	// hStdInW が子プロセスに継承されないようにする
	HANDLE hTmp = hStdInW;
	DuplicateHandle(GetCurrentProcess(), hTmp, GetCurrentProcess(), &hStdInW, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hTmp);
}


/******************************************************************************/
// 標準出力を準備
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Process::CreateStdOut()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength		= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle	= TRUE;

	CreatePipe(&hStdOutR, &hStdOutW, &sa, 0);
}


/******************************************************************************/
// 標準エラーを準備
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Process::CreateStdErr()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength		= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle	= TRUE;

	CreatePipe(&hStdErrR, &hStdErrW, &sa, 0);
}


/******************************************************************************/
// プロセス作成後に不要な読み書きハンドルを閉じる
//============================================================================//
// 概要：子プロセスが利用するハンドルを閉じる。
// 補足：なし。
//============================================================================//

void Process::CloseIoHandleAfterCreate()
{
	CLOSE_HANDLE(hStdInR);
	CLOSE_HANDLE(hStdOutW);
	CLOSE_HANDLE(hStdErrW);
}


/******************************************************************************/
// 全ての読み書きハンドルを閉じる
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void Process::CloseAllIoHandles()
{
	CLOSE_HANDLE(hStdInR);
	CLOSE_HANDLE(hStdInW);
	CLOSE_HANDLE(hStdOutR);
	CLOSE_HANDLE(hStdOutW);
	CLOSE_HANDLE(hStdErrR);
	CLOSE_HANDLE(hStdErrW);
}


/******************************************************************************/
//		待機
/******************************************************************************/
// プロセスがアイドル状態になるのを待機
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DWORD Process::WaitIdle(DWORD dwTime)
{
	if(hProcess)
	{
		return WaitForInputIdle(hProcess, dwTime);
	}
	return 0;
}


/******************************************************************************/
// プロセス終了を待機
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DWORD Process::WaitObject(DWORD dwTime)
{
	if(hProcess)
	{
		return WaitForSingleObject(hProcess, dwTime);
	}
	return 0;
}

