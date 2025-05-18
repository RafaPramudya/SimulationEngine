#include "appstate.h"

// Global appstate instance
AppState appstate_ins;

bool appState_init(HWND hwnd_in, HDC hdc_in, u32 width, u32 height) {
    appstate_ins.passedTime = 0.0;
    appstate_ins.deltaTime = 0.0;

    appstate_ins.hwnd = hwnd_in;
    appstate_ins.hdc = hdc_in;

    appstate_ins.width = width;
    appstate_ins.height = height;

    appstate_ins.isRunning = true;

    return true;
}