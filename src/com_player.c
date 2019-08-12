/*---------------------------------------------------------------\
| COM_PLAYER.CPP                                                 |
| Last Revised: 2003.2.10                                        |
\---------------------------------------------------------------*/

#include "globals.h"
#include "com_player.h"

LPINSTANCE_NODE				lpRoot;
unsigned long				ulCurrentMode;

Winner						IsWinner;
Forced						IsForced;
NodeValue					ComputeNodeValue;
SubTree						GenerateSubTree;

unsigned long				ul16Begin;
unsigned long				ul16End;

/*------------------------------------------------------
 FUNCTION: Initialize
 DESCRIPTION: General initialization
------------------------------------------------------*/
HRESULT Initialize
	(
	void
	)
{
	HRESULT					hr = S_OK;

	SetMode(MODE_STARTUP);

	RETURNHR(hr);
}

/*------------------------------------------------------
 FUNCTION: Uninitialize
 DESCRIPTION: General uninitialization
------------------------------------------------------*/
void Uninitialize
	(
	void
	)
{
	DeleteTree(lpRoot);
}


/*------------------------------------------------------
 FUNCTION: SetMode
 DESCRIPTION: set the flags for game [i.e., who starts
 first, number of pieces, random or ai, etc.].
------------------------------------------------------*/
void SetMode
	(
	register unsigned long	ulMode
	)
{
	switch (ulMode)
	{
	case MODE_COMPUTERFIRST:
		ulCurrentMode |= MODE_COMPUTERFIRST;
		ulCurrentMode &= ~MODE_USERFIRST;
		break;
	case MODE_USERFIRST:
		ulCurrentMode |= MODE_USERFIRST;
		ulCurrentMode &= ~MODE_COMPUTERFIRST;
		break;
	case MODE_3x3:
		ulCurrentMode |= MODE_3x3;
		ulCurrentMode &= ~MODE_4x4;
		ulMaxMoves = MAX_MOVE_9;

		IsWinner = IsWinner9;
		IsForced = IsForced9;
		GenerateSubTree = GenerateSubTree9;
		ComputeNodeValue = ComputeNodeValue9;
		break;
	case MODE_4x4:
		ulCurrentMode |= MODE_4x4;
		ulCurrentMode &= ~MODE_3x3;
		ulMaxMoves = MAX_MOVE_16;

		IsWinner = IsWinner16;
		IsForced = IsForced16;
		GenerateSubTree = GenerateSubTree16;
		ComputeNodeValue = ComputeNodeValue16;
		break;
	case MODE_OBLITERATE:
		ulCurrentMode |= MODE_OBLITERATE;
		ulCurrentMode &= ~MODE_RANDOM;
		break;
	case MODE_RANDOM:
		ulCurrentMode |= MODE_RANDOM;
		ulCurrentMode &= ~MODE_OBLITERATE;
		break;
	case MODE_STARTUP:
		ulCurrentMode = ulMode;
		ulMaxMoves = MAX_MOVE_9;

		IsWinner = IsWinner9;
		IsForced = IsForced9;
		GenerateSubTree = GenerateSubTree9;
		ComputeNodeValue = ComputeNodeValue9;
		break;
#ifdef _MSC_VER
	default:__assume(0);
#endif // _MSC_VER

	}
}

/*------------------------------------------------------
 FUNCTION: IsWinner9
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsWinner9
	(
	register unsigned short	usField
	)
{
	/*

	hex: 8 4 2 1 | 8 4 2 1 | 8 4 2 1 | 8 4 2 1
	bit: f e d c | b a 9 8 | 7 6 5 4 | 3 2 1 0

	the playing field is set this way:

	  0 1 2 9
	  3 4 5 a
	  6 7 8 b
	  c d e f

	so, if 0 1 2 is filled w/ all X or all O,
	0x1 + 0x2 + 0x4 = 0x7... which is a win.

	*/

	/*
	 No biggie, as the compiler will optimize these
	*/

	if ((usField & 0x0007) == 0x0007)   /* 012 */
		return TRUE;
	if ((usField & 0x0111) == 0x0111)   /* 059 */
		return TRUE;
	if ((usField & 0x0049) == 0x0049)   /* 036 */
		return TRUE;
	if ((usField & 0x0092) == 0x0092)   /* 147 */
		return TRUE;
	if ((usField & 0x0124) == 0x0124)   /* 258 */
		return TRUE;
	if ((usField & 0x0038) == 0x0038)   /* 345 */
		return TRUE;
	if ((usField & 0x01c0) == 0x01c0)   /* 678 */
		return TRUE;
	if ((usField & 0x0054) == 0x0054)   /* 246 */
		return TRUE;

	return FALSE;
}

