#include "appstate.h"

// Global appstate instance
AppState appstate;

bool appState_init(HWND hwnd_in, HDC hdc_in, u32 width, u32 height) {
    appstate.passedTime = 0.0;
    appstate.deltaTime = 0.0;

    appstate.hwnd = hwnd_in;
    appstate.hdc = hdc_in;

    appstate.width = width;
    appstate.height = height;

    appstate.isRunning = true;

    return true;
}