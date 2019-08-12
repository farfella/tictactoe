/*---------------------------------------------------------------\
| ENTRY_POINT.CPP                                                |
| Last Revised: 2003.2.10                                        |
| Authors: Ateeq Sharfuddin, Taylor Helms                        |
\---------------------------------------------------------------*/

#include "globals.h"
#include "com_player.h"
#include "mode_random.h"
#include "mode_obliterate.h"
#include "resource.h"

HMENU						hMenu;
HACCEL						hAccel;
HWND						hwndMain;

ComputerPicks				cpk;
InitializeModule			imod;

unsigned long				ulMaxMoves = 0;
unsigned long				ulMoveCount = 0;
unsigned short				usUserField = 0;
unsigned short				usComputerField = 0;

LPINSTANCE_NODE				lpRoot = 0;

/*---------------------------------------------------------------
 FUNCTION: WinMain
---------------------------------------------------------------*/
int WINAPI WinMain
   (
   HINSTANCE				hInstance,
   HINSTANCE				hPrevInstance,
   LPSTR					lpCmdLine,
   int						nShowCmd
   )
{
   MSG						msg;
   BOOL						fRet;

   hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_KBDACCEL));

   ZeroMemory(&msg, sizeof(msg));

   /* Now, this app is not slow in the least;
    however, because a java app running concurrently
    with this app may/will use more CPU and resources,
    I'm giving my app a boost, or else the java app
    will be running in the same priority as my app.
   */
   SetPriorityClass( GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

   hwndMain = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TICTACTOE),
                                GetDesktopWindow(), (DLGPROC)TicTacToeDialogProc, 0L);

   while ((fRet = GetMessage(&msg, NULL, 0, 0)))
   {
      switch (fRet)
      {
      case ~0:
         MessageBox(GetDesktopWindow(), _T("GetMessage(&msg,0,0,0) returned -1."),
                    _T("Error"), MB_OK|MB_ICONERROR);
         return msg.wParam;
      default:
         if (!TranslateAccelerator(hwndMain, hAccel, &msg))
         {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }
   }

   return msg.wParam;
}

/*---------------------------------------------------------------
 FUNCTION: TicTacToeDialogProc
---------------------------------------------------------------*/
INT_PTR CALLBACK TicTacToeDialogProc
   (
   HWND						hWnd,
   UINT						uMsg,
   WPARAM					wParam,
   LPARAM					lParam
   )
{
	BOOL					fHandled = FALSE;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		fHandled = InitializeDialog(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NEW:
		case IDM_FNEW:
			fHandled = NewGame(hWnd);
			break;
		case IDM_FEXIT:
			DestroyWindow(hWnd);
			fHandled = TRUE;
			break;
		case IDM_M9:
			fHandled = DrawMode9(hWnd);
			break;
		case IDM_M16:
			fHandled = DrawMode16(hWnd);
			break;
		case IDM_MCOMP:
			fHandled = ComputerStartsFirstClick(hWnd);
			break;
		case ID_RAND:
		case IDM_LRAND:
			fHandled = PlayRandom(hWnd);
			break;
		case ID_INVIN:
		case IDM_LINVIN:
			fHandled = PlayInvincible(hWnd);
			break;
		case IDM_HABOUT:
			About(hWnd);
			fHandled = TRUE;
			break;
		case IDM_HTIPS:
			Tips(hWnd);
			fHandled = TRUE;
			break;
		case IDC_CELL0:
		case IDC_CELL1:
		case IDC_CELL2:
		case IDC_CELL3:
		case IDC_CELL4:
		case IDC_CELL5:
		case IDC_CELL6:
		case IDC_CELL7:
		case IDC_CELL8:
		case IDC_CELL9:
		case IDC_CELLA:
		case IDC_CELLB:
		case IDC_CELLC:
		case IDC_CELLD:
		case IDC_CELLE:
		case IDC_CELLF:
			fHandled = UserPlaysMove(hWnd, LOWORD(wParam)-IDC_CELL0);
			break;
		case ID_N0:
		case ID_N1:
		case ID_N2:
		case ID_N3:
		case ID_N4:
		case ID_N5:
		case ID_N6:
		case ID_N7:
		case ID_N8:
			if (((usUserField|usComputerField)&(1<<(LOWORD(wParam)-ID_N0)))!=0)
			{
				break;
			}
			fHandled = UserPlaysMove(hWnd, LOWORD(wParam)-ID_N0);
			break;
		case ID_N9:
		case ID_NA:
		case ID_NB:
		case ID_NC:
		case ID_ND:
		case ID_NE:
		case ID_NF:
			if (((usUserField|usComputerField)&(1<<(LOWORD(wParam)-ID_N0)))!=0)
			{
				break;
			}

			// these are invisible if you're playing in 3x3
			if (ulMaxMoves == MAX_MOVE_16)
			{
				fHandled = UserPlaysMove(hWnd, LOWORD(wParam)-ID_N0);
			}
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		fHandled = TRUE;
		break;
	case WM_DESTROY:
		Uninitialize();
		PostQuitMessage(0);
		fHandled = TRUE;
		break;
	}

	return fHandled;
}

