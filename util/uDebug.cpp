
// uDebug.cpp
// デバッグ処理
//============================================================================//
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "uDebug.h"
#include <tchar.h>
#include <stdio.h>


/******************************************************************************/
// 出力用関数
//============================================================================//
// 概要：メッセージマップを記述する為の関数。
// 補足：なし。
//============================================================================//

void MyOutputDebugString(PCTSTR pszFormat, ...)
{
	va_list    argp;
	TCHAR pszBuf[ 256];
	va_start(argp, pszFormat);
	_vstprintf(pszBuf, pszFormat, argp);
	va_end(argp);

	OutputDebugString(pszBuf);
}
