/* Sherry - A peer to peer file sharing application
 * Pravin Paratey (April 22, 2003)
 */

/*@@ Wedit generated application. Written Mon Apr 21 16:23:34 2003
 @@header: c:\downloads\sherry\sherryres.h
 @@resources: c:\downloads\sherry\sherry.rc
 Do not edit outside the indicated areas */
/*<---------------------------------------------------------------------->*/
/*<---------------------------------------------------------------------->*/
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string.h>
#include <winsock2.h>
#include "sherryres.h"

#define WM_WSAASYNC (WM_USER+5)
#define UDP_LISTEN_PORT	3000
#define TCP_LISTEN_PORT	4000

/*<---------------------------------------------------------------------->*/
HINSTANCE hInst;		// Instance handle
HWND hwndMain;		//Main window handle

WSADATA *WsaData;
HWND hwndToolBar;
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
/* --- The following code comes from c:\lcc\lib\wizard\statbar.tpl. */

char szDirectory[255]; // Directory which is shared
// Global Variables for the status bar control.

HWND hWndStatusbar;
HWND hListbox;
HWND hMessages;

// udplistener listens for UDP broadcasts
SOCKET udplistener;
SOCKADDR_IN udplistener_sa;

// tcplistener listens for TCP
SOCKET tcplistener;
SOCKADDR_IN tcplistener_sa;

// tcp socket connects to retrieve files and send file listings
SOCKET tcpsock;
SOCKADDR_IN tcpsock_sa;

WNDPROC OldListBoxProc;
LRESULT CALLBACK ListBoxProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);


LRESULT CALLBACK ListBoxProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int index;
	char daStr[300];
	SOCKET sock;
	SOCKADDR_IN sa;
	char szIP[24];
	char szFile[255];
	char inbuf[1000];
	char *pStart, *pEnd;
	HANDLE fd;
	DWORD numby;
	OPENFILENAME opn;
	char fileName[255];


	switch(msg)
	{
		case WM_LBUTTONDBLCLK:
			// Get Index
			index = SendMessage(hwnd,LB_GETCURSEL,0,0);
			if (index == LB_ERR)
				break;
			// Get String
			if(SendMessage(hListbox, LB_GETTEXT,(WPARAM)index,(LPARAM)daStr) == LB_ERR)
				break;

			// Chop IP Part
			pStart = daStr;
			pEnd = pStart+1;
			while(*pEnd != ':')
			{
				pEnd++;
			}
			*pEnd=0;
			strcpy(szIP,pStart);
			// Chop Filename
			pStart = pEnd+1;
			strncpy(szFile,pStart,255);
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"[local] Create TCP socket for transferring files");

			// Create socket
			sock = socket(AF_INET,SOCK_STREAM,0);
			if(sock == INVALID_SOCKET)
			{
				MessageBox(hwndMain,"Unable to create socket [DBLCLK]","ERROR",MB_OK);
				break;
			}
			// Fill in sa struct
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = inet_addr(szIP);
			sa.sin_port = htons(TCP_LISTEN_PORT);

			sprintf(daStr,"%s %s","[local] Connect to remote host",szIP);
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)daStr);

			// Connect to remote host
			if(connect(sock,&sa,sizeof(struct sockaddr)) == SOCKET_ERROR)
			{
				MessageBox(hwndMain,"Connect failed [DBLCLK]","ERROR",MB_OK);
				break;
			}
			sprintf(daStr,"RFIL    %s",szFile);
			//index = strlen(daStr);
			daStr[strlen(szFile)+8] = 0;
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Connected to remote host, sending REQUEST FILE command");
			// Send RFIL command
			if(send(sock,daStr,200,0) == SOCKET_ERROR)
			{
				MessageBox(hwndMain,"Send failed [DBLCLK]","ERROR",MB_OK);
				break;
			}
