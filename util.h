
// util.h
//============================================================================//
// 更新：02/12/28(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __UTIL_H__
#define  __UTIL_H__

#include "include.h"

/******************************************************************************/
// プロトタイプ宣言
/******************************************************************************/

USHORT makeword( BYTE*) ;
ULONG  makelong( BYTE*) ;

string GetWindowString( HWND) ;
RECT GetChildRect(HWND, HWND);

string GetDirName( const string&) ;
string GetFileName( const string&);

BOOL IsNT();
DWORD GetOsMajorVersion();

#endif