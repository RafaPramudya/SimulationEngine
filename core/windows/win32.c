#include "win32.h"
#include "appstate.h"
#include "context.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            if (MessageBox(hwnd, L"Yakin mau keluar?", L"Keluar", MB_OKCANCEL) == IDOK) {
                DestroyWindow(hwnd);
            } else {
                MessageBox(hwnd, L"Oke :>", L"Info", MB_OK);
            }
            return 0;
        case WM_DESTROY:
            context_destroy();
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
    
BOOL InitializeWindow(HWND* _outWndHandle, HINSTANCE hInstance, int nCmdShow, u32 w_width, u32 w_height) {
    const wchar_t WindowName[] = L"Tung Tung Engine";
    const wchar_t ClassName[] = L"Engine Window Class";
    WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = ClassName;

    RegisterClassEx(&wcex);

    i32 ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    i32 ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT windowRect;
    SetRect(&windowRect,
        (ScreenWidth / 2) - (w_width / 2), (ScreenHeight / 2) - (w_height / 2),
        (ScreenWidth / 2) + (w_width / 2), (ScreenHeight / 2) + (w_height / 2)
    );
    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);

    AdjustWindowRectEx(&windowRect, style, FALSE, 0);

    HWND hwnd = CreateWindowEx(
        0,
        ClassName,
        WindowName,
        style,
        windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    HDC hdc = GetDC(hwnd);

    if (hwnd == NULL) {
        return FALSE;
    }

    appState_init(hwnd, hdc, w_width, w_height);

    context_init();

    *_outWndHandle = hwnd;
    ShowWindow(hwnd, nCmdShow);
    return TRUE;
}