//			closesocket(sock);

			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Transferring file");
			//MessageBox(hwndMain,"DOODA","DOODA",MB_OK);
			// Recv File
			if(recv(sock,inbuf,1000,0) == SOCKET_ERROR)
			{
				MessageBox(hwndMain,"Recv failed [DBLCLK]","ERROR",MB_OK);
				break;
			}
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"File Transferred!!!");

			ZeroMemory(&opn,sizeof(OPENFILENAME));
			opn.lStructSize = sizeof(OPENFILENAME);
			opn.hwndOwner = hwndMain;
			opn.hInstance = hInst;
			opn.lpstrFile = szFile;
			opn.nFileOffset = 0;
			//opn.lpstrFilter = "All Files;*\0\0";
			opn.lpstrCustomFilter = NULL;
			opn.nMaxFile = 255;
			opn.lpstrFileTitle = fileName;
			opn.nMaxFileTitle = 254;
			opn.lpstrInitialDir = szDirectory;

			GetSaveFileName(&opn);
			//MessageBox(hwndMain,fileName,"DEBUG",MB_OK);

			sprintf(daStr,"%s",fileName);
//			MessageBox(hwndMain,daStr,"DEBUG",MB_OK);
			fd = CreateFile(daStr, GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
			if(fd == INVALID_HANDLE_VALUE)
			{
				MessageBox(hwndMain,"Unable to open local file for writing","ERROR",MB_OK);
				break;
			}
			index = 0;
			pStart = inbuf;
			while(*pStart != 0)
			{
				pStart++;
				index++;
			}
			//WriteFile(fd,inbuf,index > 1000 ? 1000:index, numby, NULL);
//			while(1)
//			{
				WriteFile(fd,inbuf,index, &numby, NULL);
//				if(recv(sock,inbuf,1000,0) == SOCKET_ERROR)
//					break;
//			}
//			MessageBox(hwndMain,inbuf,"DEBUG",MB_OK);
			CloseHandle(fd);
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Closing TCP socket");
			closesocket(sock);

			break;
		default:
			return CallWindowProc(OldListBoxProc,hwnd,msg,wParam,lParam);
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
	wc.lpszClassName = "sherryWndClass";
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
HWND CreatesherryWndClassWnd(void)
{
	return CreateWindow("sherryWndClass","Sherry",
		WS_MINIMIZEBOX|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZEBOX|WS_CAPTION|WS_BORDER|WS_SYSMENU|WS_THICKFRAME,
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
	SOCKET sock;
	SOCKADDR_IN sa;
	int val = 1;
	OPENFILENAME opn;
	char fileName[255];

	switch(id) {
		// ---TODO--- Add new menu commands here
		/*@@NEWCOMMANDS@@*/
		case IDM_UPDATE:
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"[local] Create UDP socket for Broadcast");

			// Send UDP Broadcast
			sock = socket(AF_INET,SOCK_DGRAM,0);
			if(sock == INVALID_SOCKET)
			{
				MessageBox(hwndMain,"Unable to create broadcasting socket","ERROR",MB_OK);
				return;
			}
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			sa.sin_port = htons(UDP_LISTEN_PORT);
			// Set this socket as BROADCAST
			if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&val,sizeof(int)) == SOCKET_ERROR)
			{
				MessageBox(hwndMain,"setsockopt() failed","ERROR",MB_OK);
				return;
			}


			// SEND COMMAND RETR LIST
			if(sendto(sock,"RETR LIST",9,0,(SOCKADDR*)&sa, sizeof(sa))==SOCKET_ERROR)
       			MessageBox(hwndMain, "sendto() failed", "ERROR", MB_OK);
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"[local] Sent Broadcast");

			closesocket(sock);
			// DEBUG
			SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"[local] Closed UDP socket");
//			MessageBox(hwndMain, "FIN", "DEBUG", MB_OK);
			break;
		case IDM_TOOLS_SHARE:
			break;
		case IDM_TOOLS_NEW: // Removes all entries from listbox
			while (SendMessage(hListbox,LB_DELETESTRING,(WPARAM)0,0) > 0)
			   ;
			break;
		case IDM_TOOLS_SAVE:
			ZeroMemory(&opn,sizeof(OPENFILENAME));
			opn.lStructSize = sizeof(OPENFILENAME);
			opn.hwndOwner = hwndMain;
			opn.hInstance = hInst;
			//opn.lpstrFilter = "All Files;*\0\0";
			opn.lpstrCustomFilter = NULL;
			opn.nMaxFile = 255;
			opn.lpstrFileTitle = fileName;
			opn.nMaxFileTitle = 254;
			opn.lpstrInitialDir = szDirectory;

			GetSaveFileName(&opn);
			break;
		case IDM_ABOUT:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),
				hwndMain,AboutDlg);
			break;

		case IDM_EXIT:
		PostMessage(hwnd,WM_CLOSE,0,0);
		break;
	}
}

