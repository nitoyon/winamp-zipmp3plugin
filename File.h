
// File.h
//============================================================================//
// �X�V�F02/12/28(�y)
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
	string		strZipPath ;
	ULONG		ulCentralDir ;
	ULONG		ulFileHead ;
	ZipChildHeader	zipheader ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	File( const string&, ULONG) ;
	~File() ;

// �t�@�C�����擾
	string GetFilePath() const{ return zipheader.strFilename ;} ;
	string GetZipPath() const{ return strZipPath ;}
	ULONG GetCentralDirSize() const ;
	string GetFileName() const ;
private :
	string GetFileDir() const ;

// ZIP �̎q���擾
public :
	ZipChildHeader GetHeader() const{ return zipheader ;}
	BOOL IsCompressed() const{ return zipheader.ulCompressedSize != zipheader.ulUncompressedSize ;}
	virtual ULONG GetPlayLength() ;
	virtual BOOL ReadHeader() ;
	ULONG GetCentralDir() const{ return ulCentralDir ;}
	ULONG GetFileHead() const{ return ulFileHead ;}

// �\�����擾
	string GetDisplayStr( const string&) ;
	virtual BOOL HasID3Tag() const{ return FALSE ;}
protected:
	virtual string GetVariable( const string&) ;
} ;

#endif