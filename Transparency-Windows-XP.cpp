/* pTrans - Adds tranlucency to any window on desktop
 * An attempt at understanding layered windows
 * Pravin Paratey (October 11, 2002)
**/
#include <windows.h>
#include <winuser.h>

WNDCLASSEX wcx;
HWND hwndMain;
HINSTANCE hInstance;

    
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND CreateMainWindow();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, 
                    LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg; /* Application messages are saved here */    
    
    hInstance = hInst;
    hwndMain = CreateMainWindow();
    
    if (!hwndMain)
    {
        MessageBox(NULL, "Unable to create window", "pTrans", 
                MB_OK|MB_ICONSTOP);
    }
    
    /* Set the WS_EX_LAYERED style */
    SetWindowLong(hwndMain, GWL_EXSTYLE, GetWindowLong(hwndMain, GWL_EXSTYLE) |
        WS_EX_LAYERED);
        
    /* Make the window 60% alpha */
    SetLayeredWindowAttributes(hwndMain, 0, (255 * 60) / 100, LWA_ALPHA);
    /* Show the window */
    ShowWindow(hwndMain, nCmdShow);
    
    /* Get message from the message queue */
    while (GetMessage (&msg, NULL, 0, 0))
    {
        /* Translates virtual key messages to character messages */
        TranslateMessage(&msg);
        /* Sends message to the window proc */        
        DispatchMessage(&msg);
    }
    
    /* Return value, 0 if WM_QUIT message posted */
    return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

HWND CreateMainWindow()
{
    HWND hwnd;
    
    /* Make the contents of the memory 0 */
    ZeroMemory(&wcx, sizeof(WNDCLASSEX));
    /* Fill the WNDCLASSEX structure */
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_DBLCLKS;
    wcx.lpfnWndProc = MainWndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = "pTrans";
    wcx.hIconSm = NULL;
        /* Register the class */
    if (!RegisterClassEx(&wcx))
    {
        MessageBox (NULL, "Unable to register window class", "Lesson 2",
                MB_OK | MB_ICONSTOP);
        return 0;
    }
    
    /* Create the window */
    hwnd = CreateWindowEx(0,
        "pTrans",     /* Class name as defined in WNDCLASSEX */
        "Lesson 2 - Understanding messages and events", /* Title of window */
        WS_OVERLAPPEDWINDOW, /* Window styles */
        CW_USEDEFAULT, /* X co-ordinate of top left corner of window */
        CW_USEDEFAULT, /* Y co-ordinate of top left corner of window */
        CW_USEDEFAULT, /* Width of window */
        CW_USEDEFAULT, /* Height of window */
        NULL,          /* Handle to parent, NULL signifies Desktop */
        NULL,          /* Handle to menu */
        hInstance,     /* Handle to application instance */
        NULL);         /* Pointer to window creation data */
        
    return hwnd;
}
