
// ZipFile.cpp
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ZipFile.h"
#include "Mp3File.h"
#include "util.h"
#include "define.h"


#define  BUF_SIZE		4096


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::ZipFile( const string& s, ULONG u)
: ulEof( u)
{
	strFilePath = s ;
	pZipChild = NULL ;

	if( ulEof == 0)
	{
		FILE* fzip = fopen( s.c_str(), "rb") ;
		if( fzip)
		{
			if( fseek( fzip, 0, SEEK_END) == 0)
			{
				ulEof = ftell( fzip) ;
			}

			fclose( fzip) ;
		}
	}
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::~ZipFile() 
{
}


/******************************************************************************/
//		�w�b�_���
/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void ZipFile::ReadHeader()
{
	FILE* fzip = fopen( strFilePath.c_str(), "rb") ;
	if( fzip)
	{
		if( fgetc( fzip) == 0x50
		 && fgetc( fzip) == 0x4b
		 && fgetc( fzip) == 0x03
		 && fgetc( fzip) == 0x04)
		{
			// ok
		}
		else
		{
			return ;
		}


		// �ǂ݂Ƃ�
		ReadEndCentralDirRec( fzip) ;

		fseek( fzip, ulOffsetStartCentralDir, SEEK_SET) ;
		for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
		{
			ReadCentralDirectory( fzip) ;
		}

		fclose( fzip) ;
	}

	vecChildFile.assign( vecZipChild.size(), NULL) ;
	for( int i = 0; i < vecZipChild.size(); i++)
	{
		vecChildFile[ i] = NULL ;
	}
}


/******************************************************************************/
// End Central Directory record �̓ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/21(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::ReadEndCentralDirRec( FILE* fzip)
{
	if( !GetEndCentralDirRecPos( fzip))
	{
		return FALSE ;
	}

	if( fseek( fzip, ulEndCentralDirRec, SEEK_SET) == 0)
	{
		BYTE byte[ END_OF_CENTRAL_SIZE] ;
		fread( byte, sizeof( BYTE), END_OF_CENTRAL_SIZE, fzip) ;

		usNumOfThisDisk 		= makeword( &byte[ NUMBER_THIS_DISK]) ;
		usNumDiskWithStartCentralDir	= makeword( &byte[ NUM_DISK_WITH_START_CENTRAL_DIR]) ;
		usNumEntriesCentrlDirThisDisk	= makeword( &byte[ NUM_ENTRIES_CENTRL_DIR_THS_DISK]) ;
		usTotalEntriesCentralDir	= makeword( &byte[ TOTAL_ENTRIES_CENTRAL_DIR]) ;
		ulSizeCentralDir		= makelong( &byte[ SIZE_CENTRAL_DIRECTORY]) ;
		ulOffsetStartCentralDir		= makelong( &byte[ OFFSET_START_CENTRAL_DIRECTORY]) ;
		usZipfileCommentLength		= makeword( &byte[ ZIPFILE_COMMENT_LENGTH]) ;
	}
	return TRUE ;
}


/******************************************************************************/
// End Central Directory record
//============================================================================//
// �X�V�F02/12/21(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::GetEndCentralDirRecPos( FILE* fzip)
{
	// �o�b�t�@�m��
	char psBuf[ BUF_SIZE + 3] ;
	psBuf[ BUF_SIZE    ] = '\0' ;
	psBuf[ BUF_SIZE + 1] = '\0' ;
	psBuf[ BUF_SIZE + 2] = '\0' ;

	ULONG ulPos = ulEof ;
	ULONG ulPrevPos ;

	while( TRUE)
	{
		// �ǂݍ��݈ʒu�ݒ�
		ulPrevPos = ulPos ;
		ulPos -= BUF_SIZE ;
		if( ulPos > ulPrevPos)
		{
			ulPos = 0 ;
		}

		if( fseek( fzip, ulPos, SEEK_SET) == 0)
		{
			fread( psBuf, sizeof(char), ulPrevPos - ulPos, fzip) ;
	
			// �w��o�C�g�ǂ݂Ƃ�
			for( int j = 0; j < ulPrevPos - ulPos; j++)
			{
				if( psBuf[ j] == 0x50 && 
				    psBuf[ j + 1] == 0x4b && 
				    psBuf[ j + 2] == 0x05 && 
				    psBuf[ j + 3] == 0x06)
				{
					ulEndCentralDirRec = ulPos + j ;
					return TRUE ;
				}
			}

			psBuf[ BUF_SIZE + 0] = psBuf[ 0] ;
			psBuf[ BUF_SIZE + 1] = psBuf[ 1] ;
			psBuf[ BUF_SIZE + 2] = psBuf[ 2] ;
		}

		if( ulPos == 0 || ulPos < ulEof - MAX_END_OF_CENTRAL)
		{
			break ;
		}
	}

	ulEndCentralDirRec = FALSE ;
	return FALSE ;
}


/******************************************************************************/
// Central Directory�̓ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::ReadCentralDirectory( FILE* fzip)
{
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
	ZipChild* pzc = new ZipChild ;
	ZeroMemory( pzc, sizeof( ZipChild)) ;
	pzc->usVersionMadeBy 			= makeword( &byte[ C_VERSION_MADE_BY_0]) ;
	pzc->usVersionNeededToExtract		= makeword( &byte[ C_VERSION_NEEDED_TO_EXTRACT_0]) ;
	pzc->usGeneralPurposeBitFlag		= makeword( &byte[ C_GENERAL_PURPOSE_BIT_FLAG]) ;
	pzc->usCompressionMethod		= makeword( &byte[ C_COMPRESSION_METHOD]) ;
	pzc->usLastModFileTime			= makeword( &byte[ C_LAST_MOD_FILE_TIME]) ;
	pzc->usLastModFileDate			= makeword( &byte[ C_LAST_MOD_FILE_DATE]) ;
	pzc->ulCrc32				= makelong( &byte[ C_CRC32]) ;
	pzc->ulCompressedSize			= makelong( &byte[ C_COMPRESSED_SIZE]) ;
	pzc->ulUncompressedSize			= makelong( &byte[ C_UNCOMPRESSED_SIZE]) ;
	pzc->usFilenameLength			= makeword( &byte[ C_FILENAME_LENGTH]) ;
	pzc->usExtraFieldLength			= makeword( &byte[ C_EXTRA_FIELD_LENGTH]) ;
	pzc->usFileCommentLength		= makeword( &byte[ C_FILE_COMMENT_LENGTH]) ;
	pzc->usDiskNumberStart			= makeword( &byte[ C_DISK_NUMBER_START]) ;
	pzc->usInternalFileAttributes		= makeword( &byte[ C_INTERNAL_FILE_ATTRIBUTES]) ;
	pzc->ulExternalFileAttributes		= makelong( &byte[ C_EXTERNAL_FILE_ATTRIBUTES]) ;
	pzc->ulRelativeOffsetLocalHeader	= makelong( &byte[ C_RELATIVE_OFFSET_LOCAL_HEADER]) ;

	// �t�@�C�����ǂ݂Ƃ�
	if( pzc->usFilenameLength > 0)
	{
		char* pszFilename = new char[ pzc->usFilenameLength + 1] ;
		fread( pszFilename, sizeof( char), pzc->usFilenameLength, fzip) ;
		pszFilename[ pzc->usFilenameLength] = '\0' ;
		pzc->pszFilename = pszFilename ;
	}

	// �g���̈�
	if( pzc->usExtraFieldLength > 0)
	{
		BYTE* pbyte = new BYTE[ pzc->usExtraFieldLength] ;
		fread( pbyte, sizeof( BYTE), pzc->usExtraFieldLength, fzip) ;
		pzc->pbyteExtra = pbyte ;
	}

	// �R�����g
	if( pzc->usFileCommentLength > 0)
	{
		char* pszComment = new char[ pzc->usFileCommentLength + 1] ;
		fread( pszComment, sizeof( char), pzc->usFileCommentLength, fzip) ;
		pszComment[ pzc->usFileCommentLength] = '\0' ;
		pzc->pszComment = pszComment ;
	}

	vecZipChild.push_back( pzc) ;
	return TRUE ;
}


/******************************************************************************/
//		�t�@�C�����
/******************************************************************************/
// �t�@�C�����
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File* ZipFile::GetFileInfo( int i)
{
	FILE* fzip = fopen( strFilePath.c_str(), "rb") ;
	if( !fzip)
	{
		return NULL ;
	}

	if( !vecChildFile[ i])
	{
		Mp3File* pMp3 = new Mp3File( strFilePath, vecZipChild[ i]) ;
		pMp3->ReadHeader() ;
		vecChildFile[ i] = pMp3 ;
	}

	return vecChildFile[ i] ;
}


/******************************************************************************/
//		�f�[�^�擾
/******************************************************************************/
// �Đ����Ԏ擾
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetPlayLength()
{
	ULONG ulPlayLength = 0 ;

	for( int i = 0; i < vecChildFile.size(); i++)
	{
		if( !vecChildFile[ i])
		{
			Mp3File* pMp3 = new Mp3File( strFilePath, vecZipChild[ i]) ;
			pMp3->ReadHeader() ;
			vecChildFile[ i] = pMp3 ;
		}

		ulPlayLength += vecChildFile[ i]->GetPlayLength() ;
	}

	return ulPlayLength ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string ZipFile::GetFileName( int i)
{
	return vecZipChild[ i]->pszFilename ;
}


/******************************************************************************/
// �q���t�@�C���̎擾
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File* ZipFile::GetChildFile( int i)
{
	if( vecChildFile[ i] == NULL)
	{
		GetFileInfo( i) ;
	}

	return vecChildFile[ i] ;
}


/******************************************************************************/
// �q���t�@�C���̐����擾
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ZipFile::GetChildFileCount() const
{
	return vecChildFile.size() ;
}