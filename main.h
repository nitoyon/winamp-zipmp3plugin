
// zipmp3.h
//============================================================================//
// �X�V�F02/12/16(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __ZIPMP3_H__
#define  __ZIPMP3_H__

#include "include.h"
#include "GEN.h"


/******************************************************************************/
// 		��`
/******************************************************************************/

#define  APP_NAME	"SimpleVis"
#define  DLLEXPORT	__declspec( dllexport )


/******************************************************************************/
//		�v���g�^�C�v�錾
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