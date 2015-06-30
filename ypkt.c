/* ypkt.c
 *
 * Pravin Paratey (May 16, 2003)
**/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
//#include <string.h>
#include <winsock2.h>
#include "ypktres.h"

#define WM_WSAASYNC	(WM_USER+5)

unsigned char SEP[] = {192,128};
unsigned char ystrUsername[50];
unsigned char ystrPassword[50];
unsigned char ystrSessionID[4];
unsigned char ystrServerName[50];
unsigned char ystrServerPort[50];
unsigned short ystrLenUsername;

unsigned char outbuf[1000];


typedef void (WINAPI*cfunc)();
cfunc GetYahooStrings;
int yahoo_outgoing_init(int sock);
int yahoo_incoming_init(int sock, unsigned char *inbuf);
int yahoo_outgoing_login(int sock, unsigned char *challenge);
int yahoo_incoming_friendslist(unsigned char *inbuf, unsigned short len);

/*<---------------------------------------------------------------------->*/
HINSTANCE hInst;		// Instance handle
HWND hwndMain;		//Main window handle
HWND hListbox1;
HWND hListbox2;
HWND  hWndStatusbar;
WSADATA *WsaData;
HWND hwndToolBar;

LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);







BOOL _stdcall LoginDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_CLOSE:
			EndDialog(hwnd,0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK: // LOGIN
					// Store username and password and initiate login process
					GetDlgItemText(hwnd, IDE_USERNAME, ystrUsername, 49);
					GetDlgItemText(hwnd, IDE_PASSWORD, ystrPassword, 49);
					GetDlgItemText(hwnd, IDE_SERVERNAME, ystrServerName, 49);
					GetDlgItemText(hwnd, IDE_SERVERPORT, ystrServerPort, 49);
					sa_out.sin_addr.s_addr = inet_addr("216.136.233.131");
					sa_out.sin_port = htons(23);

					//lstrcpy(ystrUsername,"zobloid");
					//lstrcpy(ystrPassword,"applewindow246");

    				UpdateStatusBar("Connecting to Yahoo ...",0,0);
					connect(sout, &sa_out, sizeof(struct sockaddr));
					EndDialog(hwnd, 0);
					break;
				case IDCANCEL:
					EndDialog(hwnd,0);
					break;
			}
			break;
		case WM_ACTIVATE:
			SetFocus(GetDlgItem(hwnd,IDE_USERNAME));
			break;
	}
	return 0;
}


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

int GetFileName(char *buffer,int buflen)
{
	char tmpfilter[40];
	int i = 0;
	OPENFILENAME ofn;

	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = buflen;
	ofn.lpstrTitle = "Open";
	ofn.nFilterIndex = 2;
	ofn.lpstrDefExt = "pkt";
	strcpy(buffer,"*.pkt");
	strcpy(tmpfilter,"All files,*.*,yPkt Files,*.pkt");
	while(tmpfilter[i]) {
		if (tmpfilter[i] == ',')
			tmpfilter[i] = 0;
		i++;
	}
	tmpfilter[i++] = 0; tmpfilter[i++] = 0;
	ofn.Flags = 539678;
	ofn.lpstrFilter = tmpfilter;
	return GetOpenFileName(&ofn);

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "ypktWndClass";
	wc.lpszMenuName = MAKEINTRESOURCE(IDMAINMENU);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	if (!RegisterClass(&wc))
		return 0;
/*@@0<-@@*/
	// ---TODO--- Call module specific initialization routines here

	return 1;
}

/*<---------------------------------------------------------------------->*/
/*@@1->@@*/
HWND CreateypktWndClassWnd(void)
{
	return CreateWindow("ypktWndClass","Yahoo! Messenger Packet Builder - by LightBade",
		WS_MINIMIZEBOX|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZEBOX|WS_CAPTION|WS_BORDER|WS_SYSMENU|WS_THICKFRAME,
		CW_USEDEFAULT,0,CW_USEDEFAULT,0,
		NULL,
		NULL,
		hInst,
		NULL);
}
/*@@1<-@@*/
/*<---------------------------------------------------------------------->*/
/* --- The following code comes from C:\lcc\lib\wizard\aboutdlg.tpl. */
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
/* --- The following code comes from C:\lcc\lib\wizard\defOnCmd.tpl. */
/* --- The following code comes from C:\lcc\lib\wizard\aboutcmd.tpl. */
void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{	char buffer[MAX_PATH];

	switch(id) {
		// ---TODO--- Add new menu commands here
		/*@@NEWCOMMANDS@@*/
		case IDM_ABOUT:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),
				hwndMain,AboutDlg);
			break;

		case IDM_OPEN:
		GetFileName(buffer,sizeof(buffer));
		break;

		case IDM_EXIT:
		PostMessage(hwnd,WM_CLOSE,0,0);
		break;
	}
}