// THIS FUNCTION RETURNS the buffer in buf and the size of the packet in sz
int MakeListPacket(char *buf)
{
	WIN32_FIND_DATA lpFindData;
	char tmp;
	int newlen=0, oldlen;
	HANDLE handle;

	strcpy(szDirectory,"C:\\Share");
	SetCurrentDirectory(szDirectory);
	handle = FindFirstFile("*",&lpFindData);
	if (handle)
	{
		//buf = (char *)HeapAlloc(GetProcessHeap(),0,10000);
		//buf = (char *)LocalAlloc(LMEM_ZEROINIT,10000);
		strcpy(buf,"RLST    ");
		// We are using a buffer of 10,000 bytes. If it exceeds 10,000 we will drop the extra files
		do
		{
			oldlen = strlen(lpFindData.cFileName);
			//sprintf(buf+10,"%i",oldlen);
			//MessageBox(hwndMain,buf+10,"DEBUG",MB_OK);
			if(newlen > 10000)
				break;
			if(lpFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				strcpy(buf+8+newlen,lpFindData.cFileName);

				newlen += oldlen+1;
				*(buf+7+newlen) = ';'; // Add Delimiter
				//MessageBox(hwndMain,buf+8+newlen-oldlen,"DEBUG",MB_OK);
				//MessageBox(hwndMain,lpFindData.cFileName,"DEBUG",MB_OK);
			}
		}while(FindNextFile(handle,&lpFindData));

		newlen += 8;
		memcpy(buf+4,&newlen,sizeof(int));
		//tmp = buf[4];
		//buf[4] = buf[5];
		//buf[5] = tmp;
		*(buf+newlen) = '\0';
//		MessageBox(hwndMain,buf,"DEBUG",MB_OK);
	}
	else
	{
		newlen=8;
	}
	return newlen;
}

