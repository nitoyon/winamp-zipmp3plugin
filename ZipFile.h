
// ZipFile.h
//============================================================================//
// 更新：02/12/22(日)
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

// 子供
	vector<ZipChild*>	vecZipChild ;
	vector<File*>		vecChildFile ;

public:
// コンストラクタおよびデストラクタ
	ZipFile( const string&, ULONG u = 0) ;
	~ZipFile() ;

// ヘッダ解析
	void ReadHeader() ;
private:
	BOOL ReadEndCentralDirRec( FILE*) ;
	BOOL GetEndCentralDirRecPos( FILE*) ;
	BOOL ReadCentralDirectory( FILE*) ;

public:
// データ取得
	File* GetFileInfo( int) ;
	ULONG GetPlayLength() ;
	string GetFileName( int) ;
	File* GetChildFile( int) ;
	int GetChildFileCount() const ;
} ;

#endif