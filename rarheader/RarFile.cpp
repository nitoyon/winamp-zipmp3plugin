
// RarFile.cpp
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "RarFile.h"


/******************************************************************************/
//		スタティック変数
/******************************************************************************/

RarFile* RarFile::pInstance = NULL;


/******************************************************************************/
//		定義
/******************************************************************************/


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// デストラクタ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// インスタンス取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
//		ヘッダ解析
/******************************************************************************/
// 読みとり
//============================================================================//
// 更新：03/11/28(金)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL RarFile::_ReadHeader()
{
	frar = fopen( strRarPath.c_str(), "rb") ;
	if( !frar || fseek(frar, 0, SEEK_SET) != 0)
	{
		status = Status::OPEN_ERROR ;
		return FALSE ;
	}

	// marker blockをチェック
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

	// archive headerをチェック
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
		// 各ファイルのヘッダ読みとり
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
			
			// 無圧縮時
			if( !child.blnCompressed)
			{
				// 一つでも無圧縮のファイルがあれば、無圧縮とする
				status = Status::UNCOMPRESSED ;
			}

			if( fileblkhdr.ulPACK_SIZE )
			{
				// フォルダを無視する
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
//		情報取得
/******************************************************************************/
// ファイル数
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

UINT RarFile::GetChildCount() const
{
	return vecChildList.size();
}


/******************************************************************************/
// ファイル名取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ファイルの始点取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// ファイルの終点取得
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// 圧縮されているか
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL RarFile::_IsCompressed(UINT ui) const
{
	if(ui >= GetChildCount())
	{
		return 0;
	}

	return vecChildList[ui].blnCompressed;
}