/*<---------------------------------------------------------------------->*/
/*@@2->@@*/
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	unsigned char *inbuf;
	static unsigned char header[1000];
	int retval;
	unsigned short pktsize;
	unsigned char c; //tmp var
	static int bGotHeader=0;
	unsigned char errStr[20];
	RECT rc;
	RECT rcToolbar, rcStatusbar;

	switch (msg) {
	case WM_WSAASYNC:
		switch(WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		/*
			// check for an error
			retval = WSAGetLastError();
			if (retval != 0 && retval != WSAEWOULDBLOCK) // Error
			{
				bGotHeader = 0;
				wsprintf(header,"Error %i",retval);
				MessageBox(hwnd, "Socket error in FD_READ", header, MB_OK);
				WSASetLastError(0); // Reset error
				return 0;
			}
		*/
			if(!bGotHeader) // We have not recv'd header do it now
			{
				// Get header and determine the length of the packet
				retval = recv(wParam,header,20,0);
				if(WSAGetLastError() == WSAEWOULDBLOCK)
				{
					WSASetLastError(0);
				}
				else if (retval == SOCKET_ERROR)
				{
					MessageBox(hwnd, "Error while reading header", "ERROR", MB_OK);
					break;
				}
				bGotHeader = !bGotHeader;
			}
			else // We got header now get rest of the data
			{
				// 9 and 10th bytes give size
				// Below two bytes are swapped for endian-ness
				c = header[9];
				header[9] = header[8];
				header[8] = c;
				// Set length
				memcpy(&pktsize, header+8, 2);
				inbuf = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, pktsize+20);
				if (!inbuf)
				{
					MessageBox(hwnd, "Not enough memory", "ERROR", MB_OK);
					break;
				}
				// copy header
				memcpy(inbuf,header,20);

				//recv the rest of the packet
				retval = recv(wParam, inbuf+19,pktsize,0);
				if (retval == SOCKET_ERROR)
				{
					MessageBox(hwnd, "Error while reading rest of packet", "ERROR", MB_OK);
					break;
				}
		//////////////////////////
		// IMPORTANT
		// We are swapping 9th and 10th bytes of header to change the length to
		// conform to endian architecture
		/////////////////////////
				switch(inbuf[11])
				{
				case 'W': // 0x57 Incoming Init Packet (Has challenge string)
					//UpdateStatusBar("CHALLENGE STRING",0,0);
					if(!yahoo_incoming_init(wParam, inbuf))
						MessageBox(NULL, "UNABLE TO LOGIN!", "ERROR", MB_OK);
					break;
				case 'U': // 0x55 Friends list
					UpdateStatusBar("Logged in. Receiving Friends List",0,0);
					if(!yahoo_incoming_friendslist(inbuf, pktsize+20))
						MessageBox(NULL, "Error while receiving friend list", "ERROR", MB_OK);
					break;
				case 0x96: // Init Room login
					UpdateStatusBar("Sent Init packet",0,0);
					// Call join room
					if(!yahoo_outgoing_join_chatroom(wParam, "Hackers' Lounge:1"))
						MessageBox(hwnd, "Error sending join chat packet","ERROR", MB_OK);
					break;
				case 0x98:
					UpdateStatusBar("Entered chatroom",0,0);
					break;
				case 0x0b: // Login successful! ???? I am not sure about this
					UpdateStatusBar("I'm Available",0,0);
//					if(!yahoo_outgoing_init_chatroom(wParam))
//						MessageBox(hwnd, "Error sending init chat packet","ERROR", MB_OK);
					break;
				case 0x06:
					yahoo_incoming_im(inbuf, pktsize+20);
					break;
				case 0x01: // Online Buddies
					yahoo_incoming_onlinebuddies(inbuf, pktsize+20);
					break;
				default:
					wsprintf(outbuf,"%i--%x",retval,inbuf[11]);
					UpdateStatusBar(outbuf,0,0);
//					packet_dump(inbuf,pktsize+19);
				}
				HeapFree(GetProcessHeap(),0,inbuf);
				bGotHeader = !bGotHeader;
			}
			break;
		case FD_WRITE:
			break;
		case FD_CONNECT:
			// Login here
			if(!yahoo_outgoing_init(wParam))
			{
				wsprintf(errStr,"Error %i",WSAGetLastError());
				MessageBox(hwnd, "Unable to login [1]", errStr, MB_OK);
			}
			UpdateStatusBar("SENT PACKET",0,0);
			break;
		case FD_CLOSE:
			break;
		}
		break;
	case WM_SIZE:
		SendMessage(hWndStatusbar,msg,wParam,lParam);
		SendMessage(hwndToolBar,msg,wParam,lParam);
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
/* --- The following code comes from C:\lcc\lib\wizard\tcpini.tpl. */
// WSADATA *InitWS2(void)
// Routine Description:
//
// Calls WSAStartup, makes sure we have a good version of WinSock2
//
//
// Return Value:
//  A pointer to a WSADATA structure - WinSock 2 DLL successfully started up
//  NULL - Error starting up WinSock 2 DLL.
//
WSADATA *InitWS2(void)
{
    int           Error;              // catches return value of WSAStartup
    WORD          VersionRequested;   // passed to WSAStartup
    static WSADATA       WsaData;            // receives data from WSAStartup
    BOOL          ReturnValue = TRUE; // return value flag

    // Start WinSock 2.  If it fails, we don't need to call
    // WSACleanup().
    VersionRequested = MAKEWORD(2, 0);
    Error = WSAStartup(VersionRequested, &WsaData);
    if (Error) {
        MessageBox(hwndMain,
                   "Could not find high enough version of WinSock",
                   "Error", MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        ReturnValue = FALSE;
    } else {

        // Now confirm that the WinSock 2 DLL supports the exact version
        // we want. If not, make sure to call WSACleanup().
        if (LOBYTE(WsaData.wVersion) != 2) {
            MessageBox(hwndMain,
                       "Could not find the correct version of WinSock",
                       "Error",  MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
            WSACleanup();
            ReturnValue = FALSE;
        }
    }
    if (ReturnValue)
        return &WsaData;
    return(NULL);

} // InitWS2()
/*<---------------------------------------------------------------------->*/
#define NUM_TOOLBAR_BUTTONS		3
/* --- The following code comes from C:\lcc\lib\wizard\creatToolbar1.tpl. */
HWND CreateAToolBar(HWND hwndParent)
{
	HWND hwndTB;
	TBADDBITMAP tbab;
	TBBUTTON tbb[NUM_TOOLBAR_BUTTONS];
	int index;

	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create a toolbar that the user can customize and that has a
	// tooltip associated with it.
	hwndTB = CreateWindowEx(0, TOOLBARCLASSNAME, (LPSTR) NULL,
	    WS_CHILD|WS_BORDER,//|TBSTYLE_FLAT,
	    0, 0, 0, 0, hwndParent, (HMENU) ID_TOOLBAR, hInst, NULL);

	// Send the TB_BUTTONSTRUCTSIZE message, which is required for
	// backward compatibility.
	SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE,
	    (WPARAM) sizeof(TBBUTTON), 0);

	// Add the bitmap containing button images to the toolbar.
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID   = IDB_STD_SMALL_COLOR;
	SendMessage(hwndTB, TB_ADDBITMAP, (WPARAM) NUM_TOOLBAR_BUTTONS,(LPARAM) &tbab);

	// clean memory before using it
	memset(tbb,0,sizeof tbb);

	// Add the strings

	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Replace");
	tbb[0].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Delete");
	tbb[1].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"New");
	tbb[2].iString = index;
	// Button "Replace"
	tbb[0].iBitmap = STD_REPLACE;
	tbb[0].idCommand = IDM_EDITREPLACE;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;

	// Button "Delete"
	tbb[1].iBitmap = STD_DELETE;
	tbb[1].idCommand = IDM_EDITDELETE;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;

	// Button "New"
	tbb[2].iBitmap = STD_FILENEW;
	tbb[2].idCommand = IDM_NEW;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;

	SendMessage(hwndTB,TB_ADDBUTTONS,3,(LPARAM)&tbb);
	ShowWindow(hwndTB,SW_SHOW);
	return hwndTB;
}

