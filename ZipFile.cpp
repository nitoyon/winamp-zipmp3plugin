
// ZipFile.cpp
//============================================================================//
// 更新：03/03/16(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ZipFile.h"
#include "Mp3File.h"
#include "CueFile.h"
#include "util.h"
#include "define.h"
#include "Profile.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  BUF_SIZE		4096


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/31(火)
// 概要：なし。
// 補足：なし。
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
// デストラクタ
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
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
//		ヘッダ解析
/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：03/03/16(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipFile::ReadHeader()
{
	FILE* fzip = fopen( strZipPath.c_str(), "rb") ;
	if( !fzip)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// ヘッダチェック
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


	// 読みとり
	ReadEndCentralDirRec( fzip) ;
	fclose( fzip) ;

	ULONG ulHeaderPos = ulOffsetStartCentralDir ;
	status = Status::COMPRESSED ;
	CueFile* pCueFile = NULL ;

	for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
	{
		// 各ファイルのヘッダ読みとり
		File* pFile = new File( strZipPath, ulHeaderPos) ;
		if( !pFile->ReadHeader())
		{
			status = Status::INVALID_HEADER ;
			return FALSE ;
		}
		ulHeaderPos += pFile->GetCentralDirSize() ;

		// 無圧縮時
		if( !pFile->IsCompressed())
		{
			// 一つでも無圧縮のファイルがあれば、無圧縮とする
			status = Status::UNCOMPRESSED ;

			// 拡張子が mp3 の時
			string s = pFile->GetFilePath() ;
			if( s.size() > 4 && stricmp( s.substr( s.size() - 4).c_str(), ".mp3") == 0)
			{
				// MP3 の情報を取得
				Mp3File* pMp3File = new Mp3File( pFile) ;
				pMp3File->ReadHeader() ;
				delete pFile ;
				vecChildList.push_back( pMp3File) ;
			}
			// 拡張子が cue のとき
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

	// CUE ファイルがある場合は、曲の長さを変更
	if( pCueFile)
	{
		pCueFile->CalcLength( vecChildList) ;
	}

	// 曲の長さキャッシュ作成
	vecHeadMilisec.assign( usTotalEntriesCentralDir + 1) ;
	vecHeadMilisec[ 0] = 0 ;
	for( i = 0; i < usTotalEntriesCentralDir; i++)
	{
		vecHeadMilisec[ i + 1] = vecHeadMilisec[ i] + vecChildList[ i]->GetPlayLength() ;
	}

	return TRUE ;
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
//		データ取得
/******************************************************************************/
// 再生時間取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：zipファイルのトータルの再生時間を取得。
// 補足：なし。
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
// ファイル名取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
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
// 子供ファイルの取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
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
// 子供ファイルの数を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

int ZipFile::GetChildFileCount() const
{
	return vecChildList.size() ;
}


/******************************************************************************/
// ミリ秒から曲番号を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
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
// 曲番号を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
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
// 曲の時間を取得
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
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