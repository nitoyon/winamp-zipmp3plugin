
// File.cpp
//============================================================================//
// �X�V�F02/12/28(�y)
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
: strZipPath( s), ulCentralDir( u)
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
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL File::ReadHeader()
{
	FILE* fzip = fopen( strZipPath.c_str(), "rb") ;
	if( !fzip)
	{
		return FALSE ;
	}
	if( fseek( fzip, ulCentralDir, SEEK_SET) != 0)
	{
		fclose( fzip) ;
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
		fclose( fzip) ;
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
		delete[] pszFilename ;
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
		delete[] pszComment ;
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
		fclose( fzip) ;
		return FALSE ;
	}
	fread( byte, sizeof( BYTE), LOCAL_HEADER_SIZE, fzip) ;
	USHORT usFilenameLength2	= makeword( &byte[ L_FILENAME_LENGTH]) ;
	USHORT	usExtraFieldLength2	= makeword( &byte[ L_EXTRA_FIELD_LENGTH]) ;
	ulFileHead = zipheader.ulRelativeOffsetLocalHeader + 4 + LOCAL_HEADER_SIZE
		   + usFilenameLength2 + usExtraFieldLength2 ;

	fclose( fzip) ;
	return TRUE ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string File::GetFileName() const 
{
	string strPath = zipheader.strFilename ;
	if( strPath == "")
	{
		return "" ;
	}

	int i = GetFileDir().size() ;

	// �t�H���_�̏ꍇ
	if( i >= strPath.size())
	{
		return "" ;
	}
	else
	{
		return strPath.substr( i) ;
	}
}


/******************************************************************************/
// �f�B���N�g���擾
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string File::GetFileDir() const
{
	string strPath = zipheader.strFilename ;
	if( strPath == "")
	{
		return "" ;
	}

	int	intLastYen = 0 ;
	char	pszFile[ MAX_PATH] ;
	char*	pszPointer = pszFile ;
	strcpy( pszFile, strPath.c_str()) ;

	for( int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if( IsDBCSLeadByte( *pszPointer))
		{
			// �Q�o�C�g�����Ȃ�Q�i��
			i++ ;
			continue ;
		}

		if( *pszPointer == '\\' || *pszPointer == '/')
		{
			intLastYen = i ;
		}
	}

	if( intLastYen > 0)
	{
		return strPath.substr( 0, intLastYen + 1) ;	// Yen ���܂߂ĕԂ�
	}
	else
	{
		return "" ;
	}
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


/******************************************************************************/
//		�\�����擾
/******************************************************************************/
// ���X�g�ǉ��p�̕�������擾
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string File::GetDisplayStr( const string& s)
{
	string	strRet ;
	string	strVal ;
	BOOL	blnEncode = FALSE ;

	for( int i = 0; i < s.size(); i++)
	{
		if( blnEncode)
		{
			if( s[ i] != '%')
			{
				strVal += s[ i] ;
			}
			else
			{
				if( strVal == "")
				{
					strRet += '%' ;
				}
				else
				{
					strRet += GetVariable( strVal) ;
				}

				blnEncode = FALSE ;
			}
		}
		else
		{
			if( s[ i] == '%')
			{
				blnEncode = TRUE ;
				strVal = "" ;
			}
			else
			{
				strRet += s[ i] ;
			}
		}
	}
	return strRet ;
}


/******************************************************************************/
// �ϐ��W�J
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string File::GetVariable( const string& strVal)
{
	if( strVal == "FILE_NAME")
	{
		return GetFileName() ;
	}
	else if( strVal == "FILE_PATH")
	{
		return GetFilePath() ;
	}

	return "" ;
}
