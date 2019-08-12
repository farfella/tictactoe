/*---------------------------------------------------------------\
| MODE_RANDOM.CPP                                                |
| Last Revised: 2003.2.10                                        |
\---------------------------------------------------------------*/

#include "mode_random.h"
#include "com_player.h"

/*------------------------------------------------------
 FUNCTION: InitializeRandom
------------------------------------------------------*/
void InitializeRandom
	(
	void
	)
{
   usUserField = usComputerField = 0;
   ulMoveCount = 0;
   
   srand(GetTickCount());
}

/*------------------------------------------------------
 FUNCTION: RandomMove
------------------------------------------------------*/
unsigned long RandomMove
	(
	unsigned long			usUserMove
	)
{
	unsigned short			usCurrentField;
	register int			i,rnd;
	register unsigned		ret=0;

	/* user goes first */
	if (usUserMove != ~0)
	{
		usUserField |= (1 << usUserMove);
		ulMoveCount++;
	}

	/* user has won the game */
	if (IsWinner(usUserField)) return USER_WIN;

	if (ulMaxMoves != ulMoveCount)
		rnd =  rand() % (ulMaxMoves-ulMoveCount);
	else 
		return USER_TIE;	/* computer tied w/ user */

	usCurrentField = usUserField|usComputerField;

	for (i = 0; i <= rnd; i++)
	{
		if (usCurrentField&(1<<i)) ret++;
		ret++;
	}
	ret--;

	while (usCurrentField&(1<<(ret))) ret++;

	usComputerField |= (1 << (ret));

	ulMoveCount++;
	return ret;
}