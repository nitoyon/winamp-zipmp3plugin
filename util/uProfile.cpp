
// uProfile.cpp
// �ݒ�t�@�C�����������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include  "uProfile.h"


/******************************************************************************/
//		��������
/******************************************************************************/
// ��������������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void WriteIniInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const tstring& s)
{
	TCHAR	pszBuf[256];
	wsprintf(pszBuf, TEXT("%u"), ui);
	WritePrivateProfileString(pszSection, pszName, pszBuf, s.c_str()) ;
}


/******************************************************************************/
// ���������������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void WriteIniStr(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, strData.c_str(), s.c_str()) ;
}


/******************************************************************************/
// �^�U�l����������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void WriteIniBln(LPTSTR pszSection, LPTSTR pszName, BOOL b, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, b ? TEXT("yes") : TEXT("no"), s.c_str());
}


/******************************************************************************/
// ���b�Z�[�W����������
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void WriteIniMsg(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, Str2IniMsg(strData).c_str(), s.c_str()) ;
}


/******************************************************************************/
//		�ǂݎ��
/******************************************************************************/
// �������ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring ReadIniStr(const tstring& strSection, const tstring& strName, const tstring& s, const tstring& strDefault)
{
	DWORD dwSize = 256 ;
	TCHAR* pszBuf ;
	while(1)
	{
		pszBuf = new TCHAR[dwSize] ;

		DWORD dw = GetPrivateProfileString(strSection.c_str(), strName.c_str(), strDefault.c_str(), pszBuf, dwSize, s.c_str()) ;
		if(dw == dwSize - 1 || dw == dwSize - 2)
		{
			dwSize *= 2 ;
			delete[] pszBuf ;
			continue ;
		}

		break ;
	}

	tstring str = pszBuf ;
	delete[] pszBuf ;
	return str ;
}


/******************************************************************************/
// ������ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ReadIniInt(const tstring& strSection, const tstring& strName, const tstring& s, int intDefault)
{
	return GetPrivateProfileInt(strSection.c_str(), strName.c_str(), intDefault, s.c_str());
}


/******************************************************************************/
// �^�U��ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ReadIniBln(const tstring& strSection, const tstring& strName, const tstring& s, BOOL blnDefault)
{
	return IniBln2Bln(ReadIniStr(strSection, strName, s, TEXT("")), blnDefault);
}


/******************************************************************************/
// ���Ԃ�ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT ReadIniTime(const tstring& strSection, const tstring& strName, const tstring& s, UINT uiDefault)
{
	return IniTime2Int(ReadIniStr(strSection, strName, s, Int2Str(uiDefault)));
}


/******************************************************************************/
// ���b�Z�[�W��ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring ReadIniMsg(const tstring& strSection, const tstring& strName, const tstring& s)
{
	return IniMsg2Str(ReadIniStr(strSection, strName, s, TEXT("")));
}


/******************************************************************************/
//		�ϊ�
/******************************************************************************/
// �^�U�`���ɕϊ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL IniBln2Bln(const tstring& str, BOOL blnDefault)
{
	if(str.IsEqualNoCase(TEXT("yes")) || str.IsEqualNoCase(TEXT("true")))
	{
		return TRUE;
	}
	if(str.IsEqualNoCase(TEXT("no")) || str.IsEqualNoCase(TEXT("false")))
	{
		return FALSE;
	}

	return blnDefault;

}


/******************************************************************************/
// ���b�Z�[�W�`���ɕϊ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring IniMsg2Str(const tstring& strMsg)
{
	tstring strRet;

	BOOL blnEscape = FALSE;
	for(UINT i = 0; i < strMsg.size(); i++)
	{
		// �G�X�P�[�v�����̏���
		if(blnEscape)
		{
			if(strMsg[i] == TEXT('\\'))
			{
				strRet += TEXT('\\');
			}
			else if(strMsg[i] == TEXT('n'))
			{
				strRet += TEXT('\n');
			}
			else if(strMsg[i] == TEXT('t'))
			{
				strRet += TEXT('\t');
			}

			blnEscape = FALSE;
			continue;
		}

		if(strMsg[i] == TEXT('\\'))
		{
			blnEscape = TRUE;
		}
		else
		{
			strRet += strMsg[i];
		}
	}

	return strRet;
}


/******************************************************************************/
// ���b�Z�[�W�`���ɕϊ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring Str2IniMsg(const tstring& strMsg)
{
	tstring strRet;

	for(UINT i = 0; i < strMsg.size(); i++)
	{
		if(strMsg[i] == TEXT('\n'))
		{
			strRet += TEXT("\\n");
		}
		else if(strMsg[i] == TEXT('\t'))
		{
			strRet += TEXT("\\t");
		}
		else if(strMsg[i] == TEXT('\\'))
		{
			strRet += TEXT("\\\\");
		}
		else
		{
			strRet += strMsg[i];
		}
	}

	return strRet;
}


