
// uProfile.h
// 設定ファイルを扱うユーティリティー関数群
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __UPROFILE_H__
#define  __UPROFILE_H__


/******************************************************************************/
//		インクルード
/******************************************************************************/

#include  <windows.h>
#include  "uTstring.h"
#include  "uStl.h"


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

// 書き込み
void WriteIniInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const tstring& s);
void WriteIniStr(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s);
void WriteIniBln(LPTSTR pszSection, LPTSTR pszName, BOOL b, const tstring& s);
void WriteIniMsg(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s);

// 読み取り
tstring	ReadIniStr(const tstring&, const tstring&, const tstring& s, const tstring& strDefault);
int	ReadIniInt(const tstring&, const tstring&, const tstring& s, int intDefault);
BOOL	ReadIniBln(const tstring&, const tstring&, const tstring& s, BOOL blnDefault);
UINT	ReadIniTime(const tstring&, const tstring&, const tstring& s, BOOL blnDefault);
tstring	ReadIniMsg(const tstring&, const tstring&, const tstring& s);


// 変換
BOOL	IniBln2Bln(const tstring& str, BOOL blnDefault = FALSE);
tstring	IniMsg2Str(const tstring& strMsg);
tstring	Str2IniMsg(const tstring& strMsg);
tstring	Msg2IniStr(const tstring& strMsg);
UINT	IniTime2Int(const tstring& strTime);

// セクション
map<tstring, tstring> ReadStrIni(const tstring& strData);
vector<tstring> GetSectionList(const tstring& strFileName);
tstring GetSectionString(const tstring& strSection, const tstring& strFileName);

#endif