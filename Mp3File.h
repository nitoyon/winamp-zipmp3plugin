
// Mp3File.h
//============================================================================//
// 更新：02/12/26(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MP3_FILE_H__
#define  __MP3_FILE_H__

#include "include.h"
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

public:
// コンストラクタおよびデストラクタ
	Mp3File( File*) ;
	~Mp3File() ;

// ヘッダから情報取得
	BOOL ReadHeader() ;
private:
	BOOL FindMpegHeader( FILE*) ;
	BOOL ReadMpegHeader( FILE*) ;

// 長さ取得
	ULONG GetPlayLength() ;
} ;

#endif