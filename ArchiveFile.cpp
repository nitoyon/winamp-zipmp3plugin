
// ArchiveFile.cpp
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ArchiveFile.h"
#include "Mp3File.h"
#include "CueFile.h"
#include "define.h"
#include "Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  BUF_SIZE		4096


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ArchiveFile::ArchiveFile( const string& s, UINT u)
: strArchivePath(s), intKindOfArchive(-1)
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ArchiveFile::~ArchiveFile() 
{
	for( int i = 0; i < vecChildFile.size(); i++)
	{
		File* pFile = (File*)vecChildFile[i];
		delete vecChildFile[ i] ;
	}
}


/******************************************************************************/
//		�w�b�_���
/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ArchiveFile::ReadHeader()
{
	// �w�b�_���̓ǂݎ��
	HINSTANCE	hDll;
	HeaderDllFunc	func;
	int		i;

	// �t�@�C�����ǂݎ��邩�`�F�b�N
	FILE* file = fopen(strArchivePath.c_str(), "r");
	if(!file)
	{
		status = Status::OPEN_ERROR;
		return FALSE;
	}
	fclose(file);

	for(i = 0; i < Profile::vecHeaderDll.size(); i++)
	{
		if(!Profile::vecUseHeaderDll[i])
		{
			continue;
		}

		string st = Profile::vecHeaderDll[i];
		hDll = LoadLibrary(Profile::vecHeaderDll[i].c_str());
		if(hDll == NULL)
		{
			continue;
		}

		if(!LoadFunction(hDll, &func) || !func.ReadHeader(strArchivePath.c_str()))
		{
			FreeLibrary(hDll);
			continue;
		}

		// �w�b�_�[DLL���t�@�C����ǂݎ�ꂽ�Ƃ�
		intKindOfArchive = i;
		break;
	}
	if(i == Profile::vecHeaderDll.size())
	{
		status = Status::NO_HEADER;
		return FALSE;
	}

	// �e�t�@�C���̓ǂݎ��
	BOOL blnCompressed = TRUE;
	vecHeadMilisec.push_back(0) ;
	for(i = 0; i < func.GetFileCount(); i++)
	{
		// �t�@�C���I�u�W�F�N�g�̍쐬
		FileInfo fileinfo;
		fileinfo.strArchivePath	= strArchivePath;
		fileinfo.strFilePath	= func.GetFileName(i);
		fileinfo.blnCompressed	= func.IsCompressed(i);
		fileinfo.uiStartPoint	= func.GetFileStartPoint(i);
		fileinfo.uiEndPoint	= func.GetFileEndPoint(i);

		File* pFile = CreateFileObject(&fileinfo);
		if(blnCompressed && !fileinfo.blnCompressed)
		{
			blnCompressed = FALSE;
		}
		vecChildFile.push_back(pFile);
		pFile->ReadHeader();

		vecHeadMilisec.push_back(vecHeadMilisec[ i] + vecChildFile[ i]->GetPlayLength());
	}

	status = (blnCompressed ? Status::COMPRESSED : Status::UNCOMPRESSED);
/*	// �ǂ݂Ƃ�
	ReadEndCentralDirRec( fzip) ;
	fclose( fzip) ;

	UINT ulHeaderPos = ulOffsetStartCentralDir ;
	status = Status::COMPRESSED ;
	CueFile* pCueFile = NULL ;

	for( UINT i = 0; i < usTotalEntriesCentralDir; i++)
	{
		// �e�t�@�C���̃w�b�_�ǂ݂Ƃ�
		File* pFile = new File( strZipPath, ulHeaderPos) ;
		if( !pFile->ReadHeader())
		{
			status = Status::INVALID_HEADER ;
			return FALSE ;
		}
		ulHeaderPos += pFile->GetCentralDirSize() ;

		// �����k��
		if( !pFile->IsCompressed())
		{
			// ��ł������k�̃t�@�C��������΁A�����k�Ƃ���
			status = Status::UNCOMPRESSED ;

			// �g���q�� mp3 �̎�
			string s = pFile->GetFilePath() ;
			if( s.size() > 4 && stricmp( s.substr( s.size() - 4).c_str(), ".mp3") == 0)
			{
				// MP3 �̏����擾
				Mp3File* pMp3File = new Mp3File( pFile) ;
				pMp3File->ReadHeader() ;
				delete pFile ;
				vecChildFile.push_back( pMp3File) ;
			}
			// �g���q�� cue �̂Ƃ�
			else if( s.size() > 4 && stricmp( s.substr( s.size() - 4).c_str(), ".cue") == 0)
			{
				if( pCueFile)
				{
					delete pCueFile ;
				}
				pCueFile = new CueFile( pFile) ;
				pCueFile->ReadCueFile() ;
				vecChildFile.push_back( pFile) ;
			}
			else
			{
				vecChildFile.push_back( pFile) ;
			}
		}
		else
		{
			vecChildFile.push_back( pFile) ;
		}
	}

	// CUE �t�@�C��������ꍇ�́A�Ȃ̒�����ύX
	if( pCueFile)
	{
		pCueFile->CalcLength( vecChildFile) ;
	}

	// �Ȃ̒����L���b�V���쐬
	vecHeadMilisec.assign( usTotalEntriesCentralDir + 1) ;
	vecHeadMilisec[ 0] = 0 ;
	for( i = 0; i < usTotalEntriesCentralDir; i++)
	{
		vecHeadMilisec[ i + 1] = vecHeadMilisec[ i] + vecChildFile[ i]->GetPlayLength() ;
	}
*/

	return TRUE ;
}


