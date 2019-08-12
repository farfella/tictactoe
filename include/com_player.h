/*---------------------------------------------------------------\
| COM_PLAYER.H                                                   |
| Last Revised: 2003.2.10                                        |
|----------------------------------------------------------------|
| DESCRIPTION:                                                   |
\---------------------------------------------------------------*/

#ifndef __COM_PLAYER_H__
#define __COM_PLAYER_H__

#if _MSC_VER >= 1020
	#pragma once
#endif /* _MSC_VER >= 1020 */

#include "globals.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */




/*------------------------------------------------------
 FUNCTION: Initialize
 DESCRIPTION: General initialization
 doesn't do much... just calls SetMode(MODE_STARTUP)
------------------------------------------------------*/
HRESULT Initialize
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: Uninitialize
 DESCRIPTION: General uninitialization
------------------------------------------------------*/
void Uninitialize
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: SetMode
 DESCRIPTION: set the flags for game [i.e., who starts
 first, number of pieces, random or ai, etc.].
------------------------------------------------------*/
void SetMode
	(
	register unsigned long
	);

/*------------------------------------------------------
 FUNCTION: IsWinner9
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsWinner9
	(
	register unsigned short
	);

/*------------------------------------------------------
 FUNCTION: IsWinner16
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsWinner16
	(
	register unsigned short
	);

/*------------------------------------------------------
 FUNCTION: IsForced9
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsForced9
	(
	register unsigned short,
	register unsigned short,
	unsigned long
	);

/*------------------------------------------------------
 FUNCTION: IsForced16
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsForced16
	(
	register unsigned short,
	register unsigned short,
	unsigned long
	);

/*-----------------------------------------------------------
 GAME TREE related
-----------------------------------------------------------*/

/*------------------------------------------------------
 FUNCTION: CreateInstanceNode
 DESCRIPTION: Create a node for the tree
------------------------------------------------------*/
LPINSTANCE_NODE CreateInstanceNode
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: DeleteTree
 DESCRIPTION: a recursive delete... note that the max
 depth from the root is 16. So, max on the stack will
 be 16*4.
------------------------------------------------------*/
void DeleteTree
	(
	LPINSTANCE_NODE
	);

/*------------------------------------------------------
 FUNCTION: GenerateNodes
 DESCRIPTION: Creates the children of pRoot [only the
 children]
------------------------------------------------------*/
HRESULT GenerateNodes
	(
	void
	);

/*------------------------------------------------------
 FUNCTION: GenerateSubTree9
 DESCRIPTION: for a 3x3 board
------------------------------------------------------*/
HRESULT GenerateSubTree9
	(
	LPINSTANCE_NODE	lpNode,
	unsigned long	ulMove,
	unsigned long	* pulRet
	);

/*------------------------------------------------------
 FUNCTION: GenerateSubTree16
 DESCRIPTION: for a 4x4 board
------------------------------------------------------*/
HRESULT GenerateSubTree16
	(
	LPINSTANCE_NODE	lpNode,
	unsigned long	ulMove,
	unsigned long	* pulRet
	);

/*------------------------------------------------------
 FUNCTION: ComputeNodeValue9
 DESCRIPTION: compute node values for a 3x3 board
------------------------------------------------------*/
HRESULT ComputeNodeValue9
	(
	LPINSTANCE_NODE,
	unsigned short,
	unsigned short,
	unsigned long,
	unsigned long
	);

/*------------------------------------------------------
 FUNCTION: ComputeNodeValue16
 DESCRIPTION: compute node values for a 4x4 board
------------------------------------------------------*/
HRESULT ComputeNodeValue16
	(
	LPINSTANCE_NODE,
	unsigned short,
	unsigned short,
	unsigned long,
	unsigned long
	);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COM_PLAYER_H__ */