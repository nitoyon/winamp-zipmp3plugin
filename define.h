
// define.h
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __DEFINE_H__
#define  __DEFINE_H__

/******************************************************************************/
// 		定義
/******************************************************************************/
// ヘッダDLLのエクスポート関数
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

typedef UINT	(WINAPI *PGET_DLL_VERSION)();
typedef UINT	(WINAPI *PGET_DLL_TYPE)();
typedef BOOL	(WINAPI *PREAD_HEADER)(PCTSTR);
typedef UINT	(WINAPI *PGET_FILE_COUNT)();
typedef PCTSTR	(WINAPI *PGET_FILE_NAME)(UINT);
typedef UINT	(WINAPI *PGET_FILE_START_POINT)(UINT);
typedef UINT	(WINAPI *PGET_FILE_END_POINT)(UINT);
typedef BOOL	(WINAPI *PIS_COMPRESSED)(UINT);


/******************************************************************************/
// エクスポート関数一覧の構造体
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

struct HeaderDllFunc
{
	PGET_DLL_VERSION	GetDllVersion;
	PGET_DLL_TYPE		GetDllType;
	PREAD_HEADER		ReadHeader;
	PGET_FILE_COUNT		GetFileCount;
	PGET_FILE_NAME		GetFileName;
	PGET_FILE_START_POINT	GetFileStartPoint;
	PGET_FILE_END_POINT	GetFileEndPoint;
	PIS_COMPRESSED		IsCompressed;
};


/******************************************************************************/
// 読みとり
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

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


// ID3 v1.1
#define  ID3_TAG_SIZE		128
#define  ID3_DATA_SIZE		30
#define  ID3_YEAR_SIZE		4
#define  TRACK_NUM_UNDEF	-1

#include <string>
struct ID3Tag
{
	string		strTrackName ;
	string		strArtistName ;
	string		strAlbumName ;
	int		intYear ;
	string		strComment ;
	int		intTrackNum ;
	int		intGenre ;
} ;


#endif