/*@@ Wedit generated application. Written Sun Feb 02 15:00:40 2003
 @@header: c:\downloads\blockz\blockzres.h
 @@resources: c:\downloads\blockz\blockz.rc
 Do not edit outside the indicated areas */
/* Pravin Paratey pravin[at]iitb[dot]ac[dot]in*/
/*<---------------------------------------------------------------------->*/
/*<---------------------------------------------------------------------->*/
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <commctrl.h>
//#include <string.h>
#include "blockzres.h"

// Linked list
struct dalist
{
	int x;
	int y;
	struct dalist *next;
}dalist;


struct dalist *mylist;

/*<---------------------------------------------------------------------->*/
HINSTANCE hInst;		// Instance handle
HWND hwndMain;		//Main window handle

LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
/* --- The following code comes from c:\lcc\lib\wizard\statbar.tpl. */

// Global Variables for the status bar control.

HWND  hWndStatusbar;
HBRUSH hbrArray[8]; // Array for blocks
HPEN hpenArray[8];
INT blockArray[16][16];

/*------------------------------------------------------------------------
 Procedure:     UpdateStatusBar ID:1
 Purpose:       Updates the statusbar control with the appropiate
                text
 Input:         lpszStatusString: Charactar string that will be shown
                partNumber: index of the status bar part number.
                displayFlags: Decoration flags
 Output:        none
 Errors:        none

------------------------------------------------------------------------*/
void UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber, WORD displayFlags)
{
    SendMessage(hWndStatusbar,
                SB_SETTEXT,
                partNumber | displayFlags,
                (LPARAM)lpszStatusString);
}


/*------------------------------------------------------------------------
 Procedure:     MsgMenuSelect ID:1
 Purpose:       Shows in the status bar a descriptive explaation of
                the purpose of each menu item.The message
                WM_MENUSELECT is sent when the user starts browsing
                the menu for each menu item where the mouse passes.
 Input:         Standard windows.
 Output:        The string from the resources string table is shown
 Errors:        If the string is not found nothing will be shown.
------------------------------------------------------------------------*/
LRESULT MsgMenuSelect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    static char szBuffer[256];
    UINT   nStringID = 0;
    UINT   fuFlags = GET_WM_MENUSELECT_FLAGS(wparam, lparam) & 0xffff;
    UINT   uCmd    = GET_WM_MENUSELECT_CMD(wparam, lparam);
    HMENU  hMenu   = GET_WM_MENUSELECT_HMENU(wparam, lparam);

    szBuffer[0] = 0;                            // First reset the buffer
    if (fuFlags == 0xffff && hMenu == NULL)     // Menu has been closed
        nStringID = 0;

    else if (fuFlags & MFT_SEPARATOR)           // Ignore separators
        nStringID = 0;

    else if (fuFlags & MF_POPUP)                // Popup menu
    {
        if (fuFlags & MF_SYSMENU)               // System menu
            nStringID = IDS_SYSMENU;
        else
            // Get string ID for popup menu from idPopup array.
            nStringID = 0;
    }  // for MF_POPUP
    else                                        // Must be a command item
        nStringID = uCmd;                       // String ID == Command ID

    // Load the string if we have an ID
    if (0 != nStringID)
        LoadString(hInst, nStringID, szBuffer, sizeof(szBuffer));
    // Finally... send the string to the status bar
    UpdateStatusBar(szBuffer, 0, 0);
    return 0;
}


