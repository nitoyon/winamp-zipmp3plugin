
// uSocket.h
// �\�P�b�g���������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __SOCKET_H__
#define  __SOCKET_H__

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


/******************************************************************************/
// 		�N���X�錾
/******************************************************************************/

class uSocket
{
private:
	SOCKET soc;

public:
// �R���X�g���N�^����уf�X�g���N�^
	uSocket();
	uSocket(SOCKET);
	~uSocket();

// �擾
	SOCKET GetInstance() const;

// �쐬�p��
	BOOL Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	BOOL Close();
	BOOL CleanClose(UINT = 10);

// �ڑ�
	BOOL Bind(LPCTSTR pszAddress, UINT uiPort);
	BOOL Bind(const SOCKADDR* lpSockAddr, int intSockAddrLen);
	BOOL Listen(int intBackLog);
	BOOL Connect(LPCTSTR pszAddress, UINT uiPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int intSockAddrLen);
	BOOL Shutdown(int intHow);

// ����M
	int Send(const void* lpBuf, int intBufLen, int intFlag = 0);
	int Receive(void* lpBuf, int intBufLen, int intFlags = 0);
	SOCKET Accept(SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

// �ݒ�ύX
	BOOL IOCtl(LONG lCommand, DWORD* lpArgument);
	BOOL SetBlocking(BOOL);

// ��ʊ֐�
	BYTE* ReceiveTillChar(LPSTR, UINT uiTimeout = INFINITE, BOOL blnAddNull = TRUE);
	BYTE* ReceiveLine(UINT uiTimeout = INFINITE, BOOL blnAddNull = TRUE);
};


#endif