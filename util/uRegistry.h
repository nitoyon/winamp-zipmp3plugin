
// uRegistry.h
// レジストリラッパークラス
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef	__REGISTRY_H__
#define	__REGISTRY_H__

#include  <windows.h>


/******************************************************************************/
//		クラスインターフェース
/******************************************************************************/

class Registry
{
private:
	HKEY	hkey;

public:
// コンストラクタおよびデストラクタ
	Registry();
	~Registry();

// レジストリ操作
	BOOL Open(HKEY, PCTSTR, REGSAM sam = KEY_READ | KEY_WRITE);
	BOOL Close();
	static BOOL Create(HKEY, PCTSTR, REGSAM sam = KEY_READ | KEY_WRITE);
	static BOOL Delete(HKEY, PCTSTR);

	BOOL Read(PCTSTR, DWORD*, BYTE*, DWORD*);
	BOOL Write(PCTSTR, DWORD, const BYTE*, DWORD);
	BOOL Clear(PCTSTR);

// レジストリ複合操作
//	static BOOL SetValue(HKEY, PCTSTR, PCTSTR, PVOID, DWORD);
//	static BYTE* GetValue(HKEY, PCTSTR, PCTSTR, PVOID, DWORD);
};


#endif
