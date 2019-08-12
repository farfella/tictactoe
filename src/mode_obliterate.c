/*---------------------------------------------------------------\
| MODE_OBLITERATE.CPP                                            |
| Last Revised: 2003.2.10                                        |
\---------------------------------------------------------------*/

#include "mode_obliterate.h"
#include "resource.h"

LPINSTANCE_NODE			lpCurrentNode=0;

/*------------------------------------------------------
 FUNCTION: InitializeObliterate
------------------------------------------------------*/
void InitializeObliterate
	(
	void
	)
{
	HRESULT					hr = S_OK;

	usUserField = usComputerField = 0;
	ulMoveCount = 0;

	DeleteTree(lpRoot);

	HRCMEM(lpRoot = CreateInstanceNode());
	HRC(GenerateNodes());

	lpCurrentNode = lpRoot;
Cleanup:;

}

/*------------------------------------------------------
 FUNCTION: ObliterateMove
------------------------------------------------------*/
unsigned long ObliterateMove
	(	
	unsigned long			ulUserMove
	)
{
	unsigned long			ulRet=0;

	/* computer goes first.
	 these the best possible moves. */
	if (ulUserMove == ~0)
	{
		switch (ulMaxMoves)
		{
		case MAX_MOVE_9:
			ulRet = 0;
			break;
		case MAX_MOVE_16:
			ulRet = 4;
			break;
		}

		lpCurrentNode = lpCurrentNode->ppChildren[ulRet];
	}
	else
	{
		usUserField |= (1 << ulUserMove);
		ulMoveCount++;

		if (IsWinner(usUserField)) return USER_WIN;

		GenerateSubTree(lpCurrentNode, ulUserMove, &ulRet);

		__try
		{
			lpCurrentNode = lpCurrentNode->ppChildren[ulUserMove]->ppChildren[ulRet];
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionHandler();
			return ulRet;
		}
	}
	

	usComputerField |= 1<<ulRet;

	ulMoveCount++;
	return ulRet;
}

/*------------------------------------------------------
 FUNCTION: ExceptionHandler
 DESCRIPTION: This handler should/will never occur.
 But it is provided for fail-safe.
------------------------------------------------------*/
void ExceptionHandler(void)
{
	TCHAR szMsg[1024];
	wsprintf(szMsg, _T("An unexpected error has occured.\n")
					_T("Give the developer the following hex: %x\n")
					_T("A new game will restart when you click OK"),
					((usUserField<<16)|usComputerField));
	MessageBox(NULL, szMsg,	_T("TicTacToe"), MB_OK|MB_ICONSTOP);

	/* post to main window's thread's message queue */
	PostMessage(hwndMain, WM_COMMAND, IDM_FNEW, 0);
}