/*---------------------------------------------------------------\
| MODE_OBLITERATE.H                                              |
| Last Revised: 2003.2.10                                        |
|----------------------------------------------------------------|
| DESCRIPTION:                                                   |
\---------------------------------------------------------------*/
#ifndef __MODE_OBLITERATE_H__
#define __MODE_OBLITERATE_H__

#if _MSC_VER >= 1020
	#pragma once
#endif /* _MSC_VER >= 1020 */

#include "globals.h"
#include "com_player.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------
 FUNCTION: InitializeObliterate
------------------------------------------------------*/
void InitializeObliterate
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: ObliterateMove
------------------------------------------------------*/
unsigned long ObliterateMove
	(
	unsigned long
	);

/*------------------------------------------------------
 FUNCTION: ExceptionHandler
 DESCRIPTION: This handler should/will never occur.
 But it is still provided for fail-safe.
------------------------------------------------------*/
void ExceptionHandler
	(
	void
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MODE_OBLITERATE_H__ */