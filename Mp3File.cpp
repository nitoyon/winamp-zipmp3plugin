
// Mp3File.cpp
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Mp3File.h"
#include "define.h"
#include "Profile.h"

#define  BUF_SIZE  256

/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

Mp3File::Mp3File( File* f) 
: File( f->GetZipPath(), f->GetCentralDir())
, ulMpegHeader( 0), intMpeg( 0), intLayer( 0), intBitrate( 0)
{
	zipheader = f->GetHeader() ;
	ulFileHead = f->GetFileHead() ;

	// Extra フィールドの最割り当て
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
// デストラクタ
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

Mp3File::~Mp3File() 
{
}



/******************************************************************************/
// ヘッダ読みとり
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Mp3File::ReadHeader()
{
	FILE* fzip = fopen( strZipPath.c_str(), "rb") ;
	if( fzip)
	{
		// zip ではないかのチェック（今はいい加減）
		fseek( fzip, LOCAL_HEADER_SIZE, SEEK_SET) ;
		if( fgetc( fzip) == 0x50
		 && fgetc( fzip) == 0x4b
		 && fgetc( fzip) == 0x03
		 && fgetc( fzip) == 0x04)
		{
			fclose( fzip) ;
			return FALSE ;
		}

		if( FindMpegHeader( fzip))
		{
			ReadMpegHeader( fzip) ;
			ReadID3v1( fzip) ;
		}

		fclose( fzip) ;
		return TRUE ;
	}

	return FALSE ;
}


/******************************************************************************/
// MPEGヘッダを探す
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Mp3File::FindMpegHeader( FILE* fzip)
{
	// バッファ確保
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
	
			// 指定バイト読みとり
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
// MPEGヘッダ読みとり
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL Mp3File::ReadMpegHeader( FILE* fzip)
{
	if( fseek( fzip, ulMpegHeader, SEEK_SET) != 0)
	{
		return FALSE ;
	}

	fgetc( fzip) ;
	char c = fgetc( fzip) ;

	// MPEGバージョン取得
	if( c & 0x08)
	{
		intMpeg = 1 ;
	}
	else
	{
		intMpeg = ( c & 0x10 ? 3 : 2) ;
	}

	// レイヤ取得
	switch( c & 0x06)
	{
		case 0x06: intLayer = 1 ; break ;
		case 0x04: intLayer = 2 ; break ;
		case 0x02: intLayer = 3 ; break ;
	}

	// ビットレート
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
// ID3v1タグの読みとり
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

void Mp3File::ReadID3v1( FILE* fzip)
{
	blnHasID3Tag = FALSE ;
	if( fseek( fzip, ulFileHead + zipheader.ulUncompressedSize - ID3_TAG_SIZE, SEEK_SET) != 0)
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

	// トラック名
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strTrackName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// アーティスト名
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strArtistName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// アルバム名
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strAlbumName = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// 年号
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_YEAR_SIZE) ;
	id3tag.intYear = atoi( pszBuf) ;
	i += ID3_YEAR_SIZE ;

	// コメント
	ZeroMemory( pszBuf, ID3_DATA_SIZE + 1) ;
	memcpy( (void*)pszBuf, (void*)( pbyte+ i), ID3_DATA_SIZE) ;
	id3tag.strComment = StripSpace( pszBuf) ;
	i += ID3_DATA_SIZE ;

	// トラック番号
	if( pszBuf[ ID3_DATA_SIZE - 2] == '\0')
	{
		id3tag.intTrackNum = pszBuf[ ID3_DATA_SIZE - 1] ;
	}
	else
	{
		id3tag.intTrackNum = TRACK_NUM_UNDEF ;
	}

	// ジャンル
	id3tag.intGenre = pbyte[ i] ;
}


/******************************************************************************/
// 行末の空白を削除
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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
//		長さ取得
/******************************************************************************/
// ファイルの長さ取得
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
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


/******************************************************************************/
// 変数展開
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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