/*---------------------------------------------------------------
 FUNCTION: InitializeDialog
---------------------------------------------------------------*/
__forceinline BOOL InitializeDialog
	(
	register HWND			hWnd
	)
{
	unsigned long ulCVal;

	hMenu = GetMenu(hWnd);
	// we start in invincible mode, 9 pieces, computer starts first
	CheckMenuItem(hMenu, IDM_M16, MF_UNCHECKED);

	Initialize();
	ClearField(hWnd);

	Set3x3PlayingField(hWnd);

	if (ulCurrentMode & MODE_OBLITERATE)
	{
		CheckMenuItem(hMenu, IDM_LRAND, MF_UNCHECKED);

		imod = InitializeObliterate;
		cpk = ObliterateMove;
	}
	else
	{
		CheckMenuItem(hMenu, IDM_LINVIN, MF_UNCHECKED);

		imod = InitializeRandom;
		cpk = RandomMove;
	}

	imod();

	if (ulCurrentMode & MODE_COMPUTERFIRST)
	{
		// computer starts first
		ulCVal = cpk(~0);

		SetDlgItemText(hWnd, IDC_CELL0+ulCVal, _T("O"));
		EnableWindow(GetDlgItem(hWnd, IDC_CELL0+ulCVal), FALSE);
	}

	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: NewGame
---------------------------------------------------------------*/
__forceinline BOOL NewGame
	(
	register HWND			hWnd
	)
{
	unsigned long ulCVal;

	ClearField(hWnd);
	EnableField(hWnd);

	imod();
	if (ulCurrentMode & MODE_COMPUTERFIRST)
	{
		ulCVal = cpk(~0);
		SetDlgItemText(hWnd, IDC_CELL0+ulCVal, _T("O"));
		EnableWindow(GetDlgItem(hWnd, IDC_CELL0+ulCVal), FALSE);
	}
	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: DrawMode9
---------------------------------------------------------------*/
__forceinline BOOL DrawMode9
	(
	register HWND			hWnd
	)
{
	if (ulCurrentMode & MODE_4x4)
	{
		CheckMenuItem(hMenu, IDM_M16, MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_M9, MF_CHECKED);
		SetMode(MODE_3x3);
		Set3x3PlayingField(hWnd);
		PostMessage(hWnd, WM_COMMAND, IDC_NEW, 0);
	}
	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: DrawMode16
---------------------------------------------------------------*/
__forceinline BOOL DrawMode16
	(
	register HWND			hWnd
	)
{
	if (ulCurrentMode & MODE_3x3)
	{
		CheckMenuItem(hMenu, IDM_M16, MF_CHECKED);
		CheckMenuItem(hMenu, IDM_M9, MF_UNCHECKED);
		SetMode(MODE_4x4);
		Set4x4PlayingField(hWnd);
		PostMessage(hWnd, WM_COMMAND, IDC_NEW, 0);
	}
	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: ComputerStartsFirstClick
---------------------------------------------------------------*/
__forceinline BOOL ComputerStartsFirstClick
	(
	register HWND			hWnd
	)
{
	if (ulCurrentMode & MODE_COMPUTERFIRST)
	{
		SetMode(MODE_USERFIRST);
		CheckMenuItem(hMenu, IDM_MCOMP, MF_UNCHECKED);
	}
	else
	{
		SetMode(MODE_COMPUTERFIRST);
		CheckMenuItem(hMenu, IDM_MCOMP, MF_CHECKED);
	}
	SendMessage(hWnd, WM_COMMAND, IDM_FNEW, 0);
	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: PlayRandom
---------------------------------------------------------------*/
__forceinline BOOL PlayRandom
	(
	register HWND			hWnd
	)
{
	unsigned long ulCVal;

	if (!(ulCurrentMode & MODE_RANDOM))
	{
		CheckMenuItem(hMenu, IDM_LRAND, MF_CHECKED);
		CheckMenuItem(hMenu, IDM_LINVIN, MF_UNCHECKED);
		SetMode(MODE_RANDOM);

		ClearField(hWnd);
		EnableField(hWnd);

		imod = InitializeRandom;
		imod();

		cpk = RandomMove;
		if (ulCurrentMode & MODE_COMPUTERFIRST)
		{
			// computer starts first
			ulCVal = cpk(~0);

			SetDlgItemText(hWnd, IDC_CELL0+ulCVal, _T("O"));
			EnableWindow(GetDlgItem(hWnd, IDC_CELL0+ulCVal), FALSE);
		}
	}
	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: PlayInvincible
---------------------------------------------------------------*/
__forceinline BOOL PlayInvincible
	(
	register HWND			hWnd
	)
{
	unsigned long ulCVal;

	if (!(ulCurrentMode & MODE_OBLITERATE))
	{
		CheckMenuItem(hMenu, IDM_LRAND, MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_LINVIN, MF_CHECKED);
		SetMode(MODE_OBLITERATE);

		ClearField(hWnd);
		EnableField(hWnd);

		imod = InitializeObliterate;
		imod();

		cpk = ObliterateMove;
		if (ulCurrentMode & MODE_COMPUTERFIRST)
		{
			/* computer starts first */
			ulCVal = cpk(~0);

			SetDlgItemText(hWnd, IDC_CELL0+ulCVal, _T("O"));
			EnableWindow(GetDlgItem(hWnd, IDC_CELL0+ulCVal), FALSE);
		}
	}

	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: UserPlaysMove
---------------------------------------------------------------*/
__forceinline BOOL UserPlaysMove
	(
	register HWND			hWnd,
	register WPARAM			wParam
	)
{
	unsigned long ulCVal;

	// disable the button
	SetDlgItemText(hWnd, wParam+IDC_CELL0, _T("X"));
	EnableWindow(GetDlgItem(hWnd, wParam+IDC_CELL0), FALSE);

	_ASSERT(cpk);
	ulCVal = cpk(LOWORD(wParam));

	// game won caught by cpk
	// tie caught by cpk
	switch (ulCVal)
	{
	case USER_WIN:
		DisableField(hWnd);
		MessageBox(hWnd, _T("User won!"), _T("TicTacToe"), MB_OK|MB_ICONINFORMATION);
		break;
	case USER_TIE:
		MessageBox(hWnd, _T("Tie!"), _T("TicTacToe"), MB_OK|MB_ICONINFORMATION);
		break;
	default:
		SetDlgItemText(hWnd, IDC_CELL0+ulCVal, _T("O"));
		EnableWindow(GetDlgItem(hWnd, IDC_CELL0+ulCVal), FALSE);

		if (IsWinner(usComputerField))
		{
			DisableField(hWnd);
			MessageBox(hWnd, _T("Computer won.\nSorry, perhaps next time."),
					   _T("TicTacToe"), MB_OK|MB_ICONINFORMATION);
		}
		else if (ulMaxMoves == ulMoveCount)
		{
			MessageBox(hWnd, _T("Tie!"), _T("TicTacToe"), MB_OK|MB_ICONINFORMATION);
		}
	}

	return TRUE;
}

/*---------------------------------------------------------------
 FUNCTION: EnableField
---------------------------------------------------------------*/
__forceinline void EnableField
	(
	register HWND			hWnd
	)
{
   register WORD i = IDC_CELL0;
   for (; i <= IDC_CELLF; i++)
      EnableWindow(GetDlgItem(hWnd, i), TRUE);
}

/*---------------------------------------------------------------
 FUNCTION: DisableField
---------------------------------------------------------------*/
__forceinline void DisableField
	(
	register HWND			hWnd
	)
{
   register WORD i = IDC_CELL0;
   for (; i <= IDC_CELLF; i++)
      EnableWindow(GetDlgItem(hWnd, i), FALSE);
}

/*---------------------------------------------------------------
 FUNCTION: ClearField
---------------------------------------------------------------*/
__forceinline void ClearField
	(
	register HWND			hWnd
	)
{
   register WORD i = IDC_CELL0;
   for (; i <= IDC_CELLF; i++)
      SetDlgItemText(hWnd, i, _T(""));
}

/*---------------------------------------------------------------
 FUNCTION: Set3x3PlayingField
---------------------------------------------------------------*/
__forceinline void Set3x3PlayingField
	(
	register HWND			hWnd
	)
{
   register WORD i = IDC_CELL9;

   SetWindowPos(hWnd, 0, 0, 0, 210, 254, SWP_NOZORDER|SWP_NOMOVE);

   for (; i <= IDC_CELLF; i++)
   {
      ShowWindow(GetDlgItem(hWnd, i), SW_HIDE);
   }

   SetWindowPos(GetDlgItem(hWnd, IDC_NEW), 0, 12, 170, 170, 20, SWP_NOZORDER);
}

/*---------------------------------------------------------------
 FUNCTION: Set4x4PlayingField
---------------------------------------------------------------*/
__forceinline void Set4x4PlayingField
	(
	register HWND			hWnd
	)
{
   register WORD i = IDC_CELL9;

   SetWindowPos(hWnd, 0, 0, 0, 260, 296, SWP_NOZORDER|SWP_NOMOVE);

   for (; i <= IDC_CELLF; i++)
   {
      ShowWindow(GetDlgItem(hWnd, i), SW_SHOW);
   }

   SetWindowPos(GetDlgItem(hWnd, IDC_NEW), 0, 12, 224, 226, 20, SWP_NOZORDER);
}