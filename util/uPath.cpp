
// uPath.cpp
// パスを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include  "uPath.h"


/******************************************************************************/
//		ファイルパス関連
/******************************************************************************/
// モジュールのファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetModuleFileName(HINSTANCE hInstance)
{
	TCHAR pszPath[MAX_PATH + 1];
	GetModuleFileName(hInstance, pszPath, MAX_PATH + 1);

	return tstring(pszPath);
}


/******************************************************************************/
// フルパスからディレクトリ取得
//============================================================================//
// 概要：なし。
// 補足：\ も含めて返す。
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
			// ２バイト文字なら２進む
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
		return strPath.substr(0, intLastYen + 1) ;     // Yen も含めて返す
	}
	else
	{
		return TEXT("") ;
	}
}


/******************************************************************************/
// フルパスからファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

tstring GetFileName(const tstring& strPath)
{
	tstring strDirName = GetDirName(strPath) ;
	return strPath.substr(strDirName.size()) ;
}


/******************************************************************************/
// 拡張子を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ファイル名を取得（拡張子は除く）
//============================================================================//
// 概要：なし。
// 補足：なし。
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