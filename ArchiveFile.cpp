
// ArchiveFile.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ArchiveFile.h"
#include "Mp3File.h"
#include "CueFile.h"
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
// 概要：なし。
// 補足：なし。
//============================================================================//

ArchiveFile::ArchiveFile( const string& s, UINT u)
: strArchivePath(s), intKindOfArchive(-1)
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

ArchiveFile::~ArchiveFile() 
{
	for( int i = 0; i < vecChildFile.size(); i++)
	{
		File* pFile = (File*)vecChildFile[i];
		delete vecChildFile[ i] ;
	}
}


/******************************************************************************/
//		ヘッダ解析
/******************************************************************************/
// 読みとり
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ArchiveFile::ReadHeader()
{
	// ヘッダ情報の読み取り
	HINSTANCE	hDll;
	HeaderDllFunc	func;
	int		i;

	// ファイルが読み取れるかチェック
	FILE* file = fopen(strArchivePath.c_str(), "r");
	if(!file)
	{
		status = Status::OPEN_ERROR;
		return FALSE;
	}
	fclose(file);

	for(i = 0; i < Profile::vecHeaderDll.size(); i++)
	{
		if(!Profile::vecUseHeaderDll[i])
		{
			continue;
		}

		string st = Profile::vecHeaderDll[i];
		hDll = LoadLibrary(Profile::vecHeaderDll[i].c_str());
		if(hDll == NULL)
		{
			continue;
		}

		if(!LoadFunction(hDll, &func) || !func.ReadHeader(strArchivePath.c_str()))
		{
			FreeLibrary(hDll);
			continue;
		}

		// ヘッダーDLLがファイルを読み取れたとき
		intKindOfArchive = i;
		break;
	}
	if(i == Profile::vecHeaderDll.size())
	{
		status = Status::NO_HEADER;
		return FALSE;
	}

	// 各ファイルの読み取り
	BOOL blnCompressed = TRUE;
	vecHeadMilisec.push_back(0) ;
	for(i = 0; i < func.GetFileCount(); i++)
	{
		// ファイルオブジェクトの作成
		FileInfo fileinfo;
		fileinfo.strArchivePath	= strArchivePath;
		fileinfo.strFilePath	= func.GetFileName(i);
		fileinfo.blnCompressed	= func.IsCompressed(i);
		fileinfo.uiStartPoint	= func.GetFileStartPoint(i);
		fileinfo.uiEndPoint	= func.GetFileEndPoint(i);

		File* pFile = CreateFileObject(&fileinfo);
		if(blnCompressed && !fileinfo.blnCompressed)
		{
			blnCompressed = FALSE;
		}
		vecChildFile.push_back(pFile);
		pFile->ReadHeader();

		vecHeadMilisec.push_back(vecHeadMilisec[ i] + vecChildFile[ i]->GetPlayLength());
	}

	status = (blnCompressed ? Status::COMPRESSED : Status::UNCOMPRESSED);
/*	// 読みとり
	ReadEndCentralDirRec( fzip) ;
	fclose( fzip) ;

	UINT ulHeaderPos = ulOffsetStartCentralDir ;
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
				vecChildFile.push_back( pMp3File) ;
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
				vecChildFile.push_back( pFile) ;
			}
			else
			{
				vecChildFile.push_back( pFile) ;
			}
		}
		else
		{
			vecChildFile.push_back( pFile) ;
		}
	}

	// CUE ファイルがある場合は、曲の長さを変更
	if( pCueFile)
	{
		pCueFile->CalcLength( vecChildFile) ;
	}

	// 曲の長さキャッシュ作成
	vecHeadMilisec.assign( usTotalEntriesCentralDir + 1) ;
	vecHeadMilisec[ 0] = 0 ;
	for( i = 0; i < usTotalEntriesCentralDir; i++)
	{
		vecHeadMilisec[ i + 1] = vecHeadMilisec[ i] + vecChildFile[ i]->GetPlayLength() ;
	}
*/

	return TRUE ;
}


