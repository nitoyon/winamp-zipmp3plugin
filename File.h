
// File.h
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __FILE_H__
#define  __FILE_H__

#include "include.h"
#include "define.h"


/******************************************************************************/
//		�\���̒�`
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
//		�N���X��`
/******************************************************************************/

class File
{
protected:
// �t�@�C�����
	string		strArchivePath;
	string		strFilePath;
	BOOL		blnCompressed;
	UINT		uiStartPoint;
	UINT		uiEndPoint;

public:
// �R���X�g���N�^����уf�X�g���N�^
	//File(const string&, UINT _uiStart = 0, UINT _uiEnd = 0);
	File(FileInfo*);
	~File() ;

// �t�@�C�����擾
	string GetArchivePath() const{ return strArchivePath ;}
	string GetFilePath() const{ return strFilePath;};
	string GetFileName() const;
	string GetFileDir() const;
	BOOL IsCompressed() const{return blnCompressed;}
	UINT GetStartPoint() const{return uiStartPoint;}
	UINT GetEndPoint() const{return uiEndPoint;}

	virtual ULONG GetPlayLength() ;
	virtual BOOL ReadHeader();


// �\�����擾
	string GetDisplayStr(const string&) ;
	virtual BOOL HasID3Tag() const{return FALSE;}
	virtual const ID3Tag GetID3Tag() const{ID3Tag i ;return i;}

protected:
	virtual string GetVariable( const string&) ;
};

#endif