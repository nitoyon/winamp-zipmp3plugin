
// uProcess.cpp
// �v���Z�X�̃��b�v�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�FUNICODE ������ɂ͔�Ή��B
//============================================================================//

#include "uProcess.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  CLOSE_HANDLE(HDL)	if(HDL){CloseHandle(HDL);HDL = NULL;}


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Process::~Process()
{
	Close();
}


/******************************************************************************/
//		�N��
/******************************************************************************/
// �v���Z�X�쐬
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Process::Create(LPCSTR pszCmdLine, STARTUPINFOA* psi)
{
	STARTUPINFOA si = SetStartupInfo(psi);
	PROCESS_INFORMATION	pi;

	// �v���Z�X�쐬
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
// �v���Z�X�쐬
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Process::CreateAsUser(HANDLE hToken, LPCSTR pszCmdLine, STARTUPINFOA* psi)
{
	STARTUPINFOA si = SetStartupInfo(psi);
	PROCESS_INFORMATION	pi;

	// �v���Z�X�쐬
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
// �n���h�������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �v���Z�X�̃n���h����Ԃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

HANDLE Process::GetHandle() const
{
	return hProcess;
}


/******************************************************************************/
// �v���Z�X�̃n���h����Ԃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�n���h���𕡐����ĕԂ��B
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
// STARTUPINFO �̐ݒ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

	// �W�����͐ݒ�
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
//		�W�����o��
/******************************************************************************/
// �W�����͂ɕ�����𗬂�����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�������񂾂�W�����͂����B
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
// �W���o�͂̓ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BYTE* Process::GetStdOut(DWORD* pdwSize, BOOL blnAddNull)
{
	// �T�C�Y�̓ǂ݂Ƃ�
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

	// �T�C�Y�m�ہ{���e�ǂ݂Ƃ�
	BYTE* pByte = new BYTE[dwStdOut + (blnAddNull ? 1 : 0)];
	if(ReadFile(hStdOutR, pByte, dwStdOut, pdwSize, NULL))
	{
		// ������ NULL ��ǉ�
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
// �W���G���[�ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BYTE* Process::GetStdErr(DWORD* pdwSize, BOOL blnAddNull)
{
	// �T�C�Y�̓ǂ݂Ƃ�
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

	// �T�C�Y�m�ہ{���e�ǂ݂Ƃ�
	BYTE* pByte = new BYTE[dwStdErr + (blnAddNull ? 1 : 0)];
	if(ReadFile(hStdErrR, pByte, dwStdErr, pdwSize, NULL))
	{
		// ������ NULL ��ǉ�
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
// �W�����͂̃n���h��������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Process::CreateStdIn()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength		= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle	= TRUE;

	CreatePipe(&hStdInR, &hStdInW, &sa, 0);

	// hStdInW ���q�v���Z�X�Ɍp������Ȃ��悤�ɂ���
	HANDLE hTmp = hStdInW;
	DuplicateHandle(GetCurrentProcess(), hTmp, GetCurrentProcess(), &hStdInW, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hTmp);
}


/******************************************************************************/
// �W���o�͂�����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �W���G���[������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �v���Z�X�쐬��ɕs�v�ȓǂݏ����n���h�������
//============================================================================//
// �T�v�F�q�v���Z�X�����p����n���h�������B
// �⑫�F�Ȃ��B
//============================================================================//

void Process::CloseIoHandleAfterCreate()
{
	CLOSE_HANDLE(hStdInR);
	CLOSE_HANDLE(hStdOutW);
	CLOSE_HANDLE(hStdErrW);
}


/******************************************************************************/
// �S�Ă̓ǂݏ����n���h�������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
//		�ҋ@
/******************************************************************************/
// �v���Z�X���A�C�h����ԂɂȂ�̂�ҋ@
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �v���Z�X�I����ҋ@
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DWORD Process::WaitObject(DWORD dwTime)
{
	if(hProcess)
	{
		return WaitForSingleObject(hProcess, dwTime);
	}
	return 0;
}

