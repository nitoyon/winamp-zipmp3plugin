
// uRegistry.h
// ���W�X�g�����b�p�[�N���X
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef	__REGISTRY_H__
#define	__REGISTRY_H__

#include  <windows.h>


/******************************************************************************/
//		�N���X�C���^�[�t�F�[�X
/******************************************************************************/

class Registry
{
private:
	HKEY	hkey;

public:
// �R���X�g���N�^����уf�X�g���N�^
	Registry();
	~Registry();

// ���W�X�g������
	BOOL Open(HKEY, PCTSTR, REGSAM sam = KEY_READ | KEY_WRITE);
	BOOL Close();
	static BOOL Create(HKEY, PCTSTR, REGSAM sam = KEY_READ | KEY_WRITE);
	static BOOL Delete(HKEY, PCTSTR);

	BOOL Read(PCTSTR, DWORD*, BYTE*, DWORD*);
	BOOL Write(PCTSTR, DWORD, const BYTE*, DWORD);
	BOOL Clear(PCTSTR);

// ���W�X�g����������
//	static BOOL SetValue(HKEY, PCTSTR, PCTSTR, PVOID, DWORD);
//	static BYTE* GetValue(HKEY, PCTSTR, PCTSTR, PVOID, DWORD);
};


#endif
