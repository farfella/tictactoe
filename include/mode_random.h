/*---------------------------------------------------------------\
| MODE_RANDOM.H                                                  |
| Last Revised: 2003.2.10                                        |
|----------------------------------------------------------------|
| DESCRIPTION: random moves                                      |
\---------------------------------------------------------------*/
#ifndef __MODE_RANDOM_H__
#define __MODE_RANDOM_H__

#if _MSC_VER >= 1020
	#pragma once
#endif /* _MSC_VER >= 1020 */

#include "globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------
 FUNCTION: InitializeRandom
------------------------------------------------------*/
void InitializeRandom
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: RandomMove
------------------------------------------------------*/
unsigned long RandomMove
	(
	unsigned long
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MODE_RANDOM_H__ */