/*------------------------------------------------------
 FUNCTION: IsWinner16
 DESCRIPTION: determines if a field has a winner
------------------------------------------------------*/
BOOL IsWinner16
	(
	register unsigned short	usField
	)
{
	/* 4x4 addendum */
	if ((usField & 0x0206)==0x0206)  /* 129 */
		return TRUE;
	if ((usField & 0x0430)==0x0430)  /* 45a */
		return TRUE;
	if ((usField & 0x0e00)==0x0e00)  /* 9ab */
		return TRUE;
	if ((usField & 0x0822)==0x0822)  /* 15b */
		return TRUE;
	if ((usField & 0x02a0)==0x02a0)  /* 957 */
		return TRUE;
	if ((usField & 0x2090)==0x2090)  /* 47d */
		return TRUE;
	if ((usField & 0x2500)==0x2500)  /* d89 */
		return TRUE;
	if ((usField & 0x4088)==0x4088)  /* 37e */
		return TRUE;
	if ((usField & 0x4120)==0x4120)  /* 58e */
		return TRUE;
	if ((usField & 0x7000)==0x7000)  /* cde */
		return TRUE;
	if ((usField & 0x8110)==0x8110)  /* 48f */
		return TRUE;
	if ((usField & 0x8c00)==0x8c00)  /* abf */
		return TRUE;
	if ((usField & 0x0980)==0x0980)  /* 78b */
		return TRUE;
	if ((usField & 0x10a0)==0x10a0)  /* c75 */
		return TRUE;
	if ((usField & 0x1048)==0x1048)  /* 36c */
		return TRUE;      
	if ((usField & 0xe000)==0xe000)  /* def */
		return TRUE;

	return IsWinner9(usField);
}

/*------------------------------------------------------
 FUNCTION: IsForced9
 DESCRIPTION: determines if the pieces are set for
 a forced move. [3x3 board]
------------------------------------------------------*/
BOOL IsForced9
	(
	register unsigned short usCField,
	register unsigned short usUField,
	unsigned long ulDepth
	)
{
	switch (ulDepth)
	{
	case 1:
		switch (usUField)
		{
		case 0x0001:
		case 0x0002:
		case 0x0004:
		case 0x0008:
		case 0x0020:
		case 0x0040:
		case 0x0080:
		case 0x0100:
			if (usCField == 0x0010)
			{
				return TRUE;
			}
		}
		break;
	case 2:
		switch (usCField)
		{
		case 0x0041:
			switch(usUField)
			{
			case 0x0002:
			case 0x0100:
				return TRUE;
			}
			break;
		case 0x0005:
			switch (usUField)
			{
			case 0x0008:
			case 0x0080:
			case 0x0020:
				return TRUE;
			}
			break;
		case 0x0101:
			switch (usUField)
			{
			case 0x0004:
			case 0x0040:
			case 0x0010:
				return TRUE;
			}
			break;
		}
		break;
	case 3:
		switch (usUField)
		{
		case 0x0021:
			switch (usCField)
			{
			case 0x0090:
				return TRUE;
			}
			break;
		case 0x00a0:
			switch (usCField)
			{
			case 0x0110:
				return TRUE;
			}
			break;
		}
		break;
	case 4:
		switch (usCField)
		{
		case 0x0050:
			switch (usUField)
			{
			case 0x0108:
			case 0x0081:
			case 0x88:
				return TRUE;
			}
			break;
		case 0x0051:
			if (usUField == 0x000a)
				return TRUE;
			break;
		case 0x0015:
			switch (usUField)
			{
			case 0x000a:
			case 0x0082:
			case 0x0022:
				return TRUE;
			}
			break;
		case 0x0105:
			if (usUField == 0x0050)
				return TRUE;
			break;
		case 0x0141:
			if (usUField == 0x0014)
				return TRUE;
			break;
		case 0x0045:
			if (usUField == 0x0108)
				return TRUE;
			break;
		case 0x0103:
			if (usUField == 0x0090)
				return TRUE;
			break;
		case 0x0181:
			if (usUField == 0x0012)
				return TRUE;
			break;
		case 0x0109:
			if (usUField == 0x0030)
				return TRUE;
			break;
		case 0x0121:
			if (usUField == 0x0018)
				return TRUE;
			break;
		case 0x11:
			switch (usUField)
			{
			case 0xa:
			case 0xc:
				return TRUE;
			}
			break;
		case 0x12:
			if (usUField == 0x101)
				return TRUE;
			break;
		case 0x90:
			switch (usUField)
			{
			case 0x44:
			case 0x60:
				return TRUE;
			}
			break;
		case 0x30:
			switch (usUField)
			{
			case 0x84:
			case 0x102:
				return TRUE;
			}
			break;
		case 0x14:
			switch (usUField)
			{
			case 0x22:
				return TRUE;
			}
			break;
		}
		break;
	case 6:
		switch (usCField)
		{
		case 0x0143:
			if (usUField == 0x0094)
				return TRUE;
			break;
		case 0x010d:
			if (usUField == 0x0070)
				return TRUE;
			break;
		}
		break;
	}

	return FALSE;
}

