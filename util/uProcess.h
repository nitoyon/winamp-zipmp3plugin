
// uProcess.h
// �v���Z�X�̃��b�v�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�FUNICODE ������ɂ͔�Ή��B
//============================================================================//

#ifndef	__PROCESS_H__
#define	__PROCESS_H__

#include <windows.h>


/******************************************************************************/
//		�v���g�^�C�v�錾
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
// �R���X�g���N�^����уf�X�g���N�^
	Process(UINT u = 0);
	~Process();

// �N��
	BOOL	Create(LPCSTR, STARTUPINFOA* psi = NULL);
	BOOL	CreateAsUser(HANDLE, LPCSTR, STARTUPINFOA* psi = NULL);
	void	Close();
	HANDLE	GetHandle() const;
	HANDLE	GetDupHandle() const;
private:
	STARTUPINFOA SetStartupInfo(STARTUPINFOA*);

public:
// �W�����o��
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

// �ҋ@
	DWORD	WaitIdle(DWORD);
	DWORD	WaitObject(DWORD);
};


#endif
