
// CueFile.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "CueFile.h"
#include "Mp3File.h"
#define  BUF_SIZE  4


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

CueFile::CueFile(FileInfo* p)
: File(p)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

CueFile::~CueFile() 
{
}



/******************************************************************************/
//		ヘッダから情報取得
/******************************************************************************/
// 
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void CueFile::ReadCueFile()
{
	ULONG ulPos = uiStartPoint;
	ULONG ulEnd = uiEndPoint;
	ULONG ulBufSize ;

	// 読みとり
	FILE* fzip = fopen(strArchivePath.c_str(), "rb") ;
	strData ;
	char pszBuf[ BUF_SIZE + 1] ;
	if( fzip)
	{
		while( 1)
		{
			if( fseek( fzip, ulPos, SEEK_SET) == 0)
			{
				ulBufSize = ( ulPos + BUF_SIZE > ulEnd ? ulEnd - ulPos : BUF_SIZE) ;
				fread( pszBuf, sizeof(char), ulBufSize, fzip) ;
				pszBuf[ ulBufSize] = '\0' ;
				strData += pszBuf ;

				ulPos += BUF_SIZE ;
				if( ulPos > ulEnd)
				{
					break ;
				}
			}
		}

		fclose( fzip) ;
	}
}


/******************************************************************************/
// データを評価
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void CueFile::Compile( vector<UINT>* pvecLength)
{
	UINT	uiStart = 0 ;
	UINT	uiNext ;
	string	strLine ;

	while( TRUE)
	{
		uiNext = strData.find( '\n', uiStart) ;
		strLine = strData.substr( uiStart, uiNext - uiStart) ;
		if( strLine == "")
		{
			break ;
		}

		// 空白文字をスキップ
		UINT uiPos = strLine.find_first_not_of( ' ', 0) ;
		strLine = strLine.substr( uiPos) ;
		if( strLine != "")
		{
			// INDEX の秒を取得
			if( strLine.find( "INDEX 01 ") == 0)
			{
				strLine = strLine.substr( 9) ;
				UINT uiMin, uiSec, uiMil ;
				sscanf( strLine.c_str(), "%d:%d:%d", &uiMin, &uiSec, &uiMil) ;
				pvecLength->push_back( uiMin * 60000 + uiSec * 1000 + uiMil * 10) ;
			}
		}

		if(uiNext == string::npos || uiNext + 1 > strData.size())
		{
			break ;
		}
		uiStart = uiNext + 1 ;
	}

	// 最終曲の長さを設定
	pvecLength->push_back( -1) ;
}


/******************************************************************************/
// 長さを設定
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

void CueFile::CalcLength( const vector< File*>& vecChildList)
{
	vector<UINT> vecLength ;
	Compile( &vecLength) ;

	int intCount = 0 ;

	for( int i = 0; i < vecChildList.size(); i++)
	{
		Mp3File* pMp3File = dynamic_cast<Mp3File*>( vecChildList[ i]) ;
		if( pMp3File)
		{
			pMp3File->SetPlayLength( vecLength[ intCount + 1] - vecLength[ intCount]) ;
			intCount++ ;
		}
	}
}

