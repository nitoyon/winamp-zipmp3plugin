
// ZipFile.cpp
//============================================================================//
// �X�V�F03/03/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ZipFile.h"
#include "Mp3File.h"
#include "CueFile.h"
#include "util.h"
#include "define.h"
#include "Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  BUF_SIZE		4096


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/31(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::ZipFile( const string& s, ULONG u)
: File( s, u), ulEof( 0)
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


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipFile::~ZipFile() 
{
	for( int i = 0; i < vecChildList.size(); i++)
	{
		File* pFile = (File*)vecChildList[i];
		delete vecChildList[ i] ;
	}
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

BOOL ZipFile::ReadHeader()
{
	FILE* fzip = fopen( strZipPath.c_str(), "rb") ;
	if( !fzip)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// �w�b�_�`�F�b�N
	if( fgetc( fzip) == 0x50
	 && fgetc( fzip) == 0x4b
	 && fgetc( fzip) == 0x03
	 && fgetc( fzip) == 0x04)
	{
		// ok
	}
	else
	{
		status = Status::NOT_ZIP ;
		fclose( fzip) ;
		return FALSE ;
	}


	// �ǂ݂Ƃ�
	ReadEndCentralDirRec( fzip) ;
	fclose( fzip) ;

	ULONG ulHeaderPos = ulOffsetStartCentralDir ;
	status = Status::COMPRESSED ;
	CueFile* pCueFile = NULL ;

	for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
	{
		// �e�t�@�C���̃w�b�_�ǂ݂Ƃ�
		File* pFile = new File( strZipPath, ulHeaderPos) ;
		if( !pFile->ReadHeader())
		{
			status = Status::INVALID_HEADER ;
			return FALSE ;
		}
		ulHeaderPos += pFile->GetCentralDirSize() ;

		// �����k��
		if( !pFile->IsCompressed())
		{
			// ��ł������k�̃t�@�C��������΁A�����k�Ƃ���
			status = Status::UNCOMPRESSED ;

			// �g���q�� mp3 �̎�
			string s = pFile->GetFilePath() ;
			if( s.size() > 4 && stricmp( s.substr( s.size() - 4).c_str(), ".mp3") == 0)
			{
				// MP3 �̏����擾
				Mp3File* pMp3File = new Mp3File( pFile) ;
				pMp3File->ReadHeader() ;
				delete pFile ;
				vecChildList.push_back( pMp3File) ;
			}
			// �g���q�� cue �̂Ƃ�
			else if( s.size() > 4 && stricmp( s.substr( s.size() - 4).c_str(), ".cue") == 0)
			{
				if( pCueFile)
				{
					delete pCueFile ;
				}
				pCueFile = new CueFile( pFile) ;
				pCueFile->ReadCueFile() ;
				vecChildList.push_back( pFile) ;
			}
			else
			{
				vecChildList.push_back( pFile) ;
			}
		}
		else
		{
			vecChildList.push_back( pFile) ;
		}
	}

	// CUE �t�@�C��������ꍇ�́A�Ȃ̒�����ύX
	if( pCueFile)
	{
		pCueFile->CalcLength( vecChildList) ;
	}

	// �Ȃ̒����L���b�V���쐬
	vecHeadMilisec.assign( usTotalEntriesCentralDir + 1) ;
	vecHeadMilisec[ 0] = 0 ;
	for( i = 0; i < usTotalEntriesCentralDir; i++)
	{
		vecHeadMilisec[ i + 1] = vecHeadMilisec[ i] + vecChildList[ i]->GetPlayLength() ;
	}

	return TRUE ;
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
//		�f�[�^�擾
/******************************************************************************/
// �Đ����Ԏ擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�Fzip�t�@�C���̃g�[�^���̍Đ����Ԃ��擾�B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetPlayLength()
{
	ULONG ulPlayLength = 0 ;

	for( int i = 0; i < vecChildList.size(); i++)
	{
		if( !vecChildList[ i])
		{
			ulPlayLength += vecChildList[ i]->GetPlayLength() ;
		}
	}

	return ulPlayLength ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string ZipFile::GetFileName( int i)
{
	if( i < 0 && i >= vecChildList.size())
	{
		return "" ;
	}
	return vecChildList[ i]->GetFileName() ;
}


/******************************************************************************/
// �q���t�@�C���̎擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File* ZipFile::GetChildFile( int i)
{
	if( i < 0 && i >= vecChildList.size())
	{
		return NULL ;
	}
	return vecChildList[ i] ;
}


/******************************************************************************/
// �q���t�@�C���̐����擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ZipFile::GetChildFileCount() const
{
	return vecChildList.size() ;
}


/******************************************************************************/
// �~���b����Ȕԍ����擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ZipFile::GetSongIndex( ULONG ulMilisec) const
{
	if( vecHeadMilisec.size() != usTotalEntriesCentralDir + 1)
	{
		return -1 ;
	}

	for( int i = 0; i < vecHeadMilisec.size() - 1; i++)
	{
		if( ulMilisec < vecHeadMilisec[ i + 1])
		{
			return i ;
		}
	}

	return vecHeadMilisec.size() - 1 ;
}


/******************************************************************************/
// �Ȕԍ����擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetSongHead( int intNum) const
{
	if( intNum < 0 || intNum >= vecHeadMilisec.size() - 1)
	{
		return 0 ;
	}

	return vecHeadMilisec[ intNum] ;
}


/******************************************************************************/
// �Ȃ̎��Ԃ��擾
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG ZipFile::GetSongTime( int intSongIndex, ULONG ulMilisec) const
{
	if( intSongIndex < 0 || intSongIndex >= vecHeadMilisec.size() - 1)
	{
		return 0 ;
	}

	if( Profile::blnCountUp)
	{
		return ulMilisec - vecHeadMilisec[ intSongIndex] ;
	}
	else
	{
		return vecHeadMilisec[ intSongIndex + 1] - ulMilisec ;
	}
}