
// uProcess.h
// プロセスのラップ関数群
//============================================================================//
// 概要：なし。
// 補足：UNICODE 文字列には非対応。
//============================================================================//

#ifndef	__PROCESS_H__
#define	__PROCESS_H__

#include <windows.h>


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

class Process
{
private:
	HANDLE	hProcess;

	BOOL	blnStdIn;
	BOOL	blnStdOut;
	BOOL	blnStdErr;

	HANDLE	hStdInW;
	HANDLE	hStdInR;
	HANDLE	hStdOutW;
	HANDLE	hStdOutR;
	HANDLE	hStdErrW;
	HANDLE	hStdErrR;

public:
	enum FLAG
	{
		STDIN	= 1, 
		STDOUT	= 2, 
		STDERR	= 4,
		STDALL	= 7
	};

public:
// コンストラクタおよびデストラクタ
	Process(UINT u = 0);
	~Process();

// 起動
	BOOL	Create(LPCSTR, STARTUPINFOA* psi = NULL);
	BOOL	CreateAsUser(HANDLE, LPCSTR, STARTUPINFOA* psi = NULL);
	void	Close();
	HANDLE	GetHandle() const;
	HANDLE	GetDupHandle() const;
private:
	STARTUPINFOA SetStartupInfo(STARTUPINFOA*);

public:
// 標準入出力
	BOOL	SetStdIn(LPCSTR);
	BYTE*	GetStdOut(DWORD*, BOOL blnAddNull = TRUE);
	BYTE*	GetStdErr(DWORD*, BOOL blnAddNull = TRUE);
private:
	void	CreateStdIn();
	void	CreateStdOut();
	void	CreateStdErr();
	void	CloseIoHandleAfterCreate();
	void	CloseAllIoHandles();
public:

// 待機
	DWORD	WaitIdle(DWORD);
	DWORD	WaitObject(DWORD);
};


#endif