/*------------------------------------------------------
 FUNCTION: IsForced16
 DESCRIPTION: determines if the pieces are set for
 a forced move. [4x4 board]
------------------------------------------------------*/
BOOL IsForced16
	(
	register unsigned short usCField,
	register unsigned short usUField,
	unsigned long			ulDepth
	)
{
	switch (ulDepth)
	{
	case 2:
		switch (usCField)
		{
		case 0x0090:
			switch(usUField)
			{
			case 0x0001:
			case 0x0100:
			case 0x0200:
			case 0x0400:
			case 0x0800:
			case 0x1000:
			case 0x4000:
			case 0x8000:
				return TRUE;
			}
			break;
		case 0x0110:
			switch (usUField)
			{
			case 0x0002:
			case 0x2000:
				return TRUE;
			}
			break;
		}
		break;
	}
	return IsForced9(usCField, usUField, ulDepth);
}


/*------------------------------------------------------
 FUNCTION: CreateInstanceNode
------------------------------------------------------*/
LPINSTANCE_NODE CreateInstanceNode
	(
	void
	)
{
	void * pNewNode = malloc(sizeof(INSTANCE_NODE));

	if (pNewNode)
	{
	  ZeroMemory(pNewNode, sizeof(INSTANCE_NODE));
	}

	return (LPINSTANCE_NODE)pNewNode;
}

/*------------------------------------------------------
 FUNCTION: DeleteTree
------------------------------------------------------*/
void DeleteTree
	(
	LPINSTANCE_NODE				pNode
	)
{
	unsigned long				i;

	if (pNode)
	{
		for (i = 0; i < ulMaxMoves; i++)
		{
			if (pNode->ppChildren[i])
				DeleteTree(pNode->ppChildren[i]);
		}

		FREE(pNode);
	}
}

/*------------------------------------------------------
 FUNCTION: GenerateNodes
 DESCRIPTION: only generate the children of the root
------------------------------------------------------*/
HRESULT GenerateNodes
	(
	void
	)
{
	HRESULT						hr = S_OK;
	unsigned long				i;
	
	for (i = 0; i < MAX_MOVES; i++)
	{
		HRCMEM(lpRoot->ppChildren[i] = CreateInstanceNode());
	}

Cleanup:
	RETURNHR(hr);
}

