
// File.h
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"
#include "define.h"


/******************************************************************************/
//		構造体定義
/******************************************************************************/

struct FileInfo
{
	string	strArchivePath;
	string	strFilePath;
	BOOL	blnCompressed;
	UINT	uiStartPoint;
	UINT	uiEndPoint;
};


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class File
{
protected:
// ファイル情報
	string		strArchivePath;
	string		strFilePath;
	BOOL		blnCompressed;
	UINT		uiStartPoint;
	UINT		uiEndPoint;

public:
// コンストラクタおよびデストラクタ
	//File(const string&, UINT _uiStart = 0, UINT _uiEnd = 0);
	File(FileInfo*);
	~File() ;

// ファイル情報取得
	string GetArchivePath() const{ return strArchivePath ;}
	string GetFilePath() const{ return strFilePath;};
	string GetFileName() const;
	string GetFileDir() const;
	BOOL IsCompressed() const{return blnCompressed;}
	UINT GetStartPoint() const{return uiStartPoint;}
	UINT GetEndPoint() const{return uiEndPoint;}

	virtual ULONG GetPlayLength() ;
	virtual BOOL ReadHeader();


// 表示名取得
	string GetDisplayStr(const string&) ;
	virtual BOOL HasID3Tag() const{return FALSE;}
	virtual const ID3Tag GetID3Tag() const{ID3Tag i ;return i;}

protected:
	virtual string GetVariable( const string&) ;
};

#endif