/*<---------------------------------------------------------------------->*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HANDLE hAccelTable;

	hInst = hInstance;
	if (!InitApplication())
		return 0;
	hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDACCEL));
	if ((hwndMain = CreateypktWndClassWnd()) == (HWND)0)
		return 0;
	CreateSBar(hwndMain,"Ready",1);
	hwndToolBar = CreateAToolBar(hwndMain);
	ShowWindow(hwndMain,SW_SHOW);
	WsaData = InitWS2();
	if (WsaData == NULL)
		return 0;
	while (GetMessage(&msg,NULL,0,0)) {
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	WSACleanup();
	return msg.wParam;
}

/* OUTGOING INIT PACKET
YMSG
VERSION
00 00 00
	length
00
COMMAND = W
00 00 00 00
00 00 00 00
ASCII(1)
SEP
	username
SEP
59 4D 53 47|09|00 00 00|00 0F|00 57|00 00 00 00|  YMSG.......W....
00 00 00 00|31|C0 80|76 69 6E 69 64 75 68 70 6F   ....1..viniduhpo
6F|C0 80|                                         o..
*/
int yahoo_outgoing_init(int sock)
{
	unsigned short len;
	char *pos;
	unsigned char c;
	const unsigned char dastr[] = {'Y','M','S','G',
						9,0,0,0,
						0,0, // Length
						0,'W', // Command
						0,0,0,0,
						0,0,0,0,
						'1',
						192,128};
	// calculate length
	ystrLenUsername = lstrlen(ystrUsername);
	len = 5 + ystrLenUsername;
	pos = outbuf;
	memcpy(pos,dastr,23);
	pos += 23;

	memcpy(pos,ystrUsername, ystrLenUsername);
	pos += ystrLenUsername;

	memcpy(pos,SEP,2);

	// print this value in dastr
	memcpy(outbuf+8, &len, 2);
	c = outbuf[8];
	outbuf[8] = outbuf[9];
	outbuf[9] = c;
//	packet_dump(outbuf,len+20);
	if(send(sock, outbuf, len+20, 0) == -1)
		return 0;
	return 1;
}

