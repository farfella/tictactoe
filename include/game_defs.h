/*---------------------------------------------------------------\
| GAME_DEFS.H                                                    |
| Last Revised: 2003.2.10                                        |
|----------------------------------------------------------------|
| DESCRIPTION:                                                   |
\---------------------------------------------------------------*/
#ifndef __GAME_DEFS_H__
#define __GAME_DEFS_H__

#if _MSC_VER >= 1020
	#pragma once
#endif /* _MSC_VER >= 1020 */

#include "os.h"

/*------------------------------------------------------
 flags
------------------------------------------------------*/
#define MODE_COMPUTERFIRST		0x00000001uL
#define MODE_USERFIRST			0x00000002uL
#define MODE_3x3				0x00000004uL
#define MODE_4x4				0x00000008uL
#define MODE_OBLITERATE			0x00000010uL
#define MODE_RANDOM				0x00000020uL
#define MODE_STARTUP			MODE_COMPUTERFIRST|MODE_3x3|MODE_OBLITERATE

#define MAX_MOVE_9				9uL
#define MAX_MOVE_16				16uL
#define MAX_MOVES				MAX_MOVE_16

#define USER_WIN				~0
#define USER_TIE				~0-1


/*------------------------------------------------------
 STRUCTURE: INSTANCE_NODE [68 bytes]
 a node of the tree [used in "invincible" mode]
------------------------------------------------------*/
/* 32-bit word aligned */
#ifdef _MSC_VER
	#pragma pack(push, 4)
#endif /* _MSC_VER */

typedef struct tagINSTANCE_NODE
{
	long						lValue;					// min or max -- alternates every depth
	struct tagINSTANCE_NODE	*	ppChildren[MAX_MOVES];	// 16 possible children
} INSTANCE_NODE, * LPINSTANCE_NODE;

#ifdef _MSC_VER
	#pragma pack(pop)
#endif /* _MSC_VER */


#endif /* __GAME_DEFS_H__ */
