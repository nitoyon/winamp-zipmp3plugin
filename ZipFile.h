
// ZipFile.h
//============================================================================//
// �X�V�F02/12/22(��)
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

// �q��
	vector<ZipChild*>	vecZipChild ;
	vector<File*>		vecChildFile ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ZipFile( const string&, ULONG u = 0) ;
	~ZipFile() ;

// �w�b�_���
	void ReadHeader() ;
private:
	BOOL ReadEndCentralDirRec( FILE*) ;
	BOOL GetEndCentralDirRecPos( FILE*) ;
	BOOL ReadCentralDirectory( FILE*) ;

public:
// �f�[�^�擾
	File* GetFileInfo( int) ;
	ULONG GetPlayLength() ;
	string GetFileName( int) ;
	File* GetChildFile( int) ;
	int GetChildFileCount() const ;
} ;

#endif