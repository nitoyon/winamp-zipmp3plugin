
// Mp3File.cpp
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Mp3File.h"
#include "define.h"

#define  BUF_SIZE  256

/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Mp3File::Mp3File( File* f) 
: File( f->GetFilePath(), f->GetCentralDir())
, ulMpegHeader( 0), intMpeg( 0), intLayer( 0), intBitrate( 0)
{
	zipheader = f->GetHeader() ;
	ulFileHead = f->GetFileHead() ;

	// Extra �t�B�[���h�̍Ŋ��蓖��
	if( zipheader.usExtraFieldLength)
	{
		BYTE* pbyte = zipheader.pbyteExtra ;
		zipheader.pbyteExtra = new BYTE[ zipheader.usExtraFieldLength] ;
		for( int i = 0; i < zipheader.usExtraFieldLength; i++)
		{
			zipheader.pbyteExtra[ i] = pbyte[ i] ;
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

Mp3File::~Mp3File() 
{
}



/******************************************************************************/
// �w�b�_�ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Mp3File::ReadHeader()
{
	FILE* fzip = fopen( strFilePath.c_str(), "rb") ;
	if( fzip)
	{
		// zip �ł͂Ȃ����̃`�F�b�N�i���͂��������j
		fseek( fzip, LOCAL_HEADER_SIZE, SEEK_SET) ;
		if( fgetc( fzip) == 0x50
		 && fgetc( fzip) == 0x4b
		 && fgetc( fzip) == 0x03
		 && fgetc( fzip) == 0x04)
		{
			return FALSE ;
		}

		if( FindMpegHeader( fzip))
		{
			ReadMpegHeader( fzip) ;
		}

		fclose( fzip) ;
		return TRUE ;
	}

	return FALSE ;
}


/******************************************************************************/
// MPEG�w�b�_��T��
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Mp3File::FindMpegHeader( FILE* fzip)
{
	// �o�b�t�@�m��
	BYTE byte[ BUF_SIZE + 2] ;
	byte[ BUF_SIZE    ] = '\0' ;
	byte[ BUF_SIZE + 1] = '\0' ;

	ULONG ulPos = ulFileHead ;
	ULONG ulEnd = zipheader.ulRelativeOffsetLocalHeader + zipheader.ulCompressedSize ;
	ULONG ulBufSize = 0 ;
	while( TRUE)
	{
		if( fseek( fzip, ulPos, SEEK_SET) == 0)
		{
			ulBufSize = ( ulPos + BUF_SIZE > ulEnd ? ulEnd - ulPos : BUF_SIZE) ;
			fread( byte, sizeof(char), ulBufSize, fzip) ;
	
			// �w��o�C�g�ǂ݂Ƃ�
			for( int j = 0; j < ulBufSize; j++)
			{
				if( byte[ j] == 0xff)
				{
					if( byte[ j + 1] & 0xe0 == 0xe0)
					{
						ulMpegHeader = ulPos + j ;
						return TRUE ;
					}
				}
			}

			byte[ BUF_SIZE + 0] = byte[ 0] ;
			byte[ BUF_SIZE + 1] = byte[ 1] ;
		}

		ulPos += BUF_SIZE ;
		if( ulPos > ulEnd)
		{
			break ;
		}
	}

	ulMpegHeader = -1 ;
	return FALSE ;
}


/******************************************************************************/
// MPEG�w�b�_�ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/22(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Mp3File::ReadMpegHeader( FILE* fzip)
{
	if( fseek( fzip, ulMpegHeader, SEEK_SET) != 0)
	{
		return FALSE ;
	}

	fgetc( fzip) ;
	char c = fgetc( fzip) ;

	// MPEG�o�[�W�����擾
	if( c & 0x08)
	{
		intMpeg = 1 ;
	}
	else
	{
		intMpeg = ( c & 0x10 ? 3 : 2) ;
	}

	// ���C���擾
	switch( c & 0x06)
	{
		case 0x06: intLayer = 1 ; break ;
		case 0x04: intLayer = 2 ; break ;
		case 0x02: intLayer = 3 ; break ;
	}

	// �r�b�g���[�g
	int intBitrateTable[][ 3][ 16] = {
		{
			{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0}, 
			{ 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0}, 
			{ 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0}
		},
		{
			{ 0, 32, 48, 56,  64,  80,  96, 112, 128, 114, 160,  16, 192, 224, 256, 0}, 
			{ 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0}, 
			{ 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0}
		}
	} ;
	c = fgetc( fzip) ;
	c = (c >> 4) & 0x0f ;
	if( intMpeg <= 2)
	{
		intBitrate = intBitrateTable[ intMpeg - 1][ intLayer - 1][ c] ;
	}
	else
	{
		intBitrate = intBitrateTable[ 1][ 2][ c] ;
	}

	return TRUE ;
}


/******************************************************************************/
//		�����擾
/******************************************************************************/
// �t�@�C���̒����擾
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG Mp3File::GetPlayLength() 
{
	if( intBitrate != 0)
	{
		return zipheader.ulCompressedSize * 8 / intBitrate ;
	}
	else
	{
		return 0 ;
	}
}