/* INCOMING init PACKET
YMSG
00
00 00 00
	length[2 bytes]
COMMAND = W
00 00 00 01
	session-id[4 bytes]
ASCII(1)
SEP
	username[* bytes]
SEP
ASCII(94)
SEP
	challenge-string[24 bytes]
SEP
ASCII(13)
SEP
ASCII(0)
Packet Type: 0x57(Sending Login ID) [InComing]
59 4D 53 47|00 00 00 00|00 34|00 57|00 00 00 01|  YMSG.....4.W....
D0 22 DF C0|31|C0 80|76 69 6E 69 64 75 68 70 6F   ....1..viniduhpo
6F|C0 80|39 34|C0 80|4D 6C 48 4F 69 71 56 59 54   o..94..MlHOiqVYT
36 4C 7A 78 56 5A 52 30 48 6D 59 54 67 2D 2D|C0   6LzxVZR0HmYTg...
80|31 33|C0 80|30|C0 80                           .13..0..
*/

int yahoo_incoming_init(int sock, unsigned char *inbuf)
{
	unsigned short len;
	unsigned char *pStart, *pEnd;
	unsigned char ystrChallenge[25];
//	int i;

	// Get length of string
	len = (short) inbuf+8;
	// Get Session ID
	memcpy(ystrSessionID, inbuf+16, 4);

	// Get Challenge String
	pStart = inbuf+22;
	pEnd = pStart;
	while(1)
	{
		if(*pStart == 0x39 && *(pStart+1) == 0x34 &&
			*(pStart+2) == 0xc0 && *(pStart+3) == 0x80)
			break;
		pStart++;
		if((int)(pStart - pEnd) > 200)
		{
			// Dirty Packet
			MessageBox(NULL, ystrChallenge,"DIRTY PACKET", MB_OK);
			return 0;
		}
	}
	pStart += 4;
	// Got challenge string
	memcpy(ystrChallenge, pStart, 24);
	ystrChallenge[24]=0;
	return yahoo_outgoing_login(sock, ystrChallenge);
}

/* OUTGOING LOGIN PACKET
YMSG
VERSION
00 00 00
	length[2 bytes]
00 COMMAND T (Login command)
00 00 00 0C
	session-id[4 bytes]
ASCII(0)
SEP
	username
SEP
ASCII(6)
SEP
	str1[24 bytes]
SEP
ASCII(96)
SEP
	str2[24 bytes]
SEP
ASCII(2)
SEP
ASCII(1)
SEP
ASCII(1)
SEP
	username
SEP
*/

