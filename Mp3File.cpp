
// Mp3File.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Mp3File.h"
#include "define.h"
#include "Profile.h"
#include "Id3tagv2.h"

#define  BUF_SIZE  256

/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Mp3File::Mp3File(FileInfo* p) 
: File(p)
, ulMpegHeader( 0), intMpeg( 0), intLayer( 0), intBitrate( 0), ulLengthCache( 0)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Mp3File::~Mp3File() 
{
}



/******************************************************************************/
// �w�b�_�ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Mp3File::ReadHeader()
{
	FILE* fzip = fopen( strArchivePath.c_str(), "rb") ;
	if(!fzip)
	{
		return FALSE;
	}

	// zip �ł͂Ȃ����̃`�F�b�N�i���͂��������j
	if( FindMpegHeader( fzip))
	{
		ReadMpegHeader( fzip) ;
		ReadID3v1( fzip) ;

		// ID3 v2�^�O�ǂ݂Ƃ�
		if(Profile::blnUseId3v2 && fseek( fzip, uiStartPoint, SEEK_SET) == 0)
		{
			CId3tagv2 tagv2 ;
			tagv2.Load( fzip) ;
			if(tagv2.IsEnable())
			{
				blnHasID3Tag = TRUE ;
				id3tag.strTrackName	= tagv2.GetTitle() ;
				id3tag.strArtistName	= tagv2.GetArtist() ;
				id3tag.strAlbumName	= tagv2.GetAlbum() ;
				id3tag.intYear		= atoi( tagv2.GetYear().c_str()) ;
				id3tag.strComment	= tagv2.GetComment() ;
				id3tag.intTrackNum	= atoi( tagv2.GetTrackNo().c_str()) ;
			}
		}
	}

	fclose( fzip) ;
	return TRUE ;
}


/******************************************************************************/
// MPEG�w�b�_��T��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL Mp3File::FindMpegHeader( FILE* fzip)
{
	// �o�b�t�@�m��
	BYTE byte[ BUF_SIZE + 2] ;
	byte[ BUF_SIZE    ] = '\0' ;
	byte[ BUF_SIZE + 1] = '\0' ;

	ULONG ulPos = uiStartPoint;
	ULONG ulEnd = uiEndPoint;
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
					if( ( byte[ j + 1] & 0xe0) == 0xe0)
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
// ID3v1�^�O�̓ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Mp3File::ReadID3v1( FILE* fzip)
{
	blnHasID3Tag = FALSE ;
	if( fseek( fzip, uiEndPoint - ID3_TAG_SIZE, SEEK_SET) != 0)
	{
		return ;
	}

	BYTE pbyte[ ID3_TAG_SIZE] ;
	if( fread( pbyte, sizeof( BYTE), ID3_TAG_SIZE, fzip) != ID3_TAG_SIZE)
	{
		return ;
	}

	if( pbyte[ 0] == 'T'
	 && pbyte[ 1] == 'A'
	 && pbyte[ 2] == 'G')
	{
		// ok
	}
	else
	{
		return ;
	}

	blnHasID3Tag = TRUE ;
	int  i = 3 ;
	char pszBuf[ ID3_DATA_SIZE + 1] ;

	// �g���b�N��
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strTrackName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// �A�[�e�B�X�g��
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strArtistName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// �A���o����
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strAlbumName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// �N��
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_YEAR_SIZE) ;
	id3tag.intYear = atoi( pszBuf) ;
	i += ID3_YEAR_SIZE ;

	// �R�����g
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strComment = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// �g���b�N�ԍ�
	if( pszBuf[ ID3_DATA_SIZE - 2] == '\0')
	{
		id3tag.intTrackNum = pszBuf[ ID3_DATA_SIZE - 1] ;
	}
	else
	{
		id3tag.intTrackNum = TRACK_NUM_UNDEF ;
	}

	// �W������
	id3tag.intGenre = pbyte[ i] ;
}


/******************************************************************************/
// �s���̋󔒂��폜
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string Mp3File::StripSpace( const char* pszBuf)
{
	string s = pszBuf ;

	for( int i = ID3_DATA_SIZE - 1; i >= 0; i--)
	{
		if( s[ i] != ' ')
		{
			break ;
		}
	}

	return s.substr( 0, i + 1) ;
}


/******************************************************************************/
//		�����擾
/******************************************************************************/
// �t�@�C���̒����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG Mp3File::GetPlayLength()
{
	if( ulLengthCache != 0)
	{
		return ulLengthCache ;
	}
	else if( intBitrate != 0)
	{
		return (uiEndPoint - uiStartPoint) * 8 / intBitrate ;
	}
	else
	{
		return 0 ;
	}
}


/******************************************************************************/
// �����̃L���b�V�����w��
//============================================================================//
// �T�v�F�L���b�V�����w�肵���ꍇ�́A�r�b�g���C�g����Ȓ����v�Z���Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Mp3File::SetPlayLength( ULONG ul) 
{
	ulLengthCache = ul ;
}


/******************************************************************************/
// �ϐ��W�J
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string Mp3File::GetVariable( const string& strVal)
{
	if( blnHasID3Tag)
	{
		if( strVal == "TRACK_NAME")
		{
			return id3tag.strTrackName ;
		}
		else if( strVal == "ARTIST_NAME")
		{
			return id3tag.strArtistName ;
		}
		else if( strVal == "ALBUM_NAME")
		{
			return id3tag.strAlbumName ;
		}
		else if( strVal == "YEAR")
		{
			char pszBuf[ 5] ;
			sprintf( pszBuf, "%d", id3tag.intYear) ;
			return pszBuf ;
		}
		else if( strVal == "COMMENT")
		{
			return id3tag.strComment ;
		}
		else if( strVal == "TRACK_NUMBER")
		{
			char pszBuf[ 100] ;
			sprintf( pszBuf, "%d", id3tag.intTrackNum) ;
			return pszBuf ;
		}
		else if( strVal == "TRACK_NUMBER2")
		{
			char pszBuf[ 100] ;
			sprintf( pszBuf, "%02d", id3tag.intTrackNum) ;
			return pszBuf ;
		}
		else if( strVal == "TRACK_NUMBER3")
		{
			char pszBuf[ 100] ;
			sprintf( pszBuf, "%03d", id3tag.intTrackNum) ;
			return pszBuf ;
		}
	}

	return File::GetVariable( strVal) ;
}