
// File.cpp
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "File.h"
#include "define.h"
#include "util.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

File::File( const string& s, ULONG u)
: strZipPath( s), ulCentralDir( u)
{
	ZeroMemory( &zipheader, sizeof( ZipChildHeader)) ;
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

File::~File()
{
	if( zipheader.pbyteExtra)
	{
		delete[] zipheader.pbyteExtra ;
	}
}



/******************************************************************************/
//		取得
/******************************************************************************/
// ヘッダ情報取得
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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
		fclose( fzip) ;
		return FALSE ;
	}

	// 読みとり開始
	BYTE byte[ CENTRAL_DIR_SIZE] ;
	fread( byte, sizeof( BYTE), CENTRAL_DIR_SIZE, fzip) ;

	// ヘッダの読みとり
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
// ファイル名取得
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

string File::GetFileName() const 
{
	string strPath = zipheader.strFilename ;
	if( strPath == "")
	{
		return "" ;
	}

	int i = GetFileDir().size() ;

	// フォルダの場合
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
// ディレクトリ取得
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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
			// ２バイト文字なら２進む
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
		return strPath.substr( 0, intLastYen + 1) ;	// Yen も含めて返す
	}
	else
	{
		return "" ;
	}
}


/******************************************************************************/
// Central Directory のサイズ
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
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
// 再生時間取得
//============================================================================//
// 更新：02/12/26(木)
// 概要：デフォルトは０ミリ秒。
// 補足：なし。
//============================================================================//

ULONG File::GetPlayLength()
{
	return 0 ;
}


/******************************************************************************/
//		表示名取得
/******************************************************************************/
// リスト追加用の文字列を取得
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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
// 変数展開
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
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
