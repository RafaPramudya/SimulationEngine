#include "appstate.h"

AppState appstate_ins;

bool appState_init(HWND hwnd_in) {
    appstate_ins.passedTime = 0.0;
    appstate_ins.deltaTime = 0.0;

    appstate_ins.hwnd = hwnd_in;

    appstate_ins.isRunning = true;

    return true;
}