/******************************************************************************/
// 関数へのポインタをまとめて取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ArchiveFile::LoadFunction(HINSTANCE hDll, HeaderDllFunc* pfunc)
{
	pfunc->GetDllVersion		= (PGET_DLL_VERSION)		GetProcAddress(hDll, "GetDllVersion");
	pfunc->GetDllType		= (PGET_DLL_TYPE)		GetProcAddress(hDll, "GetDllType");
	pfunc->ReadHeader		= (PREAD_HEADER)		GetProcAddress(hDll, "ReadHeader");
	pfunc->GetFileCount		= (PGET_FILE_COUNT)		GetProcAddress(hDll, "GetFileCount");
	pfunc->GetFileName		= (PGET_FILE_NAME)		GetProcAddress(hDll, "GetFileName");
	pfunc->GetFileStartPoint	= (PGET_FILE_START_POINT)	GetProcAddress(hDll, "GetFileStartPoint");
	pfunc->GetFileEndPoint		= (PGET_FILE_END_POINT)		GetProcAddress(hDll, "GetFileEndPoint");
	pfunc->IsCompressed		= (PIS_COMPRESSED)		GetProcAddress(hDll, "IsCompressed");

	if(pfunc->GetDllVersion		== NULL || 
	   pfunc->GetDllType		== NULL || 
	   pfunc->ReadHeader		== NULL || 
	   pfunc->GetFileCount		== NULL || 
	   pfunc->GetFileName		== NULL || 
	   pfunc->GetFileStartPoint	== NULL || 
	   pfunc->GetFileEndPoint	== NULL || 
	   pfunc->IsCompressed		== NULL)
	{
		return FALSE;
	}

	if(pfunc->GetDllVersion() != 0x10)
	{
		return FALSE;
	}

	return TRUE;
}


/******************************************************************************/
// ファイルオブジェクトの作成
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

File* ArchiveFile::CreateFileObject(FileInfo* pFileInfo) const
{
	string s = pFileInfo->strFilePath;
	
	if(s.size() > 4 && stricmp(s.substr( s.size() - 4).c_str(), ".mp3") == 0)
	{
		Mp3File* p = new Mp3File(pFileInfo);
		return p;
	}
	else if(s.size() > 4 && stricmp(s.substr( s.size() - 4).c_str(), ".cue") == 0)
	{
		CueFile* p = new CueFile(pFileInfo);
		return p;
	}
	else
	{
		File* p = new File(pFileInfo);
		return p;
	}
}


/******************************************************************************/
//		データ取得
/******************************************************************************/
// 再生時間取得
//============================================================================//
// 概要：zipファイルのトータルの再生時間を取得。
// 補足：なし。
//============================================================================//

UINT ArchiveFile::GetPlayLength()
{
	UINT ulPlayLength = 0 ;

	for( int i = 0; i < vecChildFile.size(); i++)
	{
		if( !vecChildFile[ i])
		{
			ulPlayLength += vecChildFile[ i]->GetPlayLength() ;
		}
	}

	return ulPlayLength ;
}


/******************************************************************************/
// ファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

string ArchiveFile::GetFileName( int i)
{
	if( i < 0 && i >= vecChildFile.size())
	{
		return "" ;
	}
	return vecChildFile[ i]->GetFileName() ;
}


/******************************************************************************/
// 子供ファイルの取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

File* ArchiveFile::GetChildFile( int i)
{
	if( i < 0 && i >= vecChildFile.size())
	{
		return NULL ;
	}
	return vecChildFile[ i] ;
}


/******************************************************************************/
// 子供ファイルの数を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int ArchiveFile::GetChildFileCount() const
{
	return vecChildFile.size() ;
}


/******************************************************************************/
// ミリ秒から曲番号を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

int ArchiveFile::GetSongIndex( UINT ulMilisec) const
{
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
// 曲の開始時刻を取得
//============================================================================//
// 概要：なし。
// 補足：要素数を引数に与えると曲長を返す。
//============================================================================//

UINT ArchiveFile::GetSongHead( int intNum) const
{
	if( intNum < 0 || intNum > vecChildFile.size())
	{
		return 0 ;
	}

	if(vecHeadMilisec.size() != vecChildFile.size() + 1)
	{
		return 0;
	}

	return vecHeadMilisec[ intNum] ;
}


/******************************************************************************/
// 曲の時間を取得
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT ArchiveFile::GetSongTime( int intSongIndex, UINT ulMilisec) const
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