
// File.cpp
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "File.h"
#include "define.h"
#include "util.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File::File( const string& s, ULONG u)
: strFilePath( s), ulCentralDir( u)
{
	ZeroMemory( &zipheader, sizeof( ZipChildHeader)) ;
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File::~File()
{
	if( zipheader.pbyteExtra)
	{
		delete[] zipheader.pbyteExtra ;
	}
}



/******************************************************************************/
//		�擾
/******************************************************************************/
// �w�b�_���擾
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL File::ReadHeader()
{
	FILE* fzip = fopen( strFilePath.c_str(), "rb") ;
	if( !fzip)
	{
		return FALSE ;
	}
	if( fseek( fzip, ulCentralDir, SEEK_SET) != 0)
	{
		return FALSE ;
	}

	// �w�b�_�̃`�F�b�N
	if( fgetc( fzip) == 0x50 && 
	    fgetc( fzip) == 0x4b && 
	    fgetc( fzip) == 0x01 && 
	    fgetc( fzip) == 0x02)
	{
		// no problem
	}
	else
	{
		return FALSE ;
	}

	// �ǂ݂Ƃ�J�n
	BYTE byte[ CENTRAL_DIR_SIZE] ;
	fread( byte, sizeof( BYTE), CENTRAL_DIR_SIZE, fzip) ;

	// �w�b�_�̓ǂ݂Ƃ�
	zipheader.usVersionMadeBy 		= makeword( &byte[ C_VERSION_MADE_BY_0]) ;
	zipheader.usVersionNeededToExtract	= makeword( &byte[ C_VERSION_NEEDED_TO_EXTRACT_0]) ;
	zipheader.usGeneralPurposeBitFlag	= makeword( &byte[ C_GENERAL_PURPOSE_BIT_FLAG]) ;
	zipheader.usCompressionMethod		= makeword( &byte[ C_COMPRESSION_METHOD]) ;
	zipheader.usLastModFileTime		= makeword( &byte[ C_LAST_MOD_FILE_TIME]) ;
	zipheader.usLastModFileDate		= makeword( &byte[ C_LAST_MOD_FILE_DATE]) ;
	zipheader.ulCrc32			= makelong( &byte[ C_CRC32]) ;
	zipheader.ulCompressedSize		= makelong( &byte[ C_COMPRESSED_SIZE]) ;
	zipheader.ulUncompressedSize		= makelong( &byte[ C_UNCOMPRESSED_SIZE]) ;
	zipheader.usFilenameLength		= makeword( &byte[ C_FILENAME_LENGTH]) ;
	zipheader.usExtraFieldLength		= makeword( &byte[ C_EXTRA_FIELD_LENGTH]) ;
	zipheader.usFileCommentLength		= makeword( &byte[ C_FILE_COMMENT_LENGTH]) ;
	zipheader.usDiskNumberStart		= makeword( &byte[ C_DISK_NUMBER_START]) ;
	zipheader.usInternalFileAttributes	= makeword( &byte[ C_INTERNAL_FILE_ATTRIBUTES]) ;
	zipheader.ulExternalFileAttributes	= makelong( &byte[ C_EXTERNAL_FILE_ATTRIBUTES]) ;
	zipheader.ulRelativeOffsetLocalHeader	= makelong( &byte[ C_RELATIVE_OFFSET_LOCAL_HEADER]) ;

	// �t�@�C�����ǂ݂Ƃ�
	if( zipheader.usFilenameLength > 0)
	{
		char* pszFilename = new char[ zipheader.usFilenameLength + 1] ;
		fread( pszFilename, sizeof( char), zipheader.usFilenameLength, fzip) ;
		pszFilename[ zipheader.usFilenameLength] = '\0' ;
		zipheader.strFilename = pszFilename ;
	}

	// �g���̈�
	if( zipheader.usExtraFieldLength > 0)
	{
		BYTE* pbyte = new BYTE[ zipheader.usExtraFieldLength] ;
		fread( pbyte, sizeof( BYTE), zipheader.usExtraFieldLength, fzip) ;
		zipheader.pbyteExtra = pbyte ;
	}

	// �R�����g
	if( zipheader.usFileCommentLength > 0)
	{
		char* pszComment = new char[ zipheader.usFileCommentLength + 1] ;
		fread( pszComment, sizeof( char), zipheader.usFileCommentLength, fzip) ;
		pszComment[ zipheader.usFileCommentLength] = '\0' ;
		zipheader.strComment = pszComment ;
	}

	// ���[�J���w�b�_
	fseek( fzip, zipheader.ulRelativeOffsetLocalHeader, SEEK_SET) ;
	if( fgetc( fzip) == 0x50 && 
	    fgetc( fzip) == 0x4b && 
	    fgetc( fzip) == 0x03 && 
	    fgetc( fzip) == 0x04)
	{
		// no problem
	}
	else
	{
		return FALSE ;
	}
	fread( byte, sizeof( BYTE), LOCAL_HEADER_SIZE, fzip) ;
	USHORT usFilenameLength2	= makeword( &byte[ L_FILENAME_LENGTH]) ;
	USHORT	usExtraFieldLength2	= makeword( &byte[ L_EXTRA_FIELD_LENGTH]) ;
	ulFileHead = zipheader.ulRelativeOffsetLocalHeader + LOCAL_HEADER_SIZE + usFilenameLength2 + usExtraFieldLength2 ;

	fclose( fzip) ;
	return TRUE ;
}


/******************************************************************************/
// Central Directory �̃T�C�Y
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG File::GetCentralDirSize() const
{
	return 4
	     + CENTRAL_DIR_SIZE
	     + zipheader.usFilenameLength
	     + zipheader.usExtraFieldLength
	     + zipheader.usFileCommentLength ;
}


/******************************************************************************/
// �Đ����Ԏ擾
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�f�t�H���g�͂O�~���b�B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG File::GetPlayLength()
{
	return 0 ;
}

