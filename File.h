
// File.h
//============================================================================//
// 更新：02/12/28(土)
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
	string		strZipPath ;
	ULONG		ulCentralDir ;
	ULONG		ulFileHead ;
	ZipChildHeader	zipheader ;

public:
// コンストラクタおよびデストラクタ
	File( const string&, ULONG) ;
	~File() ;

// ファイル情報取得
	string GetFilePath() const{ return zipheader.strFilename ;} ;
	string GetZipPath() const{ return strZipPath ;}
	ULONG GetCentralDirSize() const ;
	string GetFileName() const ;
private :
	string GetFileDir() const ;

// ZIP の子情報取得
public :
	ZipChildHeader GetHeader() const{ return zipheader ;}
	BOOL IsCompressed() const{ return zipheader.ulCompressedSize != zipheader.ulUncompressedSize ;}
	virtual ULONG GetPlayLength() ;
	virtual BOOL ReadHeader() ;
	ULONG GetCentralDir() const{ return ulCentralDir ;}
	ULONG GetFileHead() const{ return ulFileHead ;}

// 表示名取得
	string GetDisplayStr( const string&) ;
	virtual BOOL HasID3Tag() const{ return FALSE ;}
protected:
	virtual string GetVariable( const string&) ;
} ;

#endif