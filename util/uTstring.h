
// uTstring.h
// ���[�e�B���e�B�[�֐��Q�Ŏg��������^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __TSTRING_H__
#define  __TSTRING_H__


/******************************************************************************/
// 		��`
/******************************************************************************/

#if defined(UNICODE) || defined(_UNICODE)
#undef  UNICODE
#undef  _UNICODE
#define UNICODE
#define _UNICODE
#endif


/******************************************************************************/
// 		�C���N���[�h
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
// 		�^��`
/******************************************************************************/

#ifdef _UNICODE
typedef wstring	_tstring;
#else
typedef string	_tstring;
#endif


/******************************************************************************/
// 		�N���X��`
/******************************************************************************/

class tstring : public _tstring
{
public:
// �R���X�g���N�^����уf�X�g���N�^
	tstring();
	tstring(_tstring s);
	tstring(LPCTSTR p);
	~tstring();

// ������z��

// ���o
	tstring Left(int) const;
	tstring Mid(int intPos, int intCount) const;
	tstring Right(int) const;

// ��r
	BOOL IsEqual(const tstring&) const;
	BOOL IsEqualNoCase(const tstring&) const;

// �ϊ�
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
// 		�O���[�o���֐�
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