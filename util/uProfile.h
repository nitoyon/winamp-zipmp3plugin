
// uProfile.h
// �ݒ�t�@�C�����������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __UPROFILE_H__
#define  __UPROFILE_H__


/******************************************************************************/
//		�C���N���[�h
/******************************************************************************/

#include  <windows.h>
#include  "uTstring.h"
#include  "uStl.h"


/******************************************************************************/
//		�v���g�^�C�v�錾
/******************************************************************************/

// ��������
void WriteIniInt(LPTSTR pszSection, LPTSTR pszName, UINT ui, const tstring& s);
void WriteIniStr(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s);
void WriteIniBln(LPTSTR pszSection, LPTSTR pszName, BOOL b, const tstring& s);
void WriteIniMsg(LPTSTR pszSection, LPTSTR pszName, const tstring& strData, const tstring& s);

// �ǂݎ��
tstring	ReadIniStr(const tstring&, const tstring&, const tstring& s, const tstring& strDefault);
int	ReadIniInt(const tstring&, const tstring&, const tstring& s, int intDefault);
BOOL	ReadIniBln(const tstring&, const tstring&, const tstring& s, BOOL blnDefault);
UINT	ReadIniTime(const tstring&, const tstring&, const tstring& s, BOOL blnDefault);
tstring	ReadIniMsg(const tstring&, const tstring&, const tstring& s);


// �ϊ�
BOOL	IniBln2Bln(const tstring& str, BOOL blnDefault = FALSE);
tstring	IniMsg2Str(const tstring& strMsg);
tstring	Str2IniMsg(const tstring& strMsg);
tstring	Msg2IniStr(const tstring& strMsg);
UINT	IniTime2Int(const tstring& strTime);

// �Z�N�V����
map<tstring, tstring> ReadStrIni(const tstring& strData);
vector<tstring> GetSectionList(const tstring& strFileName);
tstring GetSectionString(const tstring& strSection, const tstring& strFileName);

#endif