/******************************************************************************/
// ���Ԃɕϊ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT IniTime2Int(const tstring& strTime)
{
	// ��������
	if(strTime == TEXT("-1"))
	{
		return (UINT)-1;
	}

	// ����������ϊ�
	UINT uiTime = 0;
	int i = 0;
	while(TEXT('0') <= strTime[i] && strTime[i] <= TEXT('9'))
	{
		uiTime *= 10;
		uiTime += strTime[i] - TEXT('0');
	
		i++;
		if(i >= strTime.size())
		{
			break;
		}
	}

	// �P�ʂ�ϊ�
	if(i < strTime.size())
	{
		tstring strUnit = strTime.substr(i);
		if(strUnit == TEXT("ms"))
		{
		}
		else if(strUnit == TEXT("s"))
		{
			uiTime *= 1000;
		}
		else if(strUnit == TEXT("m"))
		{
			uiTime *= 1000 * 60;
		}
		else if(strUnit == TEXT("h"))
		{
			uiTime *= 1000 * 60 * 60;
		}
		else if(strUnit == TEXT("d"))
		{
			uiTime *= 1000 * 60 * 60 * 24;
		}
		else
		{
			uiTime *= 1000;	// s �Ɖ���
		}
	}
	else
	{
		uiTime *= 1000;
	}

	return uiTime;
}


/******************************************************************************/
// ���������Ԃɕϊ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring Int2IniTime(UINT uiTime)
{
	tstring strRet;

	if(uiTime == INFINITE)
	{
		strRet = TEXT("��������");
	}
	else
	{
		uiTime /= 1000;

		// ���Ԃ̕�����v�Z
		TCHAR pszBuf[100];

		if(uiTime / 60 > 0)
		{
			wsprintf(pszBuf, TEXT("%02d"), uiTime % 60);
		}
		else
		{
			wsprintf(pszBuf, TEXT("%d"), uiTime % 60);
		}
		strRet = pszBuf + tstring(TEXT("�b"));
		uiTime /= 60;

		if(uiTime > 0)
		{
			wsprintf(pszBuf, TEXT("%d"), uiTime);
			strRet = pszBuf + tstring(TEXT("��")) + strRet;
			uiTime /= 60;

			if(uiTime > 0)
			{
				wsprintf(pszBuf, TEXT("%d"), uiTime);
				strRet = pszBuf + tstring(TEXT("����")) + strRet;
			}
		}
	}

	return strRet;
}


/******************************************************************************/
//		�Z�N�V����
/******************************************************************************/
// ������� INI �t�@�C���Ƃ��ĕ]��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

map<tstring, tstring> ReadStrIni(const tstring& strData)
{
	vector<tstring> vecLine = SplitStr(strData, TEXT("\r\n"));
	map<tstring, tstring> mapValue;

	for(int i = 0; i < vecLine.size(); i++)
	{
		tstring str = vecLine[i];

		if(str.size() == 0 || str[0] == TEXT('#') || str[0] == TEXT(';'))
		{
			continue;
		}

		UINT uiEqual = str.find(TEXT('='));
		if(uiEqual != string::npos)
		{
			mapValue[str.substr(0, uiEqual)] = str.substr(uiEqual + 1);
		}
	}

	return mapValue;
}


/******************************************************************************/
// �Z�N�V�������X�g�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

vector<tstring> GetSectionList(const tstring& strFileName)
{
	// ini �t�@�C���ǂݎ��
	DWORD dwSize = 256 ;
	PTSTR pszBuf ;
	while( TRUE)
	{
		// �o�b�t�@�m�ۂ��ēǂݎ��
		pszBuf = new TCHAR[dwSize] ;
		DWORD dwResult = GetPrivateProfileSectionNames(pszBuf, dwSize, strFileName.c_str()) ;

		// �o�b�t�@�T�C�Y�������������Ƃ�
		if( dwResult == dwSize - 2)
		{
			dwSize *= 2 ;
			delete[] pszBuf ;
			continue ;
		}
		else
		{
			break ;
		}
	}

	// ���X�g��ݒ�
	vector<tstring> vecList;
	PTSTR pszPointer = pszBuf;
	while(*pszPointer != TEXT('\0'))
	{
		vecList.push_back(pszPointer);
		pszPointer += lstrlen(pszPointer) + 1;
	}

	return vecList;
}


/******************************************************************************/
// �Z�N�V�������̕������擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

tstring GetSectionString(const tstring& strSection, const tstring& strFileName)
{
	// ini �t�@�C���ǂݎ��
	DWORD dwSize = 256 ;
	PTSTR pszBuf ;
	while( TRUE)
	{
		// �o�b�t�@�m�ۂ��ēǂݎ��
		pszBuf = new TCHAR[dwSize] ;
		DWORD dwResult = GetPrivateProfileSection(strSection.c_str(), pszBuf, dwSize, strFileName.c_str()) ;

		// �o�b�t�@�T�C�Y�������������Ƃ�
		if( dwResult == dwSize - 2)
		{
			dwSize *= 2 ;
			delete[] pszBuf ;
			continue ;
		}
		else
		{
			break ;
		}
	}

	// ���X�g��ݒ�
	PTSTR	p = pszBuf;
	tstring s = TEXT("");
	while(*p)
	{
		if(p[0] == TEXT('#') || p[0] == TEXT(';'))
		{
			p += _tcslen(p) + 1;
			continue;
		}

		if(s != TEXT(""))
		{
			s += TEXT("\n");
		}
		s += p;
		p += lstrlen(p) + 1;
	}
	delete[] pszBuf;
	return s;
}