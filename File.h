
// File.h
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"


/******************************************************************************/
//		構造体定義
/******************************************************************************/

struct ZipChild
{
	USHORT	usVersionMadeBy ;
	USHORT	usVersionNeededToExtract ;
	USHORT	usGeneralPurposeBitFlag;
	USHORT	usCompressionMethod;
	USHORT	usLastModFileTime;
	USHORT	usLastModFileDate;
	ULONG	ulCrc32;
	ULONG	ulCompressedSize;
	ULONG	ulUncompressedSize;
	USHORT	usFilenameLength;
	USHORT	usExtraFieldLength;
	USHORT	usFileCommentLength;
	USHORT	usDiskNumberStart;
	USHORT	usInternalFileAttributes;
	ULONG	ulExternalFileAttributes;
	ULONG	ulRelativeOffsetLocalHeader;
	char*	pszFilename ;
	BYTE*	pbyteExtra ;
	char*	pszComment ;
} ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class File
{
protected:
// ファイル情報
	string		strFilePath ;
	ZipChild*	pZipChild ;

public:
// コンストラクタおよびデストラクタ
	File(){} ;
	virtual ~File(){} ;

// 長さ取得
	virtual void ReadHeader() = 0 ;
	virtual ULONG GetPlayLength() = 0 ;
} ;

#endif