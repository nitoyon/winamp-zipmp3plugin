
// ZipFile.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ZipFile.h"


/******************************************************************************/
//		�X�^�e�B�b�N�ϐ�
/******************************************************************************/

ZipFile* ZipFile::pInstance = NULL;


/******************************************************************************/
//		��`
/******************************************************************************/

#define  BUF_SIZE		4096


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::ZipFile( const string& s)
: ulEof( 0)
{
	strZipPath = s;
	
	fzip = fopen( s.c_str(), "rb") ;
	if( fzip)
	{
		if( fseek( fzip, 0, SEEK_END) == 0)
		{
			ulEof = ftell( fzip) ;
		}

		fclose( fzip) ;
		fzip = NULL ;
	}
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::~ZipFile() 
{
	if( fzip)
	{
		fclose( fzip) ;
		fzip = NULL ;
	}
}


/******************************************************************************/
// �C���X�^���X�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile* ZipFile::GetInstance(const string& s)
{
	if(s != "")
	{
		if(!pInstance)
		{
			pInstance = new ZipFile(s);
		}
		else
		{
			delete pInstance;
			pInstance = new ZipFile(s);
		}
	}

	return pInstance;
}


/******************************************************************************/
//		�w�b�_���
/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �X�V�F03/03/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::_ReadHeader()
{
	fzip = fopen( strZipPath.c_str(), "rb") ;
	if( !fzip || fseek(fzip, 0, SEEK_SET) != 0)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// �w�b�_�`�F�b�N
	int i1 = fgetc( fzip);
	int i2 = fgetc( fzip);
	int i3 = fgetc( fzip);
	int i4 = fgetc( fzip);
	if( i1 == 0x50//fgetc( fzip) == 0x50
	 && i2 == 0x4b//fgetc( fzip) == 0x4b
	 && i3 == 0x03//fgetc( fzip) == 0x03
	 && i4 == 0x04)//fgetc( fzip) == 0x04)
	{
		// ok
	}
	else
	{
		status = Status::NOT_ZIP ;
		fclose( fzip) ;
		fzip = NULL ;
		return FALSE ;
	}


	// �ǂ݂Ƃ�
	ReadEndCentralDirRec() ;

	ULONG ulHeaderPos = ulOffsetStartCentralDir ;
	status = Status::COMPRESSED ;

	for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
	{
		// �e�t�@�C���̃w�b�_�ǂ݂Ƃ�
		ChildFile child;
		ULONG ulCentralDirSize = ReadChildHeader(ulHeaderPos, &child);
		if(ulCentralDirSize == 0)
		{
			status = Status::INVALID_HEADER ;
			fclose( fzip) ;
			fzip = NULL ;
			return FALSE;
		}
		ulHeaderPos += ulCentralDirSize;

		// �����k��
		if( !child.blnCompressed)
		{
			// ��ł������k�̃t�@�C��������΁A�����k�Ƃ���
			status = Status::UNCOMPRESSED ;
		}

		vecChildList.push_back(child) ;
	}

	fclose( fzip) ;
	fzip = NULL ;
	return TRUE ;
}


/******************************************************************************/
// End Central Directory record �̓ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/21(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::ReadEndCentralDirRec()
{
	if( !GetEndCentralDirRecPos())
	{
		status = Status::INVALID_HEADER ;
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

BOOL ZipFile::GetEndCentralDirRecPos()
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
// �q�t�@�C���̃w�b�_��ǂݎ��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B	
//============================================================================//

ULONG ZipFile::ReadChildHeader(ULONG ulCentralDir, ChildFile* pChild)
{
	if( !fzip)
	{
		return 0;
	}

	if(fseek( fzip, ulCentralDir, SEEK_SET) != 0)
	{
		return 0;
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
		return 0;
	}

	// �ǂ݂Ƃ�J�n
	BYTE byte[ CENTRAL_DIR_SIZE] ;
	fread( byte, sizeof( BYTE), CENTRAL_DIR_SIZE, fzip) ;

	// �w�b�_�̓ǂ݂Ƃ�
	ZipChildHeader zipheader;
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
		return 0;
	}
	fread( byte, sizeof( BYTE), LOCAL_HEADER_SIZE, fzip) ;
	USHORT usFilenameLength2	= makeword( &byte[ L_FILENAME_LENGTH]) ;
	USHORT usExtraFieldLength2	= makeword( &byte[ L_EXTRA_FIELD_LENGTH]) ;
	ULONG  ulFileHead = zipheader.ulRelativeOffsetLocalHeader + 4 + LOCAL_HEADER_SIZE
		   + usFilenameLength2 + usExtraFieldLength2 ;

	// �\���̂ɃZ�b�g
	pChild->uiStart		= ulFileHead;
	pChild->uiEnd		= ulFileHead + zipheader.ulCompressedSize;
	pChild->strFileName	= zipheader.strFilename;
	pChild->blnCompressed	= (zipheader.ulCompressedSize != zipheader.ulCompressedSize);

	return 4
	     + CENTRAL_DIR_SIZE
	     + zipheader.usFilenameLength
	     + zipheader.usExtraFieldLength
	     + zipheader.usFileCommentLength;;
}


/******************************************************************************/
//		���[�e�B���e�B�[�֐�
/******************************************************************************/
// ushort
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

USHORT makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG  makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}




/******************************************************************************/
//		���擾
/******************************************************************************/
// �t�@�C����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT ZipFile::GetChildCount() const
{
	return vecChildList.size();
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string ZipFile::GetChildName(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return "";
	}

	return vecChildList[ui].strFileName;
}


/******************************************************************************/
// �t�@�C���̎n�_�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetChildStartPoint(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].uiStart;
}


/******************************************************************************/
// �t�@�C���̏I�_�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetChildEndPoint(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].uiEnd;
}


/******************************************************************************/
// ���k����Ă��邩
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipFile::_IsCompressed(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].blnCompressed;
}

/******************************************************************************/
//		�ǂݎ��֐�
/******************************************************************************/
// ushort
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

USHORT ZipFile::makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG  ZipFile::makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}


