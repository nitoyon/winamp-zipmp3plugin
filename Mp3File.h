
// Mp3File.h
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MP3_FILE_H__
#define  __MP3_FILE_H__

#include "include.h"
#include "define.h"
#include "File.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Mp3File : public File
{
private:
	ULONG		ulMpegHeader ;
	int		intMpeg ;	// 1 : MPEG1, 2 : MPEG2, 3: MPEG2.5
	int		intLayer ;
	int		intBitrate ;

	BOOL		blnHasID3Tag ;
	ID3Tag		id3tag ;

public:
// コンストラクタおよびデストラクタ
	Mp3File( File*) ;
	~Mp3File() ;

// ヘッダから情報取得
	BOOL ReadHeader() ;
private:
	BOOL FindMpegHeader( FILE*) ;
	BOOL ReadMpegHeader( FILE*) ;
	void ReadID3v1( FILE*) ;
	string StripSpace( const char*) ;

public:
// 長さ取得
	ULONG GetPlayLength() ;
	string GetVariable( const string&) ;

// ID3v1 タグ取得
	BOOL	HasID3Tag() const{ return blnHasID3Tag ;}
	const ID3Tag GetID3Tag() const{ return id3tag ;}
} ;

#endif