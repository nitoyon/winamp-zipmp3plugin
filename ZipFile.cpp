
// ZipFile.cpp
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ZipFile.h"
#include "Mp3File.h"
#include "util.h"
#include "define.h"


#define  BUF_SIZE		4096


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
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
// デストラクタ
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

ZipFile::~ZipFile() 
{
}


/******************************************************************************/
//		ヘッダ解析
/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
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


		// 読みとり
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
// End Central Directory record の読みとり
//============================================================================//
// 更新：02/12/21(土)
// 概要：なし。
// 補足：なし。
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
// 更新：02/12/21(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipFile::GetEndCentralDirRecPos( FILE* fzip)
{
	// バッファ確保
	char psBuf[ BUF_SIZE + 3] ;
	psBuf[ BUF_SIZE    ] = '\0' ;
	psBuf[ BUF_SIZE + 1] = '\0' ;
	psBuf[ BUF_SIZE + 2] = '\0' ;

	ULONG ulPos = ulEof ;
	ULONG ulPrevPos ;

	while( TRUE)
	{
		// 読み込み位置設定
		ulPrevPos = ulPos ;
		ulPos -= BUF_SIZE ;
		if( ulPos > ulPrevPos)
		{
			ulPos = 0 ;
		}

		if( fseek( fzip, ulPos, SEEK_SET) == 0)
		{
			fread( psBuf, sizeof(char), ulPrevPos - ulPos, fzip) ;
	
			// 指定バイト読みとり
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
// Central Directoryの読みとり
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipFile::ReadCentralDirectory( FILE* fzip)
{
	// ヘッダのチェック
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

	// 読みとり開始
	BYTE byte[ CENTRAL_DIR_SIZE] ;
	fread( byte, sizeof( BYTE), CENTRAL_DIR_SIZE, fzip) ;

	// ヘッダの読みとり
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

	// ファイル名読みとり
	if( pzc->usFilenameLength > 0)
	{
		char* pszFilename = new char[ pzc->usFilenameLength + 1] ;
		fread( pszFilename, sizeof( char), pzc->usFilenameLength, fzip) ;
		pszFilename[ pzc->usFilenameLength] = '\0' ;
		pzc->pszFilename = pszFilename ;
	}

	// 拡張領域
	if( pzc->usExtraFieldLength > 0)
	{
		BYTE* pbyte = new BYTE[ pzc->usExtraFieldLength] ;
		fread( pbyte, sizeof( BYTE), pzc->usExtraFieldLength, fzip) ;
		pzc->pbyteExtra = pbyte ;
	}

	// コメント
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
//		ファイル解析
/******************************************************************************/
// ファイル解析
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
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
//		データ取得
/******************************************************************************/
// 再生時間取得
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
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
// ファイル名取得
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string ZipFile::GetFileName( int i)
{
	return vecZipChild[ i]->pszFilename ;
}


/******************************************************************************/
// 子供ファイルの取得
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
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
// 子供ファイルの数を取得
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

int ZipFile::GetChildFileCount() const
{
	return vecChildFile.size() ;
}