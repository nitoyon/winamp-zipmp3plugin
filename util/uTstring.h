
// uTstring.h
// ���[�e�B���e�B�[�֐��Q�Ŏg��������^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __TSTRING_H__
#define  __TSTRING_H__


/******************************************************************************/
// 		�C���N���[�h
/******************************************************************************/

#pragma warning( disable : 4312 )
#pragma warning( disable : 4786 )

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

using namespace std;


/******************************************************************************/
// 		�^��`
/******************************************************************************/

#ifdef UNICODE
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

	int toInt() const;
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
int		Str2Int(const tstring& s);


#endif