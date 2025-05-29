#include "event.h"
#include "appstate.h"

Event event;
extern AppState appstate;

void EventInit(void) {
    event.isWindowActive = true;
    event.mouseState.isCaptured = true;
    
    memset(event.keyStates, 0, sizeof(event.keyStates));
    memset(event.prevKeyStates, 0, sizeof(event.prevKeyStates));
    memset(event.keySwitches, 0, sizeof(event.keySwitches));
}

void CaptureMouse(bool captured) {
    if (captured) {
        SetCapture(appstate.hwnd);

        RECT rect;
        GetClientRect(appstate.hwnd, &rect);
        POINT center = {
            (rect.right - rect.left) / 2,
            (rect.bottom - rect.top) / 2
        };
        ClientToScreen(appstate.hwnd, &center);
        SetCursorPos(center.x, center.y);
    } else {
        ShowCursor(TRUE);
        ReleaseCapture();
    }

    event.mouseState.isCaptured = captured;
}