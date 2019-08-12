/*---------------------------------------------------------------\
| GLOBALS.H                                                      |
| Last Revised: 2003.2.10                                        |
|----------------------------------------------------------------|
| DESCRIPTION: contains a few global variables as well as proto- |
| types                                                          |
\---------------------------------------------------------------*/

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#if _MSC_VER >= 1020
	#pragma once
#endif /* _MSC_VER >= 1020 */

#include "os.h"
#include "game_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------
 game related global variables
------------------------------------------------------*/

/*---------------------------------------
 function pointer type definitions
---------------------------------------*/
typedef void (* InitializeModule)
	(
	void
	);

typedef unsigned long (* ComputerPicks)
	(
	unsigned long
	);


typedef BOOL (*Winner)
	(
	register unsigned short
	);

typedef BOOL (*Forced)
	(
	register unsigned short,
	register unsigned short,
	unsigned long
	);

typedef HRESULT (*SubTree)
	(
	LPINSTANCE_NODE,
	unsigned long,
	unsigned long *);

typedef HRESULT (*NodeValue)
	(
	LPINSTANCE_NODE,
	unsigned short,
	unsigned short,
	unsigned long,
	unsigned long
	);


/*----------------------------------------
 GAME RELATED
----------------------------------------*/
extern unsigned long			ulCurrentMode;		/* bit mask of current mode */

extern unsigned long			ulMaxMoves;			/* 16 or 9 pieces */
extern unsigned long			ulMoveCount;		/* # of moves made */

extern unsigned short			usUserField;		/* moves user made */
extern unsigned short			usComputerField;	/* moves computer made */

extern LPINSTANCE_NODE			lpRoot;				/* the root [AI] */
extern LPINSTANCE_NODE			lpCurrentNode;		/* the current node [AI] */

/*----------------------------------------
 FIELD contains winner or forced move
----------------------------------------*/
extern Winner						IsWinner;
extern Forced						IsForced;

/*----------------------------------------
 GAME TREE RELATED
----------------------------------------*/
extern NodeValue					ComputeNodeValue;
extern SubTree						GenerateSubTree;

/*----------------------------------------
 4x4 AI RELATED
----------------------------------------*/
extern unsigned long				ul16Begin;
extern unsigned long				ul16End;


/*---------------------------------------
 function pointers
---------------------------------------*/
extern ComputerPicks			cpk;				/* computer picks function */
extern InitializeModule			imod;				/* module initialization function */

/*------------------------------------------------------
 UI-related global variables
------------------------------------------------------*/

/*---------------------------------------
 helper strings [look in res_strings.c]
---------------------------------------*/
extern LPCTSTR					szTips;
extern LPCTSTR					szAbout;

extern HMENU					hMenu;
extern HACCEL					hAccel;
extern HWND						hwndMain;


/*---------------------------------------
 [implemented in anchillary.c]
---------------------------------------*/
void Tips
	(
	HWND
	);

void About
	(
	HWND
	);


/*---------------------------------------
 Dialog procedure [messages are
 dispatched from the thread-queue to
 this function] [look in entry_point.c]
---------------------------------------*/
INT_PTR CALLBACK TicTacToeDialogProc
	(
	HWND,
	UINT,
	WPARAM,
	LPARAM
	);

__forceinline BOOL InitializeDialog
	(
	register HWND
	);

__forceinline BOOL NewGame
	(
	register HWND
	);

__forceinline BOOL DrawMode9
	(
	register HWND
	);

__forceinline BOOL DrawMode16
	(
	register HWND
	);

__forceinline BOOL ComputerStartsFirstClick
	(
	register HWND
	);

__forceinline BOOL PlayRandom
	(
	register HWND
	);

__forceinline BOOL PlayInvincible
	(
	register HWND
	);

__forceinline BOOL UserPlaysMove
	(
	register HWND,
	register WPARAM
	);


/*---------------------------------------
 display [look in entry_point.c]
---------------------------------------*/
__forceinline void EnableField
	(
	register HWND
	);

__forceinline void DisableField
	(
	register HWND
	);

__forceinline void ClearField
	(
	register HWND
	);

__forceinline void Set3x3PlayingField
	(
	register HWND
	);

__forceinline void Set4x4PlayingField
	(
	register HWND
	);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GLOBALS_H__ */