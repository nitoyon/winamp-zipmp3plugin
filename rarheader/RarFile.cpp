
// RarFile.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "RarFile.h"


/******************************************************************************/
//		�X�^�e�B�b�N�ϐ�
/******************************************************************************/

RarFile* RarFile::pInstance = NULL;


/******************************************************************************/
//		��`
/******************************************************************************/


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

RarFile::RarFile( const string& s)
: ulEof( 0)
{
	strRarPath = s;
	
	frar = fopen( s.c_str(), "rb") ;
	if( frar)
	{
		if( fseek( frar, 0, SEEK_END) == 0)
		{
			ulEof = ftell( frar) ;
		}

		fclose( frar) ;
		frar = NULL ;
	}
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

RarFile::~RarFile() 
{
	if( frar)
	{
		fclose( frar) ;
		frar = NULL ;
	}
}


/******************************************************************************/
// �C���X�^���X�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

RarFile* RarFile::GetInstance(const string& s)
{
	if(s != "")
	{
		if(!pInstance)
		{
			pInstance = new RarFile(s);
		}
		else
		{
			delete pInstance;
			pInstance = new RarFile(s);
		}
	}

	return pInstance;
}


/******************************************************************************/
//		�w�b�_���
/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �X�V�F03/11/28(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL RarFile::_ReadHeader()
{
	frar = fopen( strRarPath.c_str(), "rb") ;
	if( !frar || fseek(frar, 0, SEEK_SET) != 0)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// marker block���`�F�b�N
	fread( &blkhdr, sizeof(blkhdr), 1, frar);
	if( blkhdr.usHEAD_CRC == 0x6152
	 && blkhdr.byteHEAD_TYPE == 0x72
	 && blkhdr.usHEAD_FLAGS == 0x1a21
	 && blkhdr.usHEAD_SIZE == 0x0007)
	{
		// ok
	}
	else
	{
		status = Status::NOT_RAR ;
		fclose( frar) ;
		frar = NULL ;
		return FALSE ;
	}

	// archive header���`�F�b�N
	fread( &blkhdr, sizeof(blkhdr), 1, frar);
	if( blkhdr.byteHEAD_TYPE == 0x73
	 && !(blkhdr.usHEAD_FLAGS & 0x0080) )
	{
		// skip contents(comments etc.)
		fseek(frar, blkhdr.usHEAD_SIZE - sizeof(blkhdr), SEEK_CUR);
	}
	else
	{
		status = Status::INVALID_HEADER ;
		fclose( frar) ;
		frar = NULL ;
		return FALSE ;
	}

	status = Status::COMPRESSED ;

	while( 1 )
	{
		// �e�t�@�C���̃w�b�_�ǂ݂Ƃ�
		blkhdr.byteHEAD_TYPE = 0x00;
		fileblkhdr.usNAME_SIZE = 0x0000;

		fread( &blkhdr, sizeof(blkhdr), 1, frar);
		if( blkhdr.byteHEAD_TYPE == 0x74
		 && !(blkhdr.usHEAD_FLAGS & 0x0504) )
		{
			// file header block
			ChildFile child;
			fread( &fileblkhdr, sizeof(fileblkhdr), 1, frar);
			if( fileblkhdr.usNAME_SIZE )
			{
				char* pszFilename = new char[fileblkhdr.usNAME_SIZE + 1];
				fread(pszFilename, 1, fileblkhdr.usNAME_SIZE, frar);
				child.strFileName = pszFilename;
				delete[] pszFilename;
			}

			child.uiStart = ftell(frar) + blkhdr.usHEAD_SIZE - sizeof(blkhdr)
				- sizeof(fileblkhdr) - fileblkhdr.usNAME_SIZE;
			child.uiEnd = child.uiStart + fileblkhdr.ulPACK_SIZE;
			child.blnCompressed = ( fileblkhdr.ulPACK_SIZE != fileblkhdr.ulUNP_SIZE );
			
			// �����k��
			if( !child.blnCompressed)
			{
				// ��ł������k�̃t�@�C��������΁A�����k�Ƃ���
				status = Status::UNCOMPRESSED ;
			}

			if( fileblkhdr.ulPACK_SIZE )
			{
				// �t�H���_�𖳎�����
				vecChildList.push_back(child) ;
			}

			fseek(frar, child.uiEnd, SEEK_SET);
		}
		else
		{
			// terminate on any non-file header, dirty but works
			break;
		}

	}

	fclose( frar) ;
	frar = NULL ;
	return TRUE ;
}


/******************************************************************************/
//		���擾
/******************************************************************************/
// �t�@�C����
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT RarFile::GetChildCount() const
{
	return vecChildList.size();
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string RarFile::GetChildName(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return "";
	}

	return vecChildList[ui].strFileName;
}


/******************************************************************************/
// �t�@�C���̎n�_�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG RarFile::GetChildStartPoint(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].uiStart;
}


/******************************************************************************/
// �t�@�C���̏I�_�擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG RarFile::GetChildEndPoint(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].uiEnd;
}


/******************************************************************************/
// ���k����Ă��邩
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL RarFile::_IsCompressed(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].blnCompressed;
}
