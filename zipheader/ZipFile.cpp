
// ZipFile.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ZipFile.h"


/******************************************************************************/
//		スタティック変数
/******************************************************************************/

ZipFile* ZipFile::pInstance = NULL;


/******************************************************************************/
//		定義
/******************************************************************************/

#define  BUF_SIZE		4096


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// インスタンス取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
//		ヘッダ解析
/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：03/03/16(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipFile::_ReadHeader()
{
	fzip = fopen( strZipPath.c_str(), "rb") ;
	if( !fzip || fseek(fzip, 0, SEEK_SET) != 0)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// ヘッダチェック
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


	// 読みとり
	ReadEndCentralDirRec() ;

	ULONG ulHeaderPos = ulOffsetStartCentralDir ;
	status = Status::COMPRESSED ;

	for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
	{
		// 各ファイルのヘッダ読みとり
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

		// 無圧縮時
		if( !child.blnCompressed)
		{
			// 一つでも無圧縮のファイルがあれば、無圧縮とする
			status = Status::UNCOMPRESSED ;
		}

		vecChildList.push_back(child) ;
	}

	fclose( fzip) ;
	fzip = NULL ;
	return TRUE ;
}


/******************************************************************************/
// End Central Directory record の読みとり
//============================================================================//
// 更新：02/12/21(土)
// 概要：なし。
// 補足：なし。
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
// 更新：02/12/21(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipFile::GetEndCentralDirRecPos()
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
// 子ファイルのヘッダを読み取る
//============================================================================//
// 概要：なし。
// 補足：なし。	
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
		return 0;
	}

	// 読みとり開始
	BYTE byte[ CENTRAL_DIR_SIZE] ;
	fread( byte, sizeof( BYTE), CENTRAL_DIR_SIZE, fzip) ;

	// ヘッダの読みとり
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

	// ファイル名読みとり
	if( zipheader.usFilenameLength > 0)
	{
		char* pszFilename = new char[ zipheader.usFilenameLength + 1] ;
		fread( pszFilename, sizeof( char), zipheader.usFilenameLength, fzip) ;
		pszFilename[ zipheader.usFilenameLength] = '\0' ;
		zipheader.strFilename = pszFilename ;
		delete[] pszFilename ;
	}

	// 拡張領域
	if( zipheader.usExtraFieldLength > 0)
	{
		BYTE* pbyte = new BYTE[ zipheader.usExtraFieldLength] ;
		fread( pbyte, sizeof( BYTE), zipheader.usExtraFieldLength, fzip) ;
		zipheader.pbyteExtra = pbyte ;
	}

	// コメント
	if( zipheader.usFileCommentLength > 0)
	{
		char* pszComment = new char[ zipheader.usFileCommentLength + 1] ;
		fread( pszComment, sizeof( char), zipheader.usFileCommentLength, fzip) ;
		pszComment[ zipheader.usFileCommentLength] = '\0' ;
		zipheader.strComment = pszComment ;
		delete[] pszComment ;
	}

	// ローカルヘッダ
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

	// 構造体にセット
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
//		ユーティリティー関数
/******************************************************************************/
// ushort
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

USHORT makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

ULONG  makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}




/******************************************************************************/
//		情報取得
/******************************************************************************/
// ファイル数
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT ZipFile::GetChildCount() const
{
	return vecChildList.size();
}


/******************************************************************************/
// ファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ファイルの始点取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ファイルの終点取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 圧縮されているか
//============================================================================//
// 概要：なし。
// 補足：なし。
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
//		読み取り関数
/******************************************************************************/
// ushort
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

USHORT ZipFile::makeword( BYTE* p)
{
	return (USHORT)((p[1] << 8) | p[0]) ;
}


/******************************************************************************/
// ulong
//============================================================================//
// 更新：02/12/22(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

ULONG  ZipFile::makelong( BYTE* p)
{
	return (((ULONG)p[3]) << 24)
	     + (((ULONG)p[2]) << 16)
	     + (((ULONG)p[1]) << 8)
	     + ((ULONG)p[0]);
}


