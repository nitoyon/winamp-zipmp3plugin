
// uDebug.h
// デバッグ用
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef	_DEBUG__H_
#define	_DEBUG__H_

#include <windows.h>


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

void MyOutputDebugString(PCTSTR pszFormat, ...);


/******************************************************************************/
// デバッグマクロ
//============================================================================//
// 概要：なし。
// 補足：なし。
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
// TRACEマクロ
//============================================================================//
// 概要：デバッグのメッセージ出力
// 補足：なし。
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debugのとき
#define TRACE(x)		OutputDebugString(x)
#define TRACE0(x)		OutputDebugString(x)
#define TRACE1(x, a)		MyOutputDebugString(x, a)
#define TRACE2(x, a, b) 	MyOutputDebugString(x, a, b)
#define TRACE3(x, a, b, c)	MyOutputDebugString(x, a, b, c)
#define TRACE4(x, a, b, c, d)	MyOutputDebugString(x, a, b, c, d)
#else
// Releaseのとき
#define TRACE(x)
#define TRACE0(x)
#define TRACE1(x, a)
#define TRACE2(x, a, b)
#define TRACE3(x, a, b, c)
#define TRACE4(x, a, b, c, d)
#endif


/******************************************************************************/
// ASSERTマクロ
//============================================================================//
// 概要：変数チェック
// 補足：なし。
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debugのとき
#define ASSERT(x) \
    if (!(x)) { \
        MyOutputDebugString("Assertion failed! in %s (%d)\n", \
            __FILE__, __LINE__); \
        DebugBreak(); \
    }
#define VERIFY(x)	ASSERT(x)

#else
// Releaseのとき
#define ASSERT(x)
#define VERIFY(x)	x
#endif



#endif
