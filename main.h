
// zipmp3.h
//============================================================================//
// 更新：02/12/16(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ZIPMP3_H__
#define  __ZIPMP3_H__

#include "include.h"
#include "GEN.h"


/******************************************************************************/
// 		定義
/******************************************************************************/

#define  APP_NAME	"SimpleVis"
#define  DLLEXPORT	__declspec( dllexport )


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
DLLEXPORT winampGeneralPurposePlugin * winampGetGeneralPurposePlugin() ;
#ifdef __cplusplus
} ;
#endif

void	config() ; // configuration dialog
int	init() ;	   // initialization for module
void	quit() ;   // deinitialization for module


#endif