/*<---------------------------------------------------------------------->*/
/*@@2->@@*/
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	RECT rc, rcs, rct;
	SOCKET sock;
	SOCKADDR_IN sa;
	SOCKADDR_IN peer;
	char buf[16];
	char recvbuf[16];
	char *inbuf;
	int size;
	char szRemoteAddress[20];
	int retval;
	char *pStart, *pEnd;
	char pravinbuf[300];
	LPNMHDR pnmh;
	DWORD filesize, bytesread, bytestrans;
	HANDLE fd;
	char filebuf[1000];
	OPENFILENAME opn;
	char fileName[255];
	DWORD numby;


	switch (msg) {
	case WM_WSAASYNC:
		switch(WSAGETSELECTEVENT(lParam))
		{
			// THIS IS WHERE THE ACTION HAPPENS
			// I am the poo bear
			// Hell Yeah!
			case FD_ACCEPT:
				if(wParam == tcplistener)
				{
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Incoming TCP packet on tcplistener.");
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Creating temporary TCP socket to receive packet");
					// Create temporary socket
					size = sizeof(struct sockaddr);
					sock = accept(wParam, &sa, &size);
					if(sock == INVALID_SOCKET)
					{
						MessageBox(hwndMain,"accept() returned invalid socket!","ERROR",MB_OK);
						break;
					}
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Getting Header");
					// Get header
//					if(recv(sock,recvbuf,8,0) == SOCKET_ERROR)
//					{
//						MessageBox(hwndMain,"[tcplistener] recv() failed!","ERROR",MB_OK);
//						break;
//					}
recv(sock,recvbuf,8,0);
//					MessageBox(hwndMain,recvbuf,"DEBUG",MB_OK);
recvbuf[7]=0;
SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)recvbuf);
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Receivied Header. Determining packet type.");

					// **********************************************
					// RLIST BEGINS HERE
					// Retrieve list
					if(strncmp(recvbuf,"RLST",4)==0)
					{
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Packet Type confirmed as RLST");
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Getting length of rest of packet and allocating memory");

						memcpy(&size,recvbuf+4,sizeof(int));
sprintf(pravinbuf,"%i",size);
SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)pravinbuf);
						inbuf = (char*)HeapAlloc(GetProcessHeap(), 0, size);
						//inbuf = (char*)HeapAlloc(GetProcessHeap(), 0, 10000);

						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Receiving list from [remote] machine");
						recv(sock,inbuf,size,0);

						// Get Remote IP
						size = (sizeof(peer));
						if(getpeername(sock,&peer,&size) == 0)
							strcpy(szRemoteAddress,inet_ntoa(peer.sin_addr));

						sprintf(pravinbuf,"Remote IP address identified as [%s]",szRemoteAddress);
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)pravinbuf);

						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Beginning parsing list");
						// Parse all the files and add them to list
						pStart = inbuf;
						pEnd = pStart;
						while(*pEnd!=0)
						{
							if(*pEnd == ';')
							{
								*pEnd = 0;
								sprintf(pravinbuf,"%s : %s",szRemoteAddress,pStart);
								SendMessage(hListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)pravinbuf);
								pStart = pEnd+1;
							}
							pEnd++;
						}
						// The below is to aviod null shares
						if(pEnd - pStart > 2) // There is still one more entry left
						{
							*pEnd = 0;
							SendMessage(hListbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)pStart);
						}
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Parsing list ended.");
						// The structure of packet is RLST<file1>;<file2>;.....<filen>
						// NO SEMICOLON AT END!!!
						// Free allocated memory
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Freeing allocated memory");
						HeapFree(GetProcessHeap(),0,inbuf);
					}
					else if(strncmp(recvbuf,"RFIL",4)==0)
					{
						// Get Remote IP
						size = (sizeof(peer));
						if(getpeername(sock,&peer,&size) == 0)
							strcpy(szRemoteAddress,inet_ntoa(peer.sin_addr));

						sprintf(pravinbuf,"Remote host identified as [%s]",szRemoteAddress);
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)pravinbuf);

						// The structure of this is
						// RFILXXXX<filename>
						// pravinbuf is used because it is not used here
						recv(sock,pravinbuf,300,0);
						//MessageBox(hwndMain,pravinbuf,"DEBUG",MB_OK);
						// Set current directory
						SetCurrentDirectory(szDirectory);
						sprintf(filebuf,"%s\\%s",szDirectory,pravinbuf+1);
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)filebuf);

//						closesocket(sock);

						// Open file
						//fd = CreateFile(pravinbuf,GENERIC_READ,FILE_SHARE_READ,NULL,
						fd = CreateFile(filebuf,GENERIC_READ,FILE_SHARE_READ,NULL,
							OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
						if(fd == INVALID_HANDLE_VALUE)
		 				{

							sprintf(pravinbuf,"File request by %s cannot be processed",szRemoteAddress);
							MessageBox(hwndMain,filebuf,"ERROR",MB_OK);
							break;
						}
						filesize = GetFileSize(fd,NULL);
						bytestrans = 0;
						// Send file piece by piece
					//	MessageBox(hwndMain,"OLONGA","DEBUG",MB_OK);
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Transferring requested file");
						// Connect to TCP LISTENER
//						sa.sin_family = AF_INET;
//						sa.sin_addr.s_addr = inet_addr(szRemoteAddress);//peer.sin_addr.s_addr;
//						sa.sin_port = htons(TCP_LISTEN_PORT);

//						retval = connect(sock,&sa,sizeof(sa));
//						if(retval == SOCKET_ERROR)
//						{
//							retval = WSAGetLastError();
//							if(retval == WSAECONNREFUSED)
//								MessageBox(hwndMain,"[RFIL] Unable to connect to remote host","ERROR",MB_OK);
//							break;
//						}
//						while(bytestrans < filesize)
//						{

							// Send file
							if(!ReadFile(fd,&filebuf,1000,&bytesread,NULL))
							{
								MessageBox(hwndMain,"Unable to read file","ERROR",MB_OK);
								break;
							}
							filebuf[bytesread] = 0;
							send(sock,filebuf,1000,0);
							bytestrans += 1000;
//						}
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"File Transferred");
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Closing file handle");
						CloseHandle(fd);
					}