/*------------------------------------------------------------------------
 Procedure:     InitializeStatusBar ID:1
 Purpose:       Initialize the status bar
 Input:         hwndParent: the parent window
                nrOfParts: The status bar can contain more than one
                part. What is difficult, is to figure out how this
                should be drawn. So, for the time being only one is
                being used...
 Output:        The status bar is created
 Errors:
------------------------------------------------------------------------*/
void InitializeStatusBar(HWND hwndParent,int nrOfParts)
{
    const int cSpaceInBetween = 8;
    int   ptArray[40];   // Array defining the number of parts/sections
    RECT  rect;
    HDC   hDC;

   /* * Fill in the ptArray...  */

    hDC = GetDC(hwndParent);
    GetClientRect(hwndParent, &rect);

    ptArray[nrOfParts-1] = rect.right;
    //---TODO--- Add code to calculate the size of each part of the status
    // bar here.

    ReleaseDC(hwndParent, hDC);
    SendMessage(hWndStatusbar,
                SB_SETPARTS,
                nrOfParts,
                (LPARAM)(LPINT)ptArray);

    UpdateStatusBar("Ready", 0, 0);
    //---TODO--- Add code to update all fields of the status bar here.
    // As an example, look at the calls commented out below.

//    UpdateStatusBar("Cursor Pos:", 1, SBT_POPOUT);
//    UpdateStatusBar("Time:", 3, SBT_POPOUT);
}


/*------------------------------------------------------------------------
 Procedure:     CreateSBar ID:1
 Purpose:       Calls CreateStatusWindow to create the status bar
 Input:         hwndParent: the parent window
                initial text: the initial contents of the status bar
 Output:
 Errors:
------------------------------------------------------------------------*/
static BOOL CreateSBar(HWND hwndParent,char *initialText,int nrOfParts)
{
    hWndStatusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_BORDER|SBARS_SIZEGRIP,
                                       initialText,
                                       hwndParent,
                                       IDM_STATUSBAR);
    if(hWndStatusbar)
    {
        InitializeStatusBar(hwndParent,nrOfParts);
        return TRUE;
    }

    return FALSE;
}

/*<---------------------------------------------------------------------->*/
/*@@0->@@*/
static BOOL InitApplication(void)
{
	WNDCLASS wc;

	memset(&wc,0,sizeof(WNDCLASS));
	wc.style = CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(0,0,0));//(COLOR_WINDOW);
	wc.lpszClassName = "blockzWndClass";
	wc.lpszMenuName = MAKEINTRESOURCE(IDMAINMENU);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	if (!RegisterClass(&wc))
		return 0;
/*@@0<-@@*/
	// ---TODO--- Call module specific initialization routines here
	hbrArray[0] = CreateSolidBrush(RGB(255,0,0));
	hbrArray[1] = CreateSolidBrush(RGB(255,255,0));
	hbrArray[2] = CreateSolidBrush(RGB(0,255,0));
	hbrArray[3] = CreateSolidBrush(RGB(0,255,255));
	hbrArray[4] = CreateSolidBrush(RGB(0,0,255));
	hbrArray[5] = CreateSolidBrush(RGB(255,0,255));
	hbrArray[6] = CreateSolidBrush(RGB(255,125,0));
	hbrArray[7] = CreateSolidBrush(RGB(255,0, 125));
	hpenArray[0] = CreatePen(PS_SOLID,2,RGB(255,0,0));
	hpenArray[1] = CreatePen(PS_SOLID,2,RGB(255,255,0));
	hpenArray[2] = CreatePen(PS_SOLID,2,RGB(0,255,0));
	hpenArray[3] = CreatePen(PS_SOLID,2,RGB(0,255,255));
	hpenArray[4] = CreatePen(PS_SOLID,2,RGB(0,0,255));
	hpenArray[5] = CreatePen(PS_SOLID,2,RGB(255,0,255));
	hpenArray[6] = CreatePen(PS_SOLID,2,RGB(255,125,0));
	hpenArray[7] = CreatePen(PS_SOLID,2,RGB(255,0,125));
	hpenArray[0] = CreatePen(PS_NULL,2,RGB(255,0,0));
	for (int i=0;i<16;i++)
		for(int j=0;j<16;j++)
			blockArray[i][j] = -1;
	srand(time(NULL));
	// Top row
	for (int i=2;i<14;i++)
	{
		for(int j=0;j<2;j++)
		{
			blockArray[i][j] = (int)(7.0*rand())/RAND_MAX;
		}
	}
	// Left Row
	for (int i=0;i<2;i++)
	{
		for(int j=2;j<14;j++)
		{
			blockArray[i][j] = (int)(7.0*rand())/RAND_MAX;
		}
	}
	// Bottom Row
	for (int i=2;i<14;i++)
	{
		for(int j=14;j<16;j++)
		{
			blockArray[i][j] = (int)(7.0*rand())/RAND_MAX;
		}
	}
	// Right Row
	for (int i=14;i<16;i++)
	{
		for(int j=2;j<14;j++)
		{
			blockArray[i][j] = (int)(7.0*rand())/RAND_MAX;
		}
	}
	// Random Starting Blocks
	blockArray[6][7] = (int)(7.0*rand())/RAND_MAX;
	blockArray[6][8] = (int)(7.0*rand())/RAND_MAX;
	blockArray[9][8] = (int)(7.0*rand())/RAND_MAX;
	blockArray[9][7] = (int)(7.0*rand())/RAND_MAX;
	blockArray[7][6] = (int)(7.0*rand())/RAND_MAX;
	blockArray[8][6] = (int)(7.0*rand())/RAND_MAX;
	blockArray[8][9] = (int)(7.0*rand())/RAND_MAX;
	blockArray[7][9] = (int)(7.0*rand())/RAND_MAX;
	return 1;
}

