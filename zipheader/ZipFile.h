
// ZipFile.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ZIP_FILE_H__
#define  __ZIP_FILE_H__

#include "..\include.h"
#include <vector>
#include <string>

using namespace std;



/******************************************************************************/
//		子ファイル情報の定義
/******************************************************************************/

struct ChildFile
{
	UINT	uiStart;
	UINT	uiEnd;		// 次のアイテムの先頭アドレス
	string	strFileName;
	BOOL	blnCompressed;
};


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ZipFile
{
public:
	enum Status
	{
		OPEN_ERROR,
		NOT_ZIP,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
// Singlton
	static ZipFile* pInstance;

// zip の情報
	string	strZipPath;
	
	ULONG	ulEof ;
	ULONG	ulEndCentralDirRec ;
	USHORT	usNumOfThisDisk ;
	USHORT	usNumDiskWithStartCentralDir ;
	USHORT	usNumEntriesCentrlDirThisDisk ;
	USHORT	usTotalEntriesCentralDir ;
	ULONG	ulSizeCentralDir ;
	ULONG	ulOffsetStartCentralDir ;
	USHORT	usZipfileCommentLength ;

	Status	status ;

// ファイル
	FILE*	fzip;

// 子供
	vector<ChildFile>	vecChildList ;
	vector< ULONG>	vecHeadMilisec ;

// コンストラクタおよびデストラクタ
private:
	ZipFile( const string&) ;
public:
	~ZipFile() ;
	static ZipFile* GetInstance( const string& s = "") ;

// ヘッダ解析
	BOOL _ReadHeader() ;
private:
	BOOL ReadEndCentralDirRec() ;
	BOOL GetEndCentralDirRecPos() ;
	BOOL ReadCentralDirectory() ;
	ULONG ReadChildHeader(ULONG, ChildFile*) ;

// 読み取り関数
	USHORT makeword( BYTE* p);
	ULONG  makelong( BYTE* p);

public:
// データ取得
	Status	GetStatus() const{ return status ;}

	string	GetChildName(UINT) const;
	UINT	GetChildCount() const ;
	ULONG	GetChildStartPoint(UINT) const ;
	ULONG	GetChildEndPoint(UINT) const ;
	BOOL	_IsCompressed(UINT) const ;
} ;


/******************************************************************************/
//		ZIP の子供ファイル情報
/******************************************************************************/

struct ZipChildHeader
{
	USHORT	usVersionMadeBy ;
	USHORT	usVersionNeededToExtract ;
	USHORT	usGeneralPurposeBitFlag;
	USHORT	usCompressionMethod;
	USHORT	usLastModFileTime;
	USHORT	usLastModFileDate;
	ULONG	ulCrc32;
	ULONG	ulCompressedSize;
	ULONG	ulUncompressedSize;
	USHORT	usFilenameLength;
	USHORT	usExtraFieldLength;
	USHORT	usFileCommentLength;
	USHORT	usDiskNumberStart;
	USHORT	usInternalFileAttributes;
	ULONG	ulExternalFileAttributes;
	ULONG	ulRelativeOffsetLocalHeader;
	string	strFilename ;
	BYTE*	pbyteExtra ;
	string	strComment ;
} ;


/******************************************************************************/
//		ZIP のサイズ情報
/******************************************************************************/
// Local file header
#define L_VERSION_NEEDED_TO_EXTRACT_0     0
#define L_VERSION_NEEDED_TO_EXTRACT_1     1
#define L_GENERAL_PURPOSE_BIT_FLAG        2
#define L_COMPRESSION_METHOD              4
#define L_LAST_MOD_FILE_TIME              6
#define L_LAST_MOD_FILE_DATE              8
#define L_CRC32                           10
#define L_COMPRESSED_SIZE                 14
#define L_UNCOMPRESSED_SIZE               18
#define L_FILENAME_LENGTH                 22
#define L_EXTRA_FIELD_LENGTH              24

#define LOCAL_HEADER_SIZE	26

// Central directory
#define C_VERSION_MADE_BY_0               0
#define C_VERSION_MADE_BY_1               1
#define C_VERSION_NEEDED_TO_EXTRACT_0     2
#define C_VERSION_NEEDED_TO_EXTRACT_1     3
#define C_GENERAL_PURPOSE_BIT_FLAG        4
#define C_COMPRESSION_METHOD              6
#define C_LAST_MOD_FILE_TIME              8
#define C_LAST_MOD_FILE_DATE              10
#define C_CRC32                           12
#define C_COMPRESSED_SIZE                 16
#define C_UNCOMPRESSED_SIZE               20
#define C_FILENAME_LENGTH                 24
#define C_EXTRA_FIELD_LENGTH              26
#define C_FILE_COMMENT_LENGTH             28
#define C_DISK_NUMBER_START               30
#define C_INTERNAL_FILE_ATTRIBUTES        32
#define C_EXTERNAL_FILE_ATTRIBUTES        34
#define C_RELATIVE_OFFSET_LOCAL_HEADER    38

#define CENTRAL_DIR_SIZE	42


// End of central directory record
#define NUMBER_THIS_DISK                  4
#define NUM_DISK_WITH_START_CENTRAL_DIR   6
#define NUM_ENTRIES_CENTRL_DIR_THS_DISK   8
#define TOTAL_ENTRIES_CENTRAL_DIR         10
#define SIZE_CENTRAL_DIRECTORY            12
#define OFFSET_START_CENTRAL_DIRECTORY    16
#define ZIPFILE_COMMENT_LENGTH            20

#define  END_OF_CENTRAL_SIZE	22
#define  MAX_END_OF_CENTRAL	(65536 + END_OF_CENTRAL_SIZE)

#endif