/*
					else // Its a file
					{
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Receiving file");

						// Recv File
						if(recv(sock,filebuf,1000,0) == SOCKET_ERROR)
						{
							MessageBox(hwndMain,"Recv failed [DBLCLK]","ERROR",MB_OK);
							break;
						}
						// DEBUG
						SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"File Transferred!!!");

						ZeroMemory(&opn,sizeof(OPENFILENAME));
						opn.lStructSize = sizeof(OPENFILENAME);
						opn.hwndOwner = hwndMain;
						opn.hInstance = hInst;
						//opn.lpstrFilter = "All Files;*\0\0";
						opn.lpstrCustomFilter = NULL;
						opn.nMaxFile = 255;
						opn.lpstrFileTitle = fileName;
						opn.nMaxFileTitle = 254;
						opn.lpstrInitialDir = szDirectory;

						GetSaveFileName(&opn);
						MessageBox(hwndMain,fileName,"DEBUG",MB_OK);

						sprintf(pravinbuf,"C:\\%s",fileName);
			//			MessageBox(hwndMain,daStr,"DEBUG",MB_OK);
						fd = CreateFile(pravinbuf, GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
						if(fd == INVALID_HANDLE_VALUE)
						{
							MessageBox(hwndMain,"Unable to open local file for writing","ERROR",MB_OK);
							break;
						}
						retval = strlen(inbuf);
						//WriteFile(fd,inbuf,index > 1000 ? 1000:index, numby, NULL);
//						while(1)
//						{
							WriteFile(fd,filebuf,1000, &numby, NULL);
//							if(recv(sock,inbuf,1000,0) == SOCKET_ERROR)
//								break;
//						}
//						MessageBox(hwndMain,inbuf,"DEBUG",MB_OK);
						CloseHandle(fd);
					}
*/
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Closing TCP socket");
					// Close socket
					closesocket(sock);
				}
				break;
			case FD_READ:
				if(wParam == udplistener) // We received broadcast
				{
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Received UDP Broadcast on udplistener");

					size = (sizeof(peer));
					recvfrom(wParam,buf,1000,0,&peer,&size);
					getpeername(wParam,&peer,&size);
					strcpy(szRemoteAddress,inet_ntoa(peer.sin_addr));

					sprintf(pravinbuf,"Remote machine identified as [%s]",szRemoteAddress);
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)pravinbuf);

//					MessageBox(hwndMain,szRemoteAddress,"DEBUG",MB_OK);
					// Now connect to the TCP socket and send list
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Creating TCP socket to connect to [remote] machine");
					sock = socket(AF_INET,SOCK_STREAM,0);
					sa.sin_family = AF_INET;
					sa.sin_addr.s_addr = inet_addr(szRemoteAddress);//peer.sin_addr.s_addr;
					sa.sin_port = htons(TCP_LISTEN_PORT);

					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Socket Created. Attempting connection");

					retval = connect(sock,&sa,sizeof(sa));
					if(retval == SOCKET_ERROR)
					{
						retval = WSAGetLastError();
						if(retval == WSAECONNREFUSED)
							MessageBox(hwndMain,"Unable to connect to remote host","ERROR",MB_OK);
						break;
					}
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Allocating Memory to send list of shares");
					// Create List Packet MAKE SURE TO FREE ALLOCATED MEMORY
					inbuf = (char *)HeapAlloc(GetProcessHeap(),0,10000);
					size = MakeListPacket(inbuf);

					// Send List
