
// ZipFile.h
//============================================================================//
// 更新：02/12/27(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ZIP_FILE_H__
#define  __ZIP_FILE_H__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ZipFile : public File
{
public:
	enum Status
	{
		OPEN_ERROR,
		NOT_ZIP,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
// zip の情報
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

// 子供
	vector< File*>	vecChildList ;
	vector< ULONG>	vecHeadMilisec ;

public:
// コンストラクタおよびデストラクタ
	ZipFile( const string&, ULONG u = 0) ;
	~ZipFile() ;

// ヘッダ解析
	BOOL ReadHeader() ;
private:
	BOOL ReadEndCentralDirRec( FILE*) ;
	BOOL GetEndCentralDirRecPos( FILE*) ;
	BOOL ReadCentralDirectory( FILE*) ;

public:
// データ取得
	ULONG	GetPlayLength() ;
	string	GetFileName( int) ;
	File*	GetChildFile( int) ;
	int	GetChildFileCount() const ;
	Status	GetStatus() const{ return status ;}
	int	GetSongIndex( ULONG ulMilisec) const ;
	ULONG	GetSongHead( int) const ;
	ULONG	GetSongTime( int, ULONG) const ;
} ;

#endif