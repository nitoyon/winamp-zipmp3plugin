
// ZipFile.h
//============================================================================//
// �X�V�F02/12/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __ZIP_FILE_H__
#define  __ZIP_FILE_H__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class ZipFile : public File
{
public:
	enum Status
	{
		OPEN_ERROR,
		NOT_ZIP,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
// zip �̏��
	ULONG	ulEof ;
	ULONG	ulEndCentralDirRec ;
	USHORT	usNumOfThisDisk ;
	USHORT	usNumDiskWithStartCentralDir ;
	USHORT	usNumEntriesCentrlDirThisDisk ;
	USHORT	usTotalEntriesCentralDir ;
	ULONG	ulSizeCentralDir ;
	ULONG	ulOffsetStartCentralDir ;
	USHORT	usZipfileCommentLength ;

	Status	status ;

// �q��
	vector< File*>	vecChildList ;
	vector< ULONG>	vecHeadMilisec ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ZipFile( const string&, ULONG u = 0) ;
	~ZipFile() ;

// �w�b�_���
	BOOL ReadHeader() ;
private:
	BOOL ReadEndCentralDirRec( FILE*) ;
	BOOL GetEndCentralDirRecPos( FILE*) ;
	BOOL ReadCentralDirectory( FILE*) ;

public:
// �f�[�^�擾
	ULONG	GetPlayLength() ;
	string	GetFileName( int) ;
	File*	GetChildFile( int) ;
	int	GetChildFileCount() const ;
	Status	GetStatus() const{ return status ;}
	int	GetSongIndex( ULONG ulMilisec) const ;
	ULONG	GetSongHead( int) const ;
	ULONG	GetSongTime( int, ULONG) const ;
} ;

#endif