/******************************************************************************/
// �֐��ւ̃|�C���^���܂Ƃ߂Ď擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ArchiveFile::LoadFunction(HINSTANCE hDll, HeaderDllFunc* pfunc)
{
	pfunc->GetDllVersion		= (PGET_DLL_VERSION)		GetProcAddress(hDll, "GetDllVersion");
	pfunc->GetDllType		= (PGET_DLL_TYPE)		GetProcAddress(hDll, "GetDllType");
	pfunc->ReadHeader		= (PREAD_HEADER)		GetProcAddress(hDll, "ReadHeader");
	pfunc->GetFileCount		= (PGET_FILE_COUNT)		GetProcAddress(hDll, "GetFileCount");
	pfunc->GetFileName		= (PGET_FILE_NAME)		GetProcAddress(hDll, "GetFileName");
	pfunc->GetFileStartPoint	= (PGET_FILE_START_POINT)	GetProcAddress(hDll, "GetFileStartPoint");
	pfunc->GetFileEndPoint		= (PGET_FILE_END_POINT)		GetProcAddress(hDll, "GetFileEndPoint");
	pfunc->IsCompressed		= (PIS_COMPRESSED)		GetProcAddress(hDll, "IsCompressed");

	if(pfunc->GetDllVersion		== NULL || 
	   pfunc->GetDllType		== NULL || 
	   pfunc->ReadHeader		== NULL || 
	   pfunc->GetFileCount		== NULL || 
	   pfunc->GetFileName		== NULL || 
	   pfunc->GetFileStartPoint	== NULL || 
	   pfunc->GetFileEndPoint	== NULL || 
	   pfunc->IsCompressed		== NULL)
	{
		return FALSE;
	}

	if(pfunc->GetDllVersion() != 0x10)
	{
		return FALSE;
	}

	return TRUE;
}


/******************************************************************************/
// �t�@�C���I�u�W�F�N�g�̍쐬
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File* ArchiveFile::CreateFileObject(FileInfo* pFileInfo) const
{
	string s = pFileInfo->strFilePath;
	
	if(s.size() > 4 && stricmp(s.substr( s.size() - 4).c_str(), ".mp3") == 0)
	{
		Mp3File* p = new Mp3File(pFileInfo);
		return p;
	}
	else if(s.size() > 4 && stricmp(s.substr( s.size() - 4).c_str(), ".cue") == 0)
	{
		CueFile* p = new CueFile(pFileInfo);
		return p;
	}
	else
	{
		File* p = new File(pFileInfo);
		return p;
	}
}


/******************************************************************************/
//		�f�[�^�擾
/******************************************************************************/
// �Đ����Ԏ擾
//============================================================================//
// �T�v�Fzip�t�@�C���̃g�[�^���̍Đ����Ԃ��擾�B
// �⑫�F�Ȃ��B
//============================================================================//

UINT ArchiveFile::GetPlayLength()
{
	UINT ulPlayLength = 0 ;

	for( int i = 0; i < vecChildFile.size(); i++)
	{
		if( !vecChildFile[ i])
		{
			ulPlayLength += vecChildFile[ i]->GetPlayLength() ;
		}
	}

	return ulPlayLength ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string ArchiveFile::GetFileName( int i)
{
	if( i < 0 && i >= vecChildFile.size())
	{
		return "" ;
	}
	return vecChildFile[ i]->GetFileName() ;
}


/******************************************************************************/
// �q���t�@�C���̎擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

File* ArchiveFile::GetChildFile( int i)
{
	if( i < 0 && i >= vecChildFile.size())
	{
		return NULL ;
	}
	return vecChildFile[ i] ;
}


/******************************************************************************/
// �q���t�@�C���̐����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ArchiveFile::GetChildFileCount() const
{
	return vecChildFile.size() ;
}


/******************************************************************************/
// �~���b����Ȕԍ����擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int ArchiveFile::GetSongIndex( UINT ulMilisec) const
{
	for( int i = 0; i < vecHeadMilisec.size() - 1; i++)
	{
		if( ulMilisec < vecHeadMilisec[ i + 1])
		{
			return i ;
		}
	}

	return vecHeadMilisec.size() - 1 ;
}


/******************************************************************************/
// �Ȃ̊J�n�������擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�v�f���������ɗ^����ƋȒ���Ԃ��B
//============================================================================//

UINT ArchiveFile::GetSongHead( int intNum) const
{
	if( intNum < 0 || intNum > vecChildFile.size())
	{
		return 0 ;
	}

	if(vecHeadMilisec.size() != vecChildFile.size() + 1)
	{
		return 0;
	}

	return vecHeadMilisec[ intNum] ;
}


/******************************************************************************/
// �Ȃ̎��Ԃ��擾
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

UINT ArchiveFile::GetSongTime( int intSongIndex, UINT ulMilisec) const
{
	if( intSongIndex < 0 || intSongIndex >= vecHeadMilisec.size() - 1)
	{
		return 0 ;
	}

	if( Profile::blnCountUp)
	{
		return ulMilisec - vecHeadMilisec[ intSongIndex] ;
	}
	else
	{
		return vecHeadMilisec[ intSongIndex + 1] - ulMilisec ;
	}
}