/*------------------------------------------------------
 FUNCTION: GenerateSubTree9
 DESCRIPTION: generate the game tree
------------------------------------------------------*/
HRESULT GenerateSubTree9
	(
	LPINSTANCE_NODE				lpNode,
	unsigned long				ulMove,
	unsigned long				* pulRet
	)
{
	HRESULT						hr = S_OK;
	unsigned long				i;
	long						lValue = 0;

	long						fWinnerFound = -1;
	long						fForcedFound = -1;
	long						fUserWinFound = -1;

	if (!lpNode) { return E_UNEXPECTED; }

	if (!lpNode->ppChildren[ulMove])
	{
		HRCMEM(lpNode->ppChildren[ulMove] = CreateInstanceNode());
	}

	HRC(ComputeNodeValue(lpNode->ppChildren[ulMove],usUserField, usComputerField, 1, 0));

	*pulRet = USER_TIE;
	for (i = 0; i < MAX_MOVE_9; i++)
	{
		if (lpNode->ppChildren[ulMove]->ppChildren[i])
		{
			/* if you find a forced move, or a way to win, just
			   play that way and don't think twice; traverse
			   the tree though. */
			if (!((usComputerField|usUserField)&(1<<i)) && IsWinner(usComputerField|(1<<i)))
			{
				fWinnerFound = i;
				break;
			}
			if (IsForced(usComputerField|(1<<i), usUserField, ulMoveCount))
			{
				fForcedFound = i;
			}
			else if (IsWinner(usUserField|(1<<i)))
			{
				fUserWinFound = i;
			}

			if (lpNode->ppChildren[ulMove]->ppChildren[i]->lValue > lValue)
			{
				*pulRet = i;
				lValue = lpNode->ppChildren[ulMove]->ppChildren[i]->lValue;
			}
			
			if (*pulRet == USER_TIE)
			{
				*pulRet = i;
			}
		}
	}

	/* override game tree decision when you're close to winning or
	   close to imminent loss */
	if (fWinnerFound > -1) *pulRet = fWinnerFound;
	else if (fForcedFound > -1) *pulRet = fForcedFound;
	else if (fUserWinFound > -1) *pulRet = fUserWinFound;

Cleanup:
	RETURNHR(hr);
}

/*------------------------------------------------------
 FUNCTION: ComputeNodeValue9
------------------------------------------------------*/
HRESULT ComputeNodeValue9
	(
	LPINSTANCE_NODE				lpNode,
	unsigned short				usUser,
	unsigned short				usComputer,
	unsigned long				ulNonce,
	unsigned long				ulDepth
	)
{
	HRESULT						hr = S_OK;
	unsigned long				i;

	if (!lpNode)
	{
		RETURN(S_FALSE);
	}

	/*
	We're not minimax per se, but a home-cooked derivative of it.
	Basically a tie has a value of 0. Add 1 to a node that is a
	winner. Add -1 to a node that is a loser. If a parent has
	two children that can win, the value of the parent becomes
	2. This way, we don't really have to do a min and max everytime.
	Also, if a node contains a forced move, give it more credit (9 - depth)*10.
	So, if you find a forced move in move 1, the value of the node is
	given 80 (or (9-1)*10 ) more points.
	*/
	if (IsWinner(usComputer))
	{
		lpNode->lValue += 1;
		RETURN(S_OK);
	}

	if (IsWinner(usUser))
	{
		lpNode->lValue += -1;
		RETURN(S_OK);
	}

	ulDepth++;

	for (i = 0; i < MAX_MOVE_9; i++)
	{
		if (!((usUser|usComputer) & (1<<i)))
		{
			if (!lpNode->ppChildren[i])
			{
				HRCMEM(lpNode->ppChildren[i] = CreateInstanceNode());

				if (ulNonce == 1)
				{
					HRC(ComputeNodeValue(lpNode->ppChildren[i], usUser, usComputer | (1<<i), 0, ulDepth));
				}
				else
				{
					HRC(ComputeNodeValue(lpNode->ppChildren[i], usUser | (1<<i), usComputer, 1, ulDepth));
				}

				lpNode->lValue += lpNode->ppChildren[i]->lValue;
			}
		}
	}

	/* drive the computer to play in "Forced Move" mode */
	if (IsForced(usComputer, usUser, ulDepth))
	{
		lpNode->lValue += (MAX_MOVE_9-ulDepth)*10;
	}

Cleanup:
	RETURNHR(hr);
}

