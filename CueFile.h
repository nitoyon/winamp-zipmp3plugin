
// CueFile.h
//============================================================================//
// 更新：03/03/16(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __CUE_FILE_H__
#define  __CUE_FILE_H__

#include "include.h"
#include "define.h"
#include "File.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class CueFile : public File
{
private:
	string	strData ;

public:
// コンストラクタおよびデストラクタ
	CueFile( File*) ;
	~CueFile() ;

// ヘッダから情報取得
	void ReadCueFile() ;
	void Compile( vector< UINT>*) ;
	void CalcLength( const vector< File*>& vecChildList) ;
} ;

#endif