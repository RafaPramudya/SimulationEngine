#include "win32.h"
#include "appstate.h"
#include "event.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    AppState* appState;
    if (uMsg == WM_CREATE) {
        appState_setPState(hwnd, lParam);
        appState = appState_getPState(hwnd);
    } else {
        appState = appState_getPState(hwnd);
    }

    switch (uMsg) {
        case WM_CLOSE:
            CloseMessage(hwnd);
            return 0;
        case WM_PAINT:
            PaintMessage(hwnd, appState);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            appState_destroy(appState);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL InitializeWindow(HWND* _outWndHandle, HINSTANCE hInstance, int nCmdShow) {
    AppState* appState = appState_create();

    const wchar_t CLASS_NAME[] = L"Engine Window Class";
    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Tung Tung Engine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        appState // Pass the AppState as the lpCreateParams
    );

    if (hwnd == NULL) {
        return FALSE;
    }

    *_outWndHandle = hwnd;
    ShowWindow(hwnd, nCmdShow);

    return TRUE;
}