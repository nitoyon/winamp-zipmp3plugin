
// Id3tagv2.cpp
// ID3v2 �^�O�̎���
//============================================================================//
// �X�V�F03/02/18(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Id3tagv2.h"

static const unsigned long ID3V2_PADDING_SIZE = 0x0800;
static const unsigned char SCMPX_GENRE_NULL = 247;
static const unsigned char WINAMP_GENRE_NULL = 255;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CId3tagv2::CId3tagv2(BOOL bScmpxGenre)
{
	m_bScmpxGenre = bScmpxGenre;
	m_bUnicodeEncode = FALSE;
	m_bUnSynchronization = TRUE;
	Release();
}

CId3tagv2::~CId3tagv2()
{
}

void CId3tagv2::Release()
{
	m_bEnable = FALSE;
	memset(&m_head,0,sizeof(m_head));
	memcpy(m_head.id3des,"ID3",3);

	m_head.ver[0] = 0x03;
	m_head.ver[1] = 0x00;
	m_wVer = 0x0300;
	m_frames.clear();
}

string CId3tagv2::GetId3String(const char szId[])
{
	map<DWORD,CId3Frame>::iterator p;
	DWORD dwId;
	unsigned char *data;
	DWORD i;
	switch(szId[0]){
	case 'T':	//�e�L�X�g���t���[��
		memcpy(&dwId,szId,sizeof(dwId));
		p = m_frames.find(dwId);
		if((p == m_frames.end()) || !p->second.GetSize())
		{
			break;
		}
		data = p->second.GetData();
		if( (p->second.GetSize() >= 4) && (memcmp(data,"\x01\xff\xfe",3) == 0) )
		{
			//UNICODE -> Ansi
			int size = WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[3],(p->second.GetSize()-3)/2,0,0,NULL,NULL);
			char *buf = new char[size];
			if(!buf) break;
			WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[3],(p->second.GetSize()-3)/2,buf,size,NULL,NULL);
			string strRet(buf);
			delete buf;
			return strRet;
		}
		else if(data[0] == 0)
		{
			// �I�[��\0����菜���@2002-09-16
			int len = (p->second.GetSize()-1);
			if(strlen((LPCSTR )&data[1]) < len)
			{
				len = strlen((LPCSTR )&data[1]);
			}
			return string((LPCSTR )&data[1],len);
		}
		break;
	case 'W':	//URL�����N�t���[��x
		memcpy(&dwId,szId,sizeof(dwId));
		p = m_frames.find(dwId);
		if((p == m_frames.end()) || !p->second.GetSize())
		{
			return "";
		}
		data = p->second.GetData();
		if( (p->second.GetSize() >= 4) && (memcmp(data,"\x01\xff\xfe",3) == 0) )
		{
			//��������ǂݔ�΂�(unicode)
			for(i=3; i<p->second.GetSize(); i+=2)
			{
				if((data[i] == '\0') && (data[i+1] == '\0'))
				{
					break;
				}
			}
			if(i >= p->second.GetSize())
			{
				break;//�{�����Ȃ��ꍇ
			}
			i += 2;
			if( (p->second.GetSize() >= (i+2)) && (memcmp(&data[i],"\xff\xfe",2) == 0) )
			{
				i += 2;
				//UNICODE -> Ansi
				int size = WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[i],(p->second.GetSize()-i)/2,0,0,NULL,NULL);
				char *buf = new char[size];
				if(!buf) break;
				WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[i],(p->second.GetSize()-i)/2,buf,size,NULL,NULL);
				string strRet(buf);
				delete buf;
				return strRet;
			}
		}
		else if(data[0] == 0)
		{
			//��������ǂݔ�΂�
			for(i=1; i<p->second.GetSize(); i++)
			{
				if(data[i] == '\0')
				{
					break;
				}
			}
			if(i >= p->second.GetSize())
			{
				break;//�{�����Ȃ��ꍇ
			}
			// �I�[��\0����菜���@2002-09-16
			int len = (p->second.GetSize()-(i+1));
			if(strlen((LPCSTR )&data[i+1]) < len)
			{
				len = strlen((LPCSTR )&data[i+1]);
			}
			return string((LPCSTR )&data[i+1],len);
		}
		break;
	case 'C':
		if(strcmp(szId,"COMM") != 0)
		{
			break;
		}
		memcpy(&dwId,szId,sizeof(dwId));
		p = m_frames.find(dwId);
		if((p == m_frames.end()) || !p->second.GetSize())
		{
			return "";
		}
		data = p->second.GetData();
		if( (p->second.GetSize() >= (4+3/*Language*/)) &&
			(data[0] == 1) &&
			(memcmp(&data[1+3/*Language*/],"\xff\xfe",3) == 0) )
		{
			//��������ǂݔ�΂�
			for(i=3+3/*Language*/; i<p->second.GetSize(); i+=2)
			{
				if((data[i] == '\0') && (data[i+1] == '\0'))
				{
					break;
				}
			}
			if(i >= p->second.GetSize())
			{
				break;//�{�����Ȃ��ꍇ
			}
			i += 2;
			if( (p->second.GetSize() >= (i+2)) && (memcmp(&data[i],"\xff\xfe",2) == 0) )
			{
				i += 2;
				//UNICODE -> Ansi
				int size = WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[i],(p->second.GetSize()-i)/2,0,0,NULL,NULL);
				char *buf = new char[size];
				if(!buf) break;
				WideCharToMultiByte(CP_ACP,0,(const unsigned short *)&data[i],(p->second.GetSize()-i)/2,buf,size,NULL,NULL);
				string strRet(buf);
				delete buf;
				return strRet;
			}
		}
		else if(data[0] == 0)
		{
			//��������ǂݔ�΂�
			for(i=1+3/*Language*/; i<p->second.GetSize(); i++)
			{
				if(data[i] == '\0')
				{
					break;
				}
			}
			if(i >= p->second.GetSize())
			{
				break;//�{�����Ȃ��ꍇ
			}
			// �I�[��\0����菜���@2002-09-16
			int len = p->second.GetSize()-(i+1);
			if(strlen((LPCSTR )&data[i+1]) < len)
			{
				len = strlen((LPCSTR )&data[i+1]);
			}
			return string((LPCSTR )&data[i+1],len);
//			return string((LPCSTR )&data[1],p->second.GetSize()-1);
		}
		break;
	}
	return "";
}

