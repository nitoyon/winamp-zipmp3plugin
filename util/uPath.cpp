
// uPath.cpp
// �p�X���������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include  "uPath.h"


/******************************************************************************/
//		�t�@�C���p�X�֘A
/******************************************************************************/
// ���W���[���̃t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetModuleFileName(HINSTANCE hInstance)
{
	TCHAR pszPath[MAX_PATH + 1];
	GetModuleFileName(hInstance, pszPath, MAX_PATH + 1);

	return tstring(pszPath);
}


/******************************************************************************/
// �t���p�X����f�B���N�g���擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F\ ���܂߂ĕԂ��B
//============================================================================//

tstring GetDirName(const tstring& strPath)
{
	int	intLastYen = 0 ;
	TCHAR	pszFile[MAX_PATH + 1] ;
	TCHAR*	pszPointer = pszFile ;

	lstrcpyn(pszFile, strPath.c_str(), MAX_PATH);

	for(int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if(IsDBCSLeadByte(*pszPointer))
		{
			// �Q�o�C�g�����Ȃ�Q�i��
			i++ ;
			continue ;
		}

		if(*pszPointer == TEXT('\\') || *pszPointer == TEXT('/'))
		{
			intLastYen = i ;
		}
	}

	if(intLastYen > 0)
	{
		return strPath.substr(0, intLastYen + 1) ;     // Yen ���܂߂ĕԂ�
	}
	else
	{
		return TEXT("") ;
	}
}


/******************************************************************************/
// �t���p�X����t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetFileName(const tstring& strPath)
{
	tstring strDirName = GetDirName(strPath) ;
	return strPath.substr(strDirName.size()) ;
}


/******************************************************************************/
// �g���q���擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetExtensionName(const tstring& strPath)
{
	tstring strFileName = GetFileName(strPath);
	UINT uiDotPos = strFileName.find(TEXT('.'));

	if(uiDotPos == tstring::npos || uiDotPos == strFileName.size() - 1)
	{
		return TEXT("");
	}
	else
	{
		return strFileName.substr(uiDotPos + 1);
	}
}


/******************************************************************************/
// �t�@�C�������擾�i�g���q�͏����j
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetBaseName(const tstring& strPath)
{
	tstring strFileName = GetFileName(strPath);
	UINT uiDotPos = strFileName.find(TEXT('.'));

	if(uiDotPos == tstring::npos || uiDotPos == 0)
	{
		return TEXT("");
	}
	else
	{
		return strFileName.substr(0, uiDotPos);
	}
}