
// uTstring.h
// ユーティリティー関数群で使う文字列型
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __TSTRING_H__
#define  __TSTRING_H__


/******************************************************************************/
// 		インクルード
/******************************************************************************/

#pragma warning( disable : 4312 )
#pragma warning( disable : 4786 )

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

using namespace std;


/******************************************************************************/
// 		型定義
/******************************************************************************/

#ifdef UNICODE
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

	int toInt() const;
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
int		Str2Int(const tstring& s);


#endif