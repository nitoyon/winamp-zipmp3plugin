
// ArchiveFile.h
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ARCHIVE_FILE_H__
#define  __ARCHIVE_FILE_H__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ArchiveFile
{
public:
	enum Status
	{
		OPEN_ERROR,
		NO_HEADER,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
	Status	status ;

	string	strArchivePath;
	int	intKindOfArchive;

// 子供
	vector<string>	vecHeaderDll;
	vector<File*>	vecChildFile;
	vector<UINT>	vecHeadMilisec;

public:
// コンストラクタおよびデストラクタ
	ArchiveFile( const string&, UINT u = 0) ;
	~ArchiveFile() ;

// ヘッダ解析
	BOOL ReadHeader() ;
private:
	BOOL LoadFunction(HINSTANCE, HeaderDllFunc*);
	File* CreateFileObject(FileInfo*) const;

public:
// データ取得
	UINT	GetPlayLength() ;
	string	GetFileName( int) ;
	File*	GetChildFile( int) ;
	int	GetChildFileCount() const ;
	Status	GetStatus() const{ return status ;}
	int	GetSongIndex( UINT ulMilisec) const ;
	UINT	GetSongHead( int) const ;
	UINT	GetSongTime( int, UINT) const ;
} ;

#endif