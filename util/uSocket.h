
// uSocket.h
// ソケットを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __SOCKET_H__
#define  __SOCKET_H__

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


/******************************************************************************/
// 		クラス宣言
/******************************************************************************/

class uSocket
{
private:
	SOCKET soc;

public:
// コンストラクタおよびデストラクタ
	uSocket();
	uSocket(SOCKET);
	~uSocket();

// 取得
	SOCKET GetInstance() const;

// 作成廃棄
	BOOL Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	BOOL Close();
	BOOL CleanClose(UINT = 10);

// 接続
	BOOL Bind(LPCTSTR pszAddress, UINT uiPort);
	BOOL Bind(const SOCKADDR* lpSockAddr, int intSockAddrLen);
	BOOL Listen(int intBackLog);
	BOOL Connect(LPCTSTR pszAddress, UINT uiPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int intSockAddrLen);
	BOOL Shutdown(int intHow);

// 送受信
	int Send(const void* lpBuf, int intBufLen, int intFlag = 0);
	int Receive(void* lpBuf, int intBufLen, int intFlags = 0);
	SOCKET Accept(SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

// 設定変更
	BOOL IOCtl(LONG lCommand, DWORD* lpArgument);
	BOOL SetBlocking(BOOL);

// 上位関数
	BYTE* ReceiveTillChar(LPSTR, UINT uiTimeout = INFINITE, BOOL blnAddNull = TRUE);
	BYTE* ReceiveLine(UINT uiTimeout = INFINITE, BOOL blnAddNull = TRUE);
};


#endif