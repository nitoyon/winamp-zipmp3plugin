
// File.h
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"


/******************************************************************************/
//		�\���̒�`
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
//		�N���X��`
/******************************************************************************/

class File
{
protected:
// �t�@�C�����
	string		strFilePath ;
	ULONG		ulCentralDir ;
	ULONG		ulFileHead ;
	ZipChildHeader	zipheader ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	File( const string&, ULONG) ;
	~File() ;

// �擾
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