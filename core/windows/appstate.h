#ifndef APPSTATE_H
#define APPSTATE_H

#include "utils/types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct AppState_s {
    f64 deltaTime;
    f64 passedTime;

    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;

    u32 width;
    u32 height;

    bool isRunning;
} AppState;

bool appState_init(HWND hwnd_in, HDC hdc_in, u32 width, u32 height);

#endif // APPSTATE_H