
// uDebug.h
// �f�o�b�O�p
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef	_DEBUG__H_
#define	_DEBUG__H_

#include <windows.h>


/******************************************************************************/
//		�v���g�^�C�v�錾
/******************************************************************************/

void MyOutputDebugString(PCTSTR pszFormat, ...);


/******************************************************************************/
// �f�o�b�O�}�N��
//============================================================================//
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  NO_DEBUG
#define  DBGOUT(l, a)		DebugLog(l, a)
#define  DBGOUT0(l, a)		DebugLog(l, a)
#define  DBGOUT1(l, a, b)	DebugLog(l, a, b)
#define  DBGOUT2(l, a, b, c)	DebugLog(l, a, b, c)
#define  DBGOUT3(l, a, b, c, d)	DebugLog(l, a, b, c, d)
#else
#define  DBGOUT(l, a)		
#define  DBGOUT0(l, a)		
#define  DBGOUT1(l, a, b)	
#define  DBGOUT2(l, a, b, c)	
#define  DBGOUT3(l, a, b, c, d)	
#endif


/******************************************************************************/
// TRACE�}�N��
//============================================================================//
// �T�v�F�f�o�b�O�̃��b�Z�[�W�o��
// �⑫�F�Ȃ��B
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debug�̂Ƃ�
#define TRACE(x)		OutputDebugString(x)
#define TRACE0(x)		OutputDebugString(x)
#define TRACE1(x, a)		MyOutputDebugString(x, a)
#define TRACE2(x, a, b) 	MyOutputDebugString(x, a, b)
#define TRACE3(x, a, b, c)	MyOutputDebugString(x, a, b, c)
#define TRACE4(x, a, b, c, d)	MyOutputDebugString(x, a, b, c, d)
#else
// Release�̂Ƃ�
#define TRACE(x)
#define TRACE0(x)
#define TRACE1(x, a)
#define TRACE2(x, a, b)
#define TRACE3(x, a, b, c)
#define TRACE4(x, a, b, c, d)
#endif


/******************************************************************************/
// ASSERT�}�N��
//============================================================================//
// �T�v�F�ϐ��`�F�b�N
// �⑫�F�Ȃ��B
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debug�̂Ƃ�
#define ASSERT(x) \
    if (!(x)) { \
        MyOutputDebugString("Assertion failed! in %s (%d)\n", \
            __FILE__, __LINE__); \
        DebugBreak(); \
    }
#define VERIFY(x)	ASSERT(x)

#else
// Release�̂Ƃ�
#define ASSERT(x)
#define VERIFY(x)	x
#endif



#endif
