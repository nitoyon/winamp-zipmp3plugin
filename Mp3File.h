
// Mp3File.h
//============================================================================//
// �X�V�F02/12/28(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __MP3_FILE_H__
#define  __MP3_FILE_H__

#include "include.h"
#include "define.h"
#include "File.h"


/******************************************************************************/
//		�N���X��`
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
// �R���X�g���N�^����уf�X�g���N�^
	Mp3File( File*) ;
	~Mp3File() ;

// �w�b�_������擾
	BOOL ReadHeader() ;
private:
	BOOL FindMpegHeader( FILE*) ;
	BOOL ReadMpegHeader( FILE*) ;
	void ReadID3v1( FILE*) ;
	string StripSpace( const char*) ;

public:
// �����擾
	ULONG GetPlayLength() ;
	string GetVariable( const string&) ;

// ID3v1 �^�O�擾
	BOOL	HasID3Tag() const{ return blnHasID3Tag ;}
	const ID3Tag GetID3Tag() const{ return id3tag ;}
} ;

#endif