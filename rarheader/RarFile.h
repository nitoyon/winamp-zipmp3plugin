
// RarFile.h
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __RAR_FILE_H__
#define  __RAR_FILE_H__

#include <windows.h>
#include <vector>
#include <string>

using namespace std;



/******************************************************************************/
//		�q�t�@�C�����̒�`
/******************************************************************************/

struct ChildFile
{
	UINT	uiStart;
	UINT	uiEnd;		// ���̃A�C�e���̐擪�A�h���X
	string	strFileName;
	BOOL	blnCompressed;
};


/******************************************************************************/
//		RAR �̃t�@�C�����
/******************************************************************************/

struct RarBlockHeader
{
	USHORT	usHEAD_CRC;
	BYTE	byteHEAD_TYPE;
	USHORT	usHEAD_FLAGS;
	USHORT	usHEAD_SIZE;
} ;

struct RarFileBlockHeader
{
	ULONG	ulPACK_SIZE;
	ULONG	ulUNP_SIZE;
	BYTE	byteHOST_OS;
	ULONG	ulFILE_CRC;
	ULONG	ulFTIME;
	BYTE	byteUNP_VER;
	BYTE	byteMETHOD;
	USHORT	usNAME_SIZE;
	ULONG	ulATTR;
} ;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class RarFile
{
public:
	enum Status
	{
		OPEN_ERROR,
		NOT_RAR,	INVALID_HEADER, 
		COMPRESSED,	UNCOMPRESSED
	} ;

private:
// Singlton
	static RarFile* pInstance;

// rar �̏��
	string	strRarPath;

	ULONG	ulEof ;
	RarBlockHeader blkhdr;
	RarFileBlockHeader fileblkhdr;
	Status	status ;

// �t�@�C��
	FILE*	frar;

// �q��
	vector<ChildFile>	vecChildList ;
	vector< ULONG>	vecHeadMilisec ;

// �R���X�g���N�^����уf�X�g���N�^
private:
	RarFile( const string&) ;
public:
	~RarFile() ;
	static RarFile* GetInstance( const string& s = "") ;

// �w�b�_���
	BOOL _ReadHeader() ;
private:

// �ǂݎ��֐�
//	USHORT makeword( BYTE* p);
//	ULONG  makelong( BYTE* p);

public:
// �f�[�^�擾
	Status	GetStatus() const{ return status ;}

	string	GetChildName(UINT) const;
	UINT	GetChildCount() const ;
	ULONG	GetChildStartPoint(UINT) const ;
	ULONG	GetChildEndPoint(UINT) const ;
	BOOL	_IsCompressed(UINT) const ;
} ;

#endif