
// Mp3File.h
//============================================================================//
// �X�V�F02/12/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __MP3_FILE_H__
#define  __MP3_FILE_H__

#include "include.h"
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

public:
// �R���X�g���N�^����уf�X�g���N�^
	Mp3File( File*) ;
	~Mp3File() ;

// �w�b�_������擾
	BOOL ReadHeader() ;
private:
	BOOL FindMpegHeader( FILE*) ;
	BOOL ReadMpegHeader( FILE*) ;

// �����擾
	ULONG GetPlayLength() ;
} ;

#endif