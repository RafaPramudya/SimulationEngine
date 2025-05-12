#ifndef EVENT_H
#define EVENT_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "appstate.h"

void CloseMessage(HWND hwnd);
void PaintMessage(HWND hwnd, AppState* appState);

#endif // EVENT_H