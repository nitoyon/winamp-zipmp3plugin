
// Id3tagv2.h
// ID3v2 タグの実装
//============================================================================//
// 更新：03/02/01(土)
// 概要：なし。
// 補足：mp3infp のソースをベース。
//============================================================================//

#if !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)
#define AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "include.h"

static WORD ExtractI2(unsigned char buf[2])
{
	WORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	return x;
}

static DWORD ExtractI4(unsigned char buf[4])
{
	DWORD x;
	// big endian extract
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	x <<= 8;
	x |= buf[2];
	x <<= 8;
	x |= buf[3];
	return x;
}

class CId3Frame  
{
public:
	CId3Frame()
	{
		m_data = NULL;
		m_dwId = 0;
		m_dwSize = 0;
		m_wFlags = 0;
	};
	virtual ~CId3Frame()
	{
		Release();
	};
	void Release()
	{
		if(m_data)
		{
			free(m_data);
		}
		m_data = NULL;
		m_dwId = 0;
		m_dwSize = 0;
		m_wFlags = 0;
	}
	CId3Frame(const CId3Frame &obj)	//コピーコンストラクタ
	{
		m_data = (unsigned char *)malloc(obj.m_dwSize);
		memcpy(m_data,obj.m_data,obj.m_dwSize);
		m_dwId = obj.m_dwId;
		m_dwSize = obj.m_dwSize;
		m_wFlags = obj.m_wFlags;
	};
	DWORD LoadFrame(unsigned char *pData,DWORD dwSize)
	{
		Release();
		if(dwSize < 10)
		{
			return 0;	//フレームヘッダがない場合は終了
		}
		DWORD size = (((DWORD )pData[4]<<24) | ((DWORD )pData[5]<<16) | ((DWORD )pData[6]<<8) | (DWORD )pData[7]);
		if((size+10) > dwSize)
		{
			return 0;	//ヘッダサイズが入力データを超過している
		}
		DWORD dwId;
		memcpy(&dwId,pData,sizeof(dwId));
//	BYTE id[5];
//	memcpy(id,pData,sizeof(dwId));
//	id[4] = '\0';
//	TRACE("id=%s (size=%d)\n",id,size);
		if(!dwId)
		{
			return 0;	//無効なフレームID
		}
		m_data = (unsigned char *)malloc(size);
		if(!m_data)
		{
			return 0;	//メモリを確保できなかった
		}
//		memcpy(&m_dwId,pData,sizeof(m_dwId));
		m_dwSize = size;
		m_dwId = dwId;
		m_wFlags = ExtractI2(&pData[8]);
		memcpy(m_data,&pData[10],size);
		return (size + 10);
	};
	DWORD LoadFrame2_2(unsigned char *pData,DWORD dwSize)
	{
		Release();
		if(dwSize < 6)
		{
			return 0;	//フレームヘッダがない場合は終了
		}
		DWORD size = (((DWORD )pData[3]<<16) | ((DWORD )pData[4]<<8) | (DWORD )pData[5]);
		if((size+6) > dwSize)
		{
			return 0;	//ヘッダサイズが入力データを超過している
		}
		BYTE id[3+1];
		memcpy(&id,pData,sizeof(id));
		id[3] = '\0';
//		TRACE("id=%s (size=%d)\n",id,size);
		//v2.2からv2.3へフレームIDを変換
		if(memcmp(id,"TT2",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TIT2",sizeof(m_dwId));
		}
		else if(memcmp(id,"TRK",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TRCK",sizeof(m_dwId));
		}
		else if(memcmp(id,"TP1",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TPE1",sizeof(m_dwId));
		}
		else if(memcmp(id,"TAL",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TALB",sizeof(m_dwId));
		}
		else if(memcmp(id,"TYE",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TYER",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCO",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCON",sizeof(m_dwId));
		}
		else if(memcmp(id,"COM",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"COMM",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCM",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCOM",sizeof(m_dwId));
		}
		else if(memcmp(id,"TOA",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TOPE",sizeof(m_dwId));
		}
		else if(memcmp(id,"TCR",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TCOP",sizeof(m_dwId));
		}
		else if(memcmp(id,"WXX",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"WXXX",sizeof(m_dwId));
		}
		else if(memcmp(id,"TSS",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TSSE",sizeof(m_dwId));
		}
		else if(memcmp(id,"TEN",sizeof(id)) == 0)
		{
			memcpy(&m_dwId,"TENC",sizeof(m_dwId));
		}
		else
		{
			return 0;
		}

		m_data = (unsigned char *)malloc(size);
		if(!m_data)
		{
			return 0;	//メモリを確保できなかった
		}
		m_dwSize = size;
		m_wFlags = 0;	//v2.2
		memcpy(m_data,&pData[6],size);
		return (size + 6);
	};
	DWORD GetId(){return m_dwId;};
	void SetId(DWORD id){m_dwId = id;};
	DWORD GetSize(){return m_dwSize;};
	void SetSize(DWORD size){m_dwSize = size;};
	WORD GetFlags(){return m_wFlags;};
	void SetFlags(WORD flags){m_wFlags = flags;};
	unsigned char *GetData(){return m_data;};
	void SetData(unsigned char *data,DWORD size)
	{
		if(m_data)
		{
			free(m_data);
		}
		m_data = (unsigned char *)malloc(size);
		if(!m_data)
		{
			return;	//メモリを確保できなかった
		}
		memcpy(m_data,data,size);
		m_dwSize = size;
	};

private:
	unsigned char	*m_data;
	DWORD	m_dwId;
	DWORD	m_dwSize;
	WORD	m_wFlags;
};

#pragma warning(disable:4786)
#include <map>
using namespace std;

class CId3tagv2  
{
public:
#pragma pack(1)
	typedef struct _ID3HEAD
	{
		char id3des[3];
		char ver[2];
		unsigned char flag;
		unsigned char size[4];
	}ID3HEAD;
#pragma pack()
	CId3tagv2(BOOL bScmpxGenre = FALSE);
	virtual ~CId3tagv2();
	BOOL IsEnable(){return m_bEnable;};
	WORD GetVer(){return m_wVer;};
	BOOL IsSafeVer(){return (m_wVer<=0x0300)?TRUE:FALSE;};
	void SetUnicodeEncode(BOOL bEnable){m_bUnicodeEncode = bEnable;};
	void SetUnSynchronization(BOOL bEnable){m_bUnSynchronization = bEnable;};
	
	string GetTitle();	//TIT2
	string GetTrackNo();
	string GetArtist();
	string GetAlbum();
	string GetYear();
	string GetGenre();
	string GetComment();
	string GetComposer();
	string GetOrigArtist();
	string GetCopyright();
	string GetUrl();
	string GetEncoder();
	string GetEncodest();

	DWORD Load(FILE*);	// にとよんにより改変

	DWORD GetTotalFrameSize();

private:
	DWORD DecodeUnSynchronization(unsigned char *data,DWORD dwSize);
	DWORD EncodeUnSynchronization(unsigned char *srcData,DWORD dwSize,unsigned char *dstData);
	DWORD ExtractV2Size(const unsigned char size[4]);
	void MakeV2Size(DWORD dwSize,unsigned char size[4]);
	string GetId3String(const char szId[]);
	void SetId3String(const char szId[],const char *szString,const char *szDescription = NULL);

	void Release();
	BOOL m_bEnable;			//ID3v2が無い場合はFALSE
	BOOL m_bUnicodeEncode;	//文字コードエンコードにUnicodeを使用する
	BOOL m_bUnSynchronization;//非同期化する
	ID3HEAD m_head;
	WORD m_wVer;
	map<DWORD,CId3Frame> m_frames;
	string m_strDefaultEnc;	//TENCのデフォルト値
	BOOL m_bScmpxGenre;		//SCMPX拡張ジャンルを使用する
};

#endif // !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)
