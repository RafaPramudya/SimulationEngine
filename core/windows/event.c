#include "event.h"

void CloseMessage(HWND hwnd) {
    if (MessageBox(hwnd, L"Yakin mau keluar?", L"Keluar", MB_OKCANCEL) == IDOK) {
        DestroyWindow(hwnd);
    } else {
        MessageBox(hwnd, L"Oke :>", L"Info", MB_OK);
    }
}
void PaintMessage(HWND hwnd, AppState* appState) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    EndPaint(hwnd, &ps);
}