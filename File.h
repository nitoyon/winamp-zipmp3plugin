
// File.h
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"


/******************************************************************************/
//		構造体定義
/******************************************************************************/

struct ZipChildHeader
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
	string	strFilename ;
	BYTE*	pbyteExtra ;
	string	strComment ;
} ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class File
{
protected:
// ファイル情報
	string		strFilePath ;
	ULONG		ulCentralDir ;
	ULONG		ulFileHead ;
	ZipChildHeader	zipheader ;

public:
// コンストラクタおよびデストラクタ
	File( const string&, ULONG) ;
	~File() ;

// 取得
	string GetFilePath() const{ return strFilePath ;}
	ULONG GetCentralDir() const{ return ulCentralDir ;}
	ULONG GetFileHead() const{ return ulFileHead ;}

	ZipChildHeader GetHeader() const{ return zipheader ;}
	BOOL IsCompressed() const{ return zipheader.ulCompressedSize != zipheader.ulUncompressedSize ;}
	string GetFileName() const{ return string( zipheader.strFilename) ;}

	ULONG GetCentralDirSize() const ;
	virtual BOOL ReadHeader() ;
	virtual ULONG GetPlayLength() ;
} ;

#endif