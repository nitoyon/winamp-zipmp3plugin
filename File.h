
// File.h
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"


/******************************************************************************/
//		�\���̒�`
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
//		�N���X��`
/******************************************************************************/

class File
{
protected:
// �t�@�C�����
	string		strFilePath ;
	ZipChild*	pZipChild ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	File(){} ;
	virtual ~File(){} ;

// �����擾
	virtual void ReadHeader() = 0 ;
	virtual ULONG GetPlayLength() = 0 ;
} ;

#endif