
// uTstring.h
// ユーティリティー関数群で使う文字列型
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __TSTRING_H__
#define  __TSTRING_H__


/******************************************************************************/
// 		定義
/******************************************************************************/

#if defined(UNICODE) || defined(_UNICODE)
#undef  UNICODE
#undef  _UNICODE
#define UNICODE
#define _UNICODE
#endif


/******************************************************************************/
// 		インクルード
/******************************************************************************/

#pragma warning( disable : 4312 )
#pragma warning( disable : 4786 )

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <map>

using namespace std;


/******************************************************************************/
// 		型定義
/******************************************************************************/

#ifdef _UNICODE
typedef wstring	_tstring;
#else
typedef string	_tstring;
#endif


/******************************************************************************/
// 		クラス定義
/******************************************************************************/

class tstring : public _tstring
{
public:
// コンストラクタおよびデストラクタ
	tstring();
	tstring(_tstring s);
	tstring(LPCTSTR p);
	~tstring();

// 文字列配列

// 抽出
	tstring Left(int) const;
	tstring Mid(int intPos, int intCount) const;
	tstring Right(int) const;

// 比較
	BOOL IsEqual(const tstring&) const;
	BOOL IsEqualNoCase(const tstring&) const;

// 変換
	int Replace(TCHAR, TCHAR);
	int Replace(LPCTSTR, LPCTSTR);
	void Format(LPCTSTR, ...);
	void TrimLeft(TCHAR);
	void TrimLeft(LPCTSTR pszTarget = TEXT(" \n\r\t"));
	void TrimRight(TCHAR);
	void TrimRight(LPCTSTR pszTarget = TEXT(" \n\r\t"));
	int toInt() const;

private:
	BOOL IsTrimmable(TCHAR, LPCTSTR);
};


/******************************************************************************/
// 		グローバル関数
/******************************************************************************/

const string	Unicode2Sjis(const wstring& str);
const wstring	Sjis2Unicode(const string& str);
BOOL		IsStrEqual(const tstring& str1, const tstring& str2);
BOOL		IsStrEqualNoCase(const tstring& str1, const tstring& str2);
vector<tstring>	SplitStr(const tstring& str, const tstring& strDelimiter);
tstring		Int2Str(int i);
tstring		Uint2Str(unsigned int i);
int		Str2Int(const tstring& s);
int		Hex2Char(TCHAR c);
tstring		Hex2Str(const tstring& strData);
tstring		Str2Hex(const tstring& strData);
tstring		Err2Str(DWORD dwErr);

tstring		ConvertUsingValues(const tstring& strData, const map<TCHAR, tstring>&, TCHAR = TEXT('%'));

#endif