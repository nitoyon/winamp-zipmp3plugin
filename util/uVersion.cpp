
// uVersion.cpp
// �o�[�W�������������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include  "uVersion.h"
#pragma comment(linker, "/defaultlib:\"Version.lib\"")


/******************************************************************************/
//		�E�C���h�E�Y�̃o�[�W����
/******************************************************************************/
// NT �n���ǂ������擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL IsNT()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/******************************************************************************/
// ���W���[�o�[�W�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

DWORD GetOsMajorVersion()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	return ovi.dwOSVersionInfoSize;
}


/******************************************************************************/
// ���s�t�@�C���̃o�[�W�������擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetFileVersion(const tstring& strPath)
{
	// �o�[�W�������擾
	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize(strPath.c_str(), &dwHandle);
	if(dwSize == 0)
	{
		return TEXT("");
	}
	PVOID lpData = new BYTE[dwSize];
	GetFileVersionInfo(strPath.c_str(), 0, dwSize, lpData);

	// ���
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

	// ������ݒ�
	tstring strVersion = (LPCTSTR)pByte;

	delete[] lpData;
	return strVersion;
}
