
// uProfile.cpp
// 設定ファイルを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include  "uProfile.h"


/******************************************************************************/
//		書き込み
/******************************************************************************/
// 数字を書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void WriteIniInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const tstring& s)
{
	TCHAR	pszBuf[256];
	wsprintf(pszBuf, TEXT("%u"), ui);
	WritePrivateProfileString(pszSection, pszName, pszBuf, s.c_str()) ;
}


/******************************************************************************/
// 文字列を書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void WriteIniStr(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, strData.c_str(), s.c_str()) ;
}


/******************************************************************************/
// 真偽値を書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void WriteIniBln(LPTSTR pszSection, LPTSTR pszName, BOOL b, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, b ? TEXT("yes") : TEXT("no"), s.c_str());
}


/******************************************************************************/
// メッセージを書き込み
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void WriteIniMsg(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s)
{
	WritePrivateProfileString(pszSection, pszName, Str2IniMsg(strData).c_str(), s.c_str()) ;
}


/******************************************************************************/
//		読み取り
/******************************************************************************/
// 文字列を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 整数を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int ReadIniInt(const tstring& strSection, const tstring& strName, const tstring& s, int intDefault)
{
	return GetPrivateProfileInt(strSection.c_str(), strName.c_str(), intDefault, s.c_str());
}


/******************************************************************************/
// 真偽を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ReadIniBln(const tstring& strSection, const tstring& strName, const tstring& s, BOOL blnDefault)
{
	return IniBln2Bln(ReadIniStr(strSection, strName, s, TEXT("")), blnDefault);
}


/******************************************************************************/
// 時間を読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT ReadIniTime(const tstring& strSection, const tstring& strName, const tstring& s, UINT uiDefault)
{
	return IniTime2Int(ReadIniStr(strSection, strName, s, Int2Str(uiDefault)));
}


/******************************************************************************/
// メッセージを読み取り
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring ReadIniMsg(const tstring& strSection, const tstring& strName, const tstring& s)
{
	return IniMsg2Str(ReadIniStr(strSection, strName, s, TEXT("")));
}


/******************************************************************************/
//		変換
/******************************************************************************/
// 真偽形式に変換
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// メッセージ形式に変換
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring IniMsg2Str(const tstring& strMsg)
{
	tstring strRet;

	BOOL blnEscape = FALSE;
	for(UINT i = 0; i < strMsg.size(); i++)
	{
		// エスケープ文字の処理
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
// メッセージ形式に変換
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 時間に変換
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT IniTime2Int(const tstring& strTime)
{
	// 無限時間
	if(strTime == TEXT("-1"))
	{
		return (UINT)-1;
	}

	// 数字部分を変換
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

	// 単位を変換
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
			uiTime *= 1000;	// s と解釈
		}
	}
	else
	{
		uiTime *= 1000;
	}

	return uiTime;
}


/******************************************************************************/
// 数字を時間に変換
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring Int2IniTime(UINT uiTime)
{
	tstring strRet;

	if(uiTime == INFINITE)
	{
		strRet = TEXT("無限時間");
	}
	else
	{
		uiTime /= 1000;

		// 時間の文字列計算
		TCHAR pszBuf[100];

		if(uiTime / 60 > 0)
		{
			wsprintf(pszBuf, TEXT("%02d"), uiTime % 60);
		}
		else
		{
			wsprintf(pszBuf, TEXT("%d"), uiTime % 60);
		}
		strRet = pszBuf + tstring(TEXT("秒"));
		uiTime /= 60;

		if(uiTime > 0)
		{
			wsprintf(pszBuf, TEXT("%d"), uiTime);
			strRet = pszBuf + tstring(TEXT("分")) + strRet;
			uiTime /= 60;

			if(uiTime > 0)
			{
				wsprintf(pszBuf, TEXT("%d"), uiTime);
				strRet = pszBuf + tstring(TEXT("時間")) + strRet;
			}
		}
	}

	return strRet;
}


/******************************************************************************/
//		セクション
/******************************************************************************/
// 文字列を INI ファイルとして評価
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// セクションリスト取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

vector<tstring> GetSectionList(const tstring& strFileName)
{
	// ini ファイル読み取り
	DWORD dwSize = 256 ;
	PTSTR pszBuf ;
	while( TRUE)
	{
		// バッファ確保して読み取り
		pszBuf = new TCHAR[dwSize] ;
		DWORD dwResult = GetPrivateProfileSectionNames(pszBuf, dwSize, strFileName.c_str()) ;

		// バッファサイズが小さかったとき
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

	// リストを設定
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
// セクション内の文字を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetSectionString(const tstring& strSection, const tstring& strFileName)
{
	// ini ファイル読み取り
	DWORD dwSize = 256 ;
	PTSTR pszBuf ;
	while( TRUE)
	{
		// バッファ確保して読み取り
		pszBuf = new TCHAR[dwSize] ;
		DWORD dwResult = GetPrivateProfileSection(strSection.c_str(), pszBuf, dwSize, strFileName.c_str()) ;

		// バッファサイズが小さかったとき
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

	// リストを設定
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