
// RarFile.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __RAR_FILE_H__
#define  __RAR_FILE_H__

#include <windows.h>
#include <vector>
#include <string>

using namespace std;



/******************************************************************************/
//		子ファイル情報の定義
/******************************************************************************/

struct ChildFile
{
	UINT	uiStart;
	UINT	uiEnd;		// 次のアイテムの先頭アドレス
	string	strFileName;
	BOOL	blnCompressed;
};


/******************************************************************************/
//		RAR のファイル情報
/******************************************************************************/

struct RarBlockHeader
{
	USHORT	usHEAD_CRC;
	BYTE	byteHEAD_TYPE;
	USHORT	usHEAD_FLAGS;
	USHORT	usHEAD_SIZE;
} ;

struct RarFileBlockHeader
{
	ULONG	ulPACK_SIZE;
	ULONG	ulUNP_SIZE;
	BYTE	byteHOST_OS;
	ULONG	ulFILE_CRC;
	ULONG	ulFTIME;
	BYTE	byteUNP_VER;
	BYTE	byteMETHOD;
	USHORT	usNAME_SIZE;
	ULONG	ulATTR;
} ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class RarFile
{
public:
	enum Status
	{
		OPEN_ERROR,
		NOT_RAR,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
// Singlton
	static RarFile* pInstance;

// rar の情報
	string	strRarPath;

	ULONG	ulEof ;
	RarBlockHeader blkhdr;
	RarFileBlockHeader fileblkhdr;
	Status	status ;

// ファイル
	FILE*	frar;

// 子供
	vector<ChildFile>	vecChildList ;
	vector< ULONG>	vecHeadMilisec ;

// コンストラクタおよびデストラクタ
private:
	RarFile( const string&) ;
public:
	~RarFile() ;
	static RarFile* GetInstance( const string& s = "") ;

// ヘッダ解析
	BOOL _ReadHeader() ;
private:

// 読み取り関数
//	USHORT makeword( BYTE* p);
//	ULONG  makelong( BYTE* p);

public:
// データ取得
	Status	GetStatus() const{ return status ;}

	string	GetChildName(UINT) const;
	UINT	GetChildCount() const ;
	ULONG	GetChildStartPoint(UINT) const ;
	ULONG	GetChildEndPoint(UINT) const ;
	BOOL	_IsCompressed(UINT) const ;
} ;

#endif