/*<---------------------------------------------------------------------->*/
/*@@1->@@*/
HWND CreateblockzWndClassWnd(void)
{
	return CreateWindow("blockzWndClass","blockz",
		WS_MINIMIZEBOX|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZEBOX|WS_CAPTION|WS_BORDER|WS_SYSMENU,
		CW_USEDEFAULT,0,CW_USEDEFAULT,0,
		NULL,
		NULL,
		hInst,
		NULL);
}
/*@@1<-@@*/
/*<---------------------------------------------------------------------->*/
/* --- The following code comes from c:\lcc\lib\wizard\aboutdlg.tpl. */
BOOL _stdcall AboutDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_CLOSE:
			EndDialog(hwnd,0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd,1);
					return 1;
			}
			break;
	}
	return 0;
}

/*<---------------------------------------------------------------------->*/
/* --- The following code comes from c:\lcc\lib\wizard\defOnCmd.tpl. */
/* --- The following code comes from c:\lcc\lib\wizard\aboutcmd.tpl. */
void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id) {
		// ---TODO--- Add new menu commands here
		/*@@NEWCOMMANDS@@*/
		case IDM_ABOUT:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),
				hwndMain,AboutDlg);
			break;

		case IDM_EXIT:
		PostMessage(hwnd,WM_CLOSE,0,0);
		break;
	}
}

/*------------------------------------------------------------------------
 Procedure:     ManipulateBlocks
 Purpose:       Checks if any of the blocks match and if they do,
 				removes them
 Input:         (x,y) current block
 Output:		none, only visual
 Errors:
------------------------------------------------------------------------*/
void ManipulateBlocks(int x, int y)
{

}

/*<---------------------------------------------------------------------->*/
/*@@2->@@*/
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	int i,j;
	int xpos, ypos;
	HPEN hpen, oldpen, hBlockPen;
	PAINTSTRUCT paint;
	HBRUSH brThis, brOld;
	int done;
	char strTmp[10];

