
// uVersion.cpp
// バージョンを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include  "uVersion.h"
#pragma comment(linker, "/defaultlib:\"Version.lib\"")


/******************************************************************************/
//		ウインドウズのバージョン
/******************************************************************************/
// NT 系かどうかを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL IsNT()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/******************************************************************************/
// メジャーバージョン取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

DWORD GetOsMajorVersion()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwOSVersionInfoSize;
}


/******************************************************************************/
// 実行ファイルのバージョンを取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetFileVersion(const tstring& strPath)
{
	// バージョン情報取得
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strPath.c_str(), &dwHandle);
	if(dwSize == 0)
	{
		return TEXT("");
	}
	PVOID lpData = new BYTE[dwSize];
	GetFileVersionInfo(strPath.c_str(), 0, dwSize, lpData);

	// 解析
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	}* lpCodePage;
	UINT uiLen;
	if(!VerQueryValue(lpData, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpCodePage, &uiLen))
	{
		return TEXT("");
	}
	tstring strBlock;
	strBlock.Format(TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"), lpCodePage[0].wLanguage, lpCodePage[0].wCodePage);
	PVOID pByte = 0;
	VerQueryValue(lpData, (LPTSTR)strBlock.c_str(), &pByte, &uiLen);

	// 文字列設定
	tstring strVersion = (LPCTSTR)pByte;

	delete[] lpData;
	return strVersion;
}