int yahoo_outgoing_login(int sock, unsigned char *challenge)
{
	int i,j;
	unsigned char c;
	unsigned char * pos;
	unsigned short len;
	unsigned char str1[25], str2[25];
	unsigned char dastr[] = {'Y','M','S','G',
					9,
					0,0,0,
					0,0, // length
					0,'T',
					0,0,0,12,
					0,0,0,0, //Session ID 17B
					'0',
					192,128, //SEP // byte 23End
					//usrname,
					192,128, // byte 24
					'6',
					192,128, //28E
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//str1 24 bytes
					192,128, //53B
					'9','6',
					192,128, //58E
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//str2 24 bytes
					192,128, //83B
					'2',
					192,128,
					'1',
					192,128,
					'1',
					192,128, //92E
					//username,
					//192,128
					};

	memcpy(outbuf,dastr,24);
	pos = outbuf+23;

	memcpy(outbuf+16,ystrSessionID,4);

	ystrLenUsername = lstrlen(ystrUsername);
	memcpy(pos,ystrUsername,ystrLenUsername);
	pos +=ystrLenUsername;

	memcpy(pos, dastr+23, 70);
	pos += 70;

	// Make strings 1 and 2
	GetYahooStrings(ystrUsername, ystrPassword, challenge, &str1, &str2);
	memcpy(outbuf+28+ystrLenUsername, str1, 24);
	memcpy(outbuf+58+ystrLenUsername, str2, 24);

	memcpy(pos,ystrUsername,ystrLenUsername);
	pos += ystrLenUsername;

	memcpy(pos, dastr+23,2); //FINAL SEP

	// Update length bytes 8 and 9
	len = 75 + 2*ystrLenUsername; //93-20
	memcpy(outbuf+8, &len, 2);
	c = outbuf[8];
	outbuf[8] = outbuf[9];
	outbuf[9] = c;
///////////////////////////////////
//	j=0;
//	for (i=0; i <= len+20; i++)
//	{
//		j=i*3;
//		wsprintf(outbuf+400+j,".%2x",*(outbuf+i));
//	}
//	MessageBox(NULL, outbuf+400, "SENDING LOGIN", MB_OK);
///////////////////////////////////

//	packet_dump(outbuf,len+20);
	// PACKET HAS BEEN FORMED AT THIS POINT
	if(send(sock,outbuf,len+20,0) == -1)
		return 0;
	else
		return 1;
}

/* INCOMING FRIENDS LIST
YMSG
00 00 00 00
	length[2 bytes]
00 COMMAND U (Incoming friends list)
00 00 00 05
	3 BYTES
ASCII(87) <----------------- PACKET SUB TYPE
SEP
	Group Name followed by :(0x3a)
	Immediately followed by friends
	Each Friend separated by 0x2C
	Each Group separated by 0x0A
	!!! Last friend doesnot end with 0x2C
0x0A
SEP
ASCII(88) <----------------- PACKET SUB TYPE
	People to be ignored
	Each person separated by 0x2C
SEP
ASCII(89)
SEP
	List of user aliases separated by 0x2C
SEP
ASCII(59)
SEP
ASCII(Y) 09
v=1
&
n=
&
l=
&
p=
&
ig=
&
iz=
&
r=
&
cg=
&
intl=
SEMICOLON 0x20
expires=
SEMICOLON 0x20
path=
SEMICOLON 0x20
domain=
SEP
ASCII(59)
SEP
T
SEP
z=
&
a=
&
sk=
&
d=
SEMICOLON 0x20
expires=
SEMICOLON 0x20
path=
SEMICOLON 0x20
domain=
SEP
ASCII(59)
SEP
ASCII(C) 0x09
mg=
SEP
ASCII(90)
SEP
ASCII(1)
SEP
ASCII(3)
SEP
	username
SEP
ASCII(100)
SEP
ASCII(0)
SEP
ASCII(101)
SEP
SEP
ASCII(102)
SEP
SEP
ASCII (93)
SEP
ASCII(440)
blah blah
	Info about online friends ????

SEP
*/

int yahoo_incoming_friendslist(unsigned char *inbuf, unsigned short len)
{
	unsigned char c;
	unsigned char *pStart, *pEnd;
	HTREEITEM tmpParent;

//	packet_dump(inbuf, len);

	return 1;
}