#define BLOCK_SIZE 32 // Size of each block
	switch (msg) {
/*@@3->@@*/
	case WM_LBUTTONUP:
		done = 0;
		xpos = LOWORD(lParam) / BLOCK_SIZE;
		ypos = HIWORD(lParam) / BLOCK_SIZE;
		//If Clicked the top row
		if(xpos > 1 && xpos < 14 && ypos == 1)
		{
			hdc = GetDC(hwnd);

			// Move down if we can
			for(j=ypos;j<12;j++)
			{
				// I am empty and my next is full
				if(blockArray[xpos][j] == -1 && blockArray[xpos][j+1] != -1)
				{
					done = 1;
					blockArray[xpos][j] = blockArray[xpos][ypos];
					// We cannot move anymore draw the block here
					brOld = SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][j]]);
					oldpen = SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,j*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(j+1)*BLOCK_SIZE);
					// Move the old block down
					blockArray[xpos][ypos] = blockArray[xpos][ypos-1];
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][ypos-1]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Generate a new block
					blockArray[xpos][ypos-1] = (int)(7.0*rand())/RAND_MAX;
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][ypos-1]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,(ypos-1)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos)*BLOCK_SIZE);

					SelectObject(hdc, brOld);
					SelectObject(hdc, oldpen);
					break;
				}
				// I am full and my next one is full
				else if(blockArray[xpos][j] != -1 && blockArray[xpos][j+1] != -1)
					break;
			}
			if(!done)
				MessageBeep(0xFFFFFFFF);
			ReleaseDC(hwnd,hdc);
		}
		//If bottom row
		else if(xpos > 1 && xpos < 14 && ypos == 14)
		{
			hdc = GetDC(hwnd);

			// Move up if we can
			for(j=14;j>3;j--)
			{
				// I am empty and my upper is full
				if(blockArray[xpos][j] == -1 && blockArray[xpos][j-1] != -1)
				{
					done = 1;
					blockArray[xpos][j] = blockArray[xpos][ypos];
					// We cannot move anymore draw the block here
					brOld = SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][j]]);
					oldpen = SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,j*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(j+1)*BLOCK_SIZE);
					// Move the old block up
					blockArray[xpos][ypos] = blockArray[xpos][ypos+1];
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][ypos+1]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Generate a new block
					blockArray[xpos][ypos+1] = (int)(7.0*rand())/RAND_MAX;
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos][ypos+1]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, xpos*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos+2)*BLOCK_SIZE);
					SelectObject(hdc, brOld);
					SelectObject(hdc, oldpen);
					break;
				}
				// I am full and my upper one is full
				else if(blockArray[xpos][j] != -1 && blockArray[xpos][j-1] != -1)
					break;
			}
			if(!done)
				MessageBeep(0xFFFFFFFF);
			ReleaseDC(hwnd,hdc);
		}
		//If left row
		else if(ypos > 1 && ypos < 14 && xpos == 1)
		{
			hdc = GetDC(hwnd);

			// Move right if we can
			for(i=2;i<12;i++)
			{
				// I am empty and my right is full
				if(blockArray[i][ypos] == -1 && blockArray[i+1][ypos] != -1)
				{
					done = 1;
					blockArray[i][ypos] = blockArray[xpos][ypos];
					// We cannot move anymore draw the block here
					brOld = SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[i][ypos]]);
					oldpen = SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, i*BLOCK_SIZE,ypos*BLOCK_SIZE,(i+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Move the old block right
					blockArray[xpos][ypos] = blockArray[xpos-1][ypos];
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos-1][ypos]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, (xpos)*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Generate a new block
					blockArray[xpos-1][ypos] = (int)(7.0*rand())/RAND_MAX;
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos-1][ypos]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, (xpos-1)*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					SelectObject(hdc, brOld);
					SelectObject(hdc, oldpen);
					break;
				}
				// I am full and my right is full
				else if(blockArray[i][ypos] != -1 && blockArray[i+1][ypos] != -1)
					break;
			}
			if(!done)
				MessageBeep(0xFFFFFFFF);
			ReleaseDC(hwnd,hdc);
		}
		//If right row
		else if(ypos > 1 && ypos < 14 && xpos == 14)
		{
			hdc = GetDC(hwnd);

			// Move left if we can
			for(i=14;i>3;i--)
			{
				// I am empty and my left is full
				if(blockArray[i][ypos] == -1 && blockArray[i-1][ypos] != -1)
				{
					done = 1;
					blockArray[i][ypos] = blockArray[xpos][ypos];
					// We cannot move anymore draw the block here
					brOld = SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[i][ypos]]);
					oldpen = SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, i*BLOCK_SIZE,ypos*BLOCK_SIZE,(i+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Move the old block left
					blockArray[xpos][ypos] = blockArray[xpos+1][ypos];
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos+1][ypos]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, (xpos)*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos+1)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					// Generate a new block
					blockArray[xpos+1][ypos] = (int)(7.0*rand())/RAND_MAX;
					SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[xpos+1][ypos]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, (xpos+1)*BLOCK_SIZE,(ypos)*BLOCK_SIZE,(xpos+2)*BLOCK_SIZE,(ypos+1)*BLOCK_SIZE);
					SelectObject(hdc, brOld);
					SelectObject(hdc, oldpen);
					break;
				}
				// I am full and my right is full
				else if(blockArray[i][ypos] != -1 && blockArray[i-1][ypos] != -1)
					break;
			}
			if(!done)
				MessageBeep(0xFFFFFFFF);
			ReleaseDC(hwnd,hdc);
		}

		// Game blocks Manipulation and adding points
		// xpos and ypos contain current block
		ManipulateBlocks(xpos, ypos);
		// Check if the Game is over (i.e all edges are
		done = 1;
		for(i=2;i<14;i++)
			done = done && blockArray[i][2] != -1 && blockArray[i][13] != -1
		   		 && blockArray[2][i] != -1 && blockArray[13][i] != -1;
		if (done)
			MessageBox(NULL, "GAME OVER!", "GAME OVER", MB_OK);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paint);
		hpen = CreatePen(PS_SOLID, 1, RGB(30,30,0));
		oldpen = SelectObject(hdc, (HGDIOBJ)hpen);
		// Draw vertical lines
		for(i=1;i<=16;i++)
		{
			MoveToEx(hdc, i*BLOCK_SIZE, 0, NULL);
			LineTo(hdc, i*BLOCK_SIZE, 16*BLOCK_SIZE);
		}
		// Draw horizontal lines
		for(i=1;i<=16;i++)
		{
			MoveToEx(hdc, 0, i*BLOCK_SIZE, NULL);
			LineTo(hdc, 16*BLOCK_SIZE, i*BLOCK_SIZE);
		}
		// Draw Blocks
		for(i=0;i<16;i++)
		{
			for(j=0;j<16;j++)
			{
				if(blockArray[i][j] != -1)
				{
					brOld = SelectObject(hdc, (HGDIOBJ) hbrArray[blockArray[i][j]]);
					SelectObject(hdc, (HGDIOBJ) hpenArray[0]);
					Rectangle(hdc, i*BLOCK_SIZE,j*BLOCK_SIZE,(i+1)*BLOCK_SIZE,(j+1)*BLOCK_SIZE);
					/*char mes[30];
					sprintf(mes, "i=%i;j=%i",i,j);
					TextOut(hdc, 30,30, mes, 30);
					sleep(500);*/
					SelectObject(hdc, brOld);
					SelectObject(hdc, hpen);
				}
			}
		}
		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
		EndPaint(hwnd, &paint);
		break;
	case WM_SIZE:
		SendMessage(hWndStatusbar,msg,wParam,lParam);
		InitializeStatusBar(hWndStatusbar,1);
		break;
	case WM_MENUSELECT:
		return MsgMenuSelect(hwnd,msg,wParam,lParam);
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hwnd,wParam,lParam,MainWndProc_OnCommand);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
/*@@3<-@@*/
	return 0;
}
/*@@2<-@@*/

/*<---------------------------------------------------------------------->*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HANDLE hAccelTable;

	hInst = hInstance;
	if (!InitApplication())
		return 0;
	hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDACCEL));
	if ((hwndMain = CreateblockzWndClassWnd()) == (HWND)0)
		return 0;
	CreateSBar(hwndMain,"Ready",1);
	ShowWindow(hwndMain,SW_SHOW);
	while (GetMessage(&msg,NULL,0,0)) {
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
