
// ArchiveFile.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __ARCHIVE_FILE_H__
#define  __ARCHIVE_FILE_H__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		�N���X��`
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

// �q��
	vector<string>	vecHeaderDll;
	vector<File*>	vecChildFile;
	vector<UINT>	vecHeadMilisec;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ArchiveFile( const string&, UINT u = 0) ;
	~ArchiveFile() ;

// �w�b�_���
	BOOL ReadHeader() ;
private:
	BOOL LoadFunction(HINSTANCE, HeaderDllFunc*);
	File* CreateFileObject(FileInfo*) const;

public:
// �f�[�^�擾
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