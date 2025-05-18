#include "win32.h"
#include "appstate.h"

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
            if (MessageBox(hwnd, L"Yakin mau keluar?", L"Keluar", MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            } else {
                MessageBox(hwnd, L"Oke :>", L"Info", MB_OK);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(hwnd, &ps);
            }
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