//					send(sock,"RLSTF000C:\\Downloads;C:\\Bigfoot;C:\\wonderwall.mp3",
//						strlen("RLST1600C:\\Downloads;C:\\Bigfoot;C:\\wonderwall.mp3 "),0);

					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Sending share list");
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)inbuf);
					sprintf(pravinbuf,"%i",size);
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)pravinbuf);

					if(send(sock,inbuf,size,0) == SOCKET_ERROR)
					{
						MessageBox(hwndMain,"Unable to send File List","ERROR",MB_OK);
					}
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Share list sent!");
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Memory freed");
					// Free memory by inbuf
					if(inbuf)
						HeapFree(GetProcessHeap(),0,inbuf);
					// DEBUG
					SendMessage(hMessages, LB_ADDSTRING,0,(LPARAM)"Closing socket");
					closesocket(sock);
				}
				break;
			case FD_CLOSE:
				closesocket(wParam);
				break;
		}
		break;
	case WM_SIZE:
		SendMessage(hWndStatusbar,msg,wParam,lParam);
		SendMessage(hwndToolBar,msg,wParam,lParam);
		InitializeStatusBar(hWndStatusbar,1);
		GetClientRect(hwndMain, &rc);
		GetClientRect(hwndToolBar, &rct);
		GetClientRect(hWndStatusbar, &rcs);
		rc.top = rct.bottom;
		rc.bottom = rc.bottom - (rcs.bottom + rc.top);
		MoveWindow(hListbox,rc.left,rc.top,rc.right,rc.bottom,TRUE);
		break;
	case WM_MENUSELECT:
		return MsgMenuSelect(hwnd,msg,wParam,lParam);
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hwnd,wParam,lParam,MainWndProc_OnCommand);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
/*
	case WM_NOTIFY:
		pnmh = (LPNMHDR)lParam;
		if(pnmh->hwndFrom == hwndToolBar)// && pnmh->code == NM_CLICK)
		{
			sprintf(buf,"%i",pnmh->idFrom);
			MessageBox(hwndMain,buf,"DEBUG",MB_OK);
			// Which button
			switch(pnmh->idFrom)
			{
				case 0:
					MessageBox(hwndMain,"BUTTON 0","DEBUG",MB_OK);
					break;
				case 2:
					MessageBox(hwndMain,"BUTTON 2","DEBUG",MB_OK);
					break;
				case 3:
					MessageBox(hwndMain,"BUTTON 3","DEBUG",MB_OK);
					break;
			}
		}
		break;
*/
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
/*@@3<-@@*/
	return 0;
}
/*@@2<-@@*/
/*<---------------------------------------------------------------------->*/
/* --- The following code comes from c:\lcc\lib\wizard\tcpini.tpl. */
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
#define NUM_TOOLBAR_BUTTONS		5
/* --- The following code comes from c:\lcc\lib\wizard\creatToolbar1.tpl. */
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
	    WS_CHILD|WS_BORDER,
	    0, 0, 0, 0, hwndParent, (HMENU) ID_TOOLBAR, hInst, NULL);

	// Send the TB_BUTTONSTRUCTSIZE message, which is required for
	// backward compatibility.
	SendMessage(hwndTB, TB_BUTTONSTRUCTSIZE,
	    (WPARAM) sizeof(TBBUTTON), 0);

	// Add the bitmap containing button images to the toolbar.
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID   = IDB_STD_LARGE_COLOR;
	SendMessage(hwndTB, TB_ADDBITMAP, (WPARAM) NUM_TOOLBAR_BUTTONS,(LPARAM) &tbab);

	// clean memory before using it
	memset(tbb,0,sizeof tbb);

	// Add the strings

	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Save");
	tbb[0].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Clear");
	tbb[1].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Share");
	tbb[2].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Update");
	tbb[3].iString = index;
	index = SendMessage(hwndTB,TB_ADDSTRING,0,(LPARAM)"Help");
	tbb[4].iString = index;
	// Button "Copy"
	tbb[0].iBitmap = STD_FILESAVE;
	tbb[0].idCommand = IDM_EDITCOPY;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;

	// Button "New"
	tbb[1].iBitmap = STD_FILENEW;
	tbb[1].idCommand = IDM_NEW;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;

	// Button "Properties"
	tbb[2].iBitmap = STD_PROPERTIES;
	tbb[2].idCommand = IDM_PROPERTIES;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;

	// Button "Redo"
	tbb[3].iBitmap = STD_REDOW;
	tbb[3].idCommand = IDM_EDITREDO;
	tbb[3].fsState = TBSTATE_ENABLED;
	tbb[3].fsStyle = TBSTYLE_BUTTON;

	// Button "Help"
	tbb[4].iBitmap = STD_HELP;
	tbb[4].idCommand = IDM_HELP;
	tbb[4].fsState = TBSTATE_ENABLED;
	tbb[4].fsStyle = TBSTYLE_BUTTON;

	SendMessage(hwndTB,TB_ADDBUTTONS,5,(LPARAM)&tbb);
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
	if ((hwndMain = CreatesherryWndClassWnd()) == (HWND)0)
		return 0;
	CreateSBar(hwndMain,"Ready",1);
	hwndToolBar = CreateAToolBar(hwndMain);
	hListbox = CreateWindowEx(WS_EX_CLIENTEDGE,
							"LISTBOX",
							"",
							WS_BORDER | WS_VSCROLL | WS_VISIBLE | LBS_STANDARD | WS_CHILD | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY,
							0,0,100,100,
							hwndMain,
							NULL,
							hInstance,
							NULL);
	OldListBoxProc = (WNDPROC)SetWindowLong(hListbox,GWL_WNDPROC,(LONG)ListBoxProc);

	hMessages = CreateWindowEx(WS_EX_CLIENTEDGE,
							"LISTBOX",
							"Debug Messages",
							WS_OVERLAPPEDWINDOW|WS_BORDER | WS_VSCROLL | WS_VISIBLE |LBS_NOINTEGRALHEIGHT,
							0,0,500,400,
							NULL,
							NULL,
							hInstance,
							NULL);

	ShowWindow(hwndMain,SW_SHOW);
	WsaData = InitWS2();
	if (WsaData == NULL)
		return 0;

	// Create UDP Listening socket
	udplistener = socket(AF_INET, SOCK_DGRAM, 0);
	if (udplistener == INVALID_SOCKET)
	{
		WSACleanup();
		MessageBox(hwndMain, "Unable to create UDP Listener socket. Exiting ...", "ERROR", MB_OK);
		return 0;
	}
	// Set it into ASYNC mode
	WSAAsyncSelect(udplistener, hwndMain, WM_WSAASYNC, FD_READ | FD_CLOSE | FD_ACCEPT);
	// Fill in the sockaddr values
	udplistener_sa.sin_family = AF_INET;
	udplistener_sa.sin_addr.s_addr = htonl(INADDR_ANY);
	udplistener_sa.sin_port = htons(UDP_LISTEN_PORT);
	// Bind the socket
	if(bind(udplistener,&udplistener_sa, sizeof(struct sockaddr)) < 0)
	{
		MessageBox(hwndMain, "Unable to bind UDP Listener socket. Exiting ...", "ERROR", MB_OK);
		WSACleanup();
		return 0;
	}

	// Create TCP listener socket
	tcplistener = socket(AF_INET, SOCK_STREAM, 0);
	if (tcplistener == INVALID_SOCKET)
	{
		WSACleanup();
		MessageBox(hwndMain, "Unable to create TCP Listener socket. Exiting ...", "ERROR", MB_OK);
		return 0;
	}
	// Set it into ASYNC mode
	WSAAsyncSelect(tcplistener, hwndMain, WM_WSAASYNC, FD_READ | FD_CLOSE | FD_ACCEPT);
	// Fill in the sockaddr values
	tcplistener_sa.sin_family = AF_INET;
	tcplistener_sa.sin_addr.s_addr = htonl(INADDR_ANY);
	tcplistener_sa.sin_port = htons(TCP_LISTEN_PORT);
	// Bind the socket
	if(bind(tcplistener,&tcplistener_sa, sizeof(struct sockaddr)) < 0)
	{
		MessageBox(hwndMain, "Unable to bind TCP Listener socket. Exiting ...", "ERROR", MB_OK);
		WSACleanup();
		return 0;
	}
	// Set it in listen mode
	if(listen(tcplistener,SOMAXCONN) == SOCKET_ERROR)
	{
		MessageBox(hwndMain, "Listen on TCP Listener socket failed. Exiting ...", "ERROR", MB_OK);
		WSACleanup();
		return 0;
	}

	while (GetMessage(&msg,NULL,0,0)) {
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	WSACleanup();
	return msg.wParam;
}