//------------------------------------------------------
// FUNCTION: GenerateSubTree16
//------------------------------------------------------
HRESULT GenerateSubTree16
	(
	LPINSTANCE_NODE				lpNode,
	unsigned long				ulMove,
	unsigned long				* pulRet
	)
{
	HRESULT						hr = S_OK;
	unsigned long				i;
	long						lValue = 0;

	long						fWinnerFound = -1;
	long						fForcedFound = -1;
	long						fUserWinFound = -1;

	if (!lpNode->ppChildren[ulMove])
	{
		HRCMEM(lpNode->ppChildren[ulMove] = CreateInstanceNode());
	}

	/* Since we're dealing w/ a 4x4 board, only generate a few
	   subtrees. Why? Because generating too many takes too long.
	*/
	if (ulMoveCount < 6)
	{
		switch (ulMove)
		{
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
			ul16Begin=0x0;
			ul16End=0x9;
			break;
		case 0x5:
		case 0x6:
		case 0x7:
			ul16Begin=0x4;
			ul16End=0xc;
			break;
		case 0x8:
		case 0x9:
		case 0xa:
		case 0xb:
			ul16Begin=0x0;
			ul16End=0x9;
			break;
		case 0xc:
		case 0xd:
		case 0xe:
		case 0xf:
			ul16Begin=0x0;
			ul16End=0x9;
			break;
		}
	}
	else
	{
		ul16Begin = 0x0;
		ul16End = 0xf;
	}


	/* call compute 9 times to make sure a few sub-paths exist. */
	for (i = 0; i < MAX_MOVE_9; i++)
	{
		HRC(ComputeNodeValue(lpNode->ppChildren[ulMove], usUserField, usComputerField, 1, 0));
	}
	

	*pulRet = USER_TIE;
	for (i = 0; i < MAX_MOVE_16; i++)
	{
		if (lpNode->ppChildren[ulMove]->ppChildren[i])
		{
			if (lpNode->ppChildren[ulMove]->ppChildren[i]->lValue > lValue)
			{
				*pulRet = i;
				lValue = lpNode->ppChildren[ulMove]->ppChildren[i]->lValue;
			}
			
			if (*pulRet == USER_TIE)
			{
				*pulRet = i;
			}
		}

		/* since we haven't generated all options
		   chances are we may imminently win or imminently
		   lose. Make sure that you make your very best attempt
		   to win. */
		if (!((usComputerField|usUserField)&(1<<i)))
		{
			if (IsWinner(usComputerField|(1<<i)))
			{
				fWinnerFound = i;
			}
			else if (IsWinner(usUserField|(1<<i)))
			{
				fUserWinFound = i;
			}
			if (IsForced(usComputerField|(1<<i), usUserField, ulMoveCount))
			{
				fForcedFound = i;
			}
		}
	}

	if (fWinnerFound > -1) *pulRet = fWinnerFound;
	else if (fUserWinFound > -1) *pulRet = fUserWinFound;
	else if (fForcedFound > -1) *pulRet = fForcedFound;

	if (!lpNode->ppChildren[ulMove]->ppChildren[*pulRet])
	{
		HRCMEM(lpNode->ppChildren[ulMove]->ppChildren[*pulRet] = CreateInstanceNode());
	}

Cleanup:
	RETURNHR(hr);
}


/*------------------------------------------------------
 FUNCTION: ComputeNodeValue16
------------------------------------------------------*/
HRESULT ComputeNodeValue16
	(
	LPINSTANCE_NODE				lpNode,
	unsigned short				usUser,
	unsigned short				usComputer,
	unsigned long				ulNonce,
	unsigned long				ulDepth
	)
{
	HRESULT						hr = S_OK;
	unsigned long				i;

	if (!lpNode)
	{
		RETURN(S_OK);
	}

	/*
	Same as ComputeNodeValue9. However, if an S_FALSE
	is found, we stop building any more of the tree.
	*/
	if (IsWinner(usComputer))
	{
		lpNode->lValue += 1;
		RETURN(S_FALSE);
	}

	if (IsWinner(usUser))
	{
		lpNode->lValue += -1;
		RETURN(S_FALSE);
	}

	if (lpNode->lValue) { RETURN(S_OK); }

	ulDepth++;

	for (i = ul16Begin; i < ul16End; i++)
	{
		if (!((usUser|usComputer) & (1<<i)))
		{
			if (!lpNode->ppChildren[i])
			{
				HRCMEM(lpNode->ppChildren[i] = CreateInstanceNode());

				if (ulNonce == 1)
				{
					HRC(ComputeNodeValue(lpNode->ppChildren[i], usUser,
										 usComputer | (1<<i), 0, ulDepth));

					/* break away like crazy */
					if (hr == S_FALSE) { RETURN(S_FALSE); }
				}
				else
				{
					HRC(ComputeNodeValue(lpNode->ppChildren[i], usUser | (1<<i),
										 usComputer, 1, ulDepth));
				}

				lpNode->lValue += lpNode->ppChildren[i]->lValue;
			}
		}
	}


	if (IsForced(usComputer, usUser, ulDepth))
	{
		lpNode->lValue += (MAX_MOVE_16-ulDepth)*10;
	}

Cleanup:
	RETURNHR(hr);
}