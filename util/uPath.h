
// uPath.h
// �p�X���������[�e�B���e�B�[�֐��Q
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __PATH_H__
#define  __PATH_H__

#include  "uTstring.h"
#include  <windows.h>


/******************************************************************************/
// 		�v���g�^�C�v�錾
/******************************************************************************/

tstring GetModuleFileName(HINSTANCE hInstance);
tstring GetDirName(const tstring& strPath);
tstring GetFileName(const tstring& strPath);
tstring GetExtensionName(const tstring& strPath);
tstring GetBaseName(const tstring& strPath);
BOOL IsFullPath(const tstring& strPath);

tstring ChangeExtension(const tstring& strPath, const tstring& strNewExtension);
tstring GetIniFileName(HINSTANCE hInstance);

#endif