#ifndef WIN32OS_H
#define WIN32OS_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL InitializeWindow(HWND* _outWndHandle, HINSTANCE hInstance, int nCmdShow);

#endif // WINLIBOS_H