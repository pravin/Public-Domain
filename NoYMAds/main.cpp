// This code illustrates hiding the Ad in the Buddy window of Yahoo Messenger
//
// Copyright (c) 2006, Pravin Paratey
// pravinp[at]gmail[dot]com (http://www.dustyant.com)


#include <windows.h>

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszArgs, int nCmdWnd)
{
    HWND hYahooWnd = FindWindowEx(NULL, NULL, "YahooBuddyMain", NULL);
    if(hYahooWnd) // Yahoo Messenger is running
    {
        HWND hAtlWnd = FindWindowEx(hYahooWnd, NULL, "ATL:00821BC0", NULL);
        if(hAtlWnd) // Found the window
        {
            HWND hShellWnd = FindWindowEx(hAtlWnd, NULL, "Shell Embedding", NULL);
            if(hShellWnd)
            {
                HWND hDocObjWnd = FindWindowEx(hShellWnd, NULL, "Shell DocObject View", NULL);
                if(hDocObjWnd) // This is the window we must hide
                {
                    ShowWindow(hDocObjWnd, SW_HIDE);
                }
            }
        }
    }
    else
    {
        MessageBox(NULL, "No instances of Yahoo Messenger found", "Error", MB_OK);
    }
    return 0;
}
