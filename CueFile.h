
// CueFile.h
//============================================================================//
// �X�V�F03/03/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __CUE_FILE_H__
#define  __CUE_FILE_H__

#include "include.h"
#include "define.h"
#include "File.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class CueFile : public File
{
private:
	string	strData ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	CueFile( File*) ;
	~CueFile() ;

// �w�b�_������擾
	void ReadCueFile() ;
	void Compile( vector< UINT>*) ;
	void CalcLength( const vector< File*>& vecChildList) ;
} ;

#endif