DWORD CId3tagv2::GetTotalFrameSize()
{
	DWORD dwSize = 0;
	map<DWORD,CId3Frame>::iterator p;

	p = m_frames.begin();
	while(p != m_frames.end())
	{
		CId3Frame *pFrame = &p->second;
		dwSize += pFrame->GetSize() + 10;

		p++;
	}
	return dwSize;
}

string CId3tagv2::GetTitle()
{
	//�^�C�g��
	return GetId3String("TIT2");
}

string CId3tagv2::GetTrackNo()
{
	//�g���b�N�ԍ�
	return GetId3String("TRCK");
}

string CId3tagv2::GetArtist()
{
	//�A�[�e�B�X�g
	return GetId3String("TPE1");
}

string CId3tagv2::GetAlbum()
{
	//�A���o��
	return GetId3String("TALB");
}

string CId3tagv2::GetYear()
{
	//����
	return GetId3String("TYER");
}

string CId3tagv2::GetGenre()
{
	//�W������
	string strGenre = GetId3String("TCON");
	unsigned char *data = (unsigned char *)(LPCSTR)strGenre.c_str();
	//�ŏ���()��ǂݔ�΂�����	Fix 2001-05-20
	while(1)
	{
		if(strGenre.size() &&
			!IsDBCSLeadByte(strGenre[0]) &&
			(strGenre[0] == '(') )	//���ɃJ�b�R�����o
		{
			if((strGenre.length() > 1) &&
				!IsDBCSLeadByte(strGenre[1]) &&
				(strGenre[1] == '(') )
			{
				strGenre = strGenre.substr(1);	//�擪��'('���폜
				break;
			}
			int index = strGenre.find(')');	
			if(0 <= index)
			{
				strGenre = strGenre.substr(index+1);	//)'�ȑO���폜
				continue;	//Fix 2001-10-24
			}
		}
		break;
	}
	return strGenre;
}

string CId3tagv2::GetComment()
{
	//�R�����g
	return GetId3String("COMM");
}

string CId3tagv2::GetComposer()
{
	//���
	return GetId3String("TCOM");
}

string CId3tagv2::GetOrigArtist()
{
	//Orig.�A�[�e�B�X�g
	return GetId3String("TOPE");
}

string CId3tagv2::GetCopyright()
{
	//���쌠
	return GetId3String("TCOP");
}

string CId3tagv2::GetUrl()
{
	//URL
	return GetId3String("WXXX");
}

string CId3tagv2::GetEncoder()
{
	//�G���R�[�_�[
	return GetId3String("TSSE");
}

string CId3tagv2::GetEncodest()
{
	//�G���R�[�h�����l�܂��͑g�D
	return GetId3String("TENC");
}

DWORD CId3tagv2::DecodeUnSynchronization(unsigned char *data,DWORD dwSize)
{
	DWORD dwDecodeSize = 0;
	unsigned char *writePtr = data;
	BOOL bHitFF = FALSE;

	for(DWORD i=0; i<dwSize; i++)
	{
		if(data[i] == 0xff)
		{
			bHitFF = TRUE;
		}
		else
		{
			if(bHitFF && (data[i] == 0x00))
			{
				bHitFF = FALSE;
				continue;
			}
			bHitFF = FALSE;
		}
		writePtr[dwDecodeSize] = data[i];
		dwDecodeSize++;
	}
	return dwDecodeSize;
}

DWORD CId3tagv2::EncodeUnSynchronization(unsigned char *srcData,DWORD dwSize,unsigned char *dstData)
{
	DWORD dwDecodeSize = 0;
	unsigned char *writePtr = dstData;
	BOOL bHitFF = FALSE;

	for(DWORD i=0; i<dwSize; i++)
	{
		if(bHitFF && (((srcData[i]&0xe0) == 0xe0) || (srcData[i] == 0x00)) )
		{
			writePtr[dwDecodeSize] = 0x00;
			dwDecodeSize++;
		}
		if(srcData[i] == 0xff)
		{
			bHitFF = TRUE;
		}
		else
		{
			bHitFF = FALSE;
		}
		writePtr[dwDecodeSize] = srcData[i];
		dwDecodeSize++;
	}
	return dwDecodeSize;
}

DWORD CId3tagv2::ExtractV2Size(const unsigned char size[4])
{
	return (((DWORD )size[0]<<21) | ((DWORD )size[1]<<14) | ((DWORD )size[2]<<7) | (DWORD )size[3]);
}

void CId3tagv2::MakeV2Size(DWORD dwSize,unsigned char size[4])
{
	size[3] = dwSize & 0x7f;
	size[2] = (dwSize>>7) & 0x7f;
	size[1] = (dwSize>>14) & 0x7f;
	size[0] = (dwSize>>21) & 0x7f;
}

/******************************************************************************/
// ID3v2 �w�b�_��ǂ�
//============================================================================//
// �X�V�F03/02/18(��)
// �T�v�F�Ȃ��B
// �⑫�Farranged by nitoyon�B
//============================================================================//

DWORD CId3tagv2::Load(FILE* pfile)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;
	Release();

	//�t�@�C�����I�[�v��
	ID3HEAD head;
	memcpy(&head,&m_head,sizeof(m_head));

	//ID3V2�w�b�_��ǂݍ���
	if( fread(&head,sizeof(head),1,pfile) < 1)
	{
		return -1;
	}

	//ID3v2���m�F
	if(memcmp(head.id3des,"ID3",3) != 0)
	{
		return -1;
	}
	
	//�o�[�W����
	WORD ver = (head.ver[0]<<8) | head.ver[1];
	if(ver > 0x0300)
	{
		return -1;
	}
	m_wVer = ver;

	//Id3tag�T�C�Y
	DWORD dwId3Size = ExtractV2Size(head.size);

	//�S�t���[���̓Ǎ�
	unsigned char *buf = (unsigned char *)malloc(dwId3Size);
	if(!buf)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}
	if(fread(buf,sizeof(char),dwId3Size,pfile) < dwId3Size)
	{
		free(buf);
		return -1;
	}

	//�񓯊����̉���
	if(head.flag & 0x80)
	{
		dwId3Size = DecodeUnSynchronization(buf,dwId3Size);
	}

	DWORD dwRemainSize = dwId3Size;
	//�g���w�b�_��ǂݔ�΂�
	if(head.flag & 0x40)
	{
		dwRemainSize -= ExtractV2Size(buf) + 4;
	}
	head.flag &= ~0x40;	//����

	while(dwRemainSize)
	{
		CId3Frame frame;
		DWORD dwReadSize;
		if(m_wVer < 0x0300)
		{
			dwReadSize = frame.LoadFrame2_2(buf+(dwId3Size-dwRemainSize),dwRemainSize);
		}
		else
		{
			dwReadSize = frame.LoadFrame(buf+(dwId3Size-dwRemainSize),dwRemainSize);
		}
		if(!dwReadSize)
			break;
		m_frames.insert(pair<DWORD,CId3Frame>(frame.GetId(),frame));
		dwRemainSize -= dwReadSize;
	}

	free(buf);

	if(m_wVer < 0x0300)	//v2.2��v2.3�֕ϊ����ĕۑ�����
	{
		head.ver[0] = 0x03;
		head.ver[1] = 0x00;
		m_wVer = 0x0300;
	}

	memcpy(&m_head,&head,sizeof(m_head));
	m_bEnable = TRUE;

	return dwWin32errorCode;
}

