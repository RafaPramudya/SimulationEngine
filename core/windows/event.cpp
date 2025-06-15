#include "event.h"
#include "appstate.h"

#include <cstdio>

Event* event = nullptr;

Event::Event() {
    windowActive = true;
    mouseState.setCaptured(true);

    Event::clearKeyBuffer();
}

void Event::captureMouse(bool captured) {
    if (captured) {
        SetCapture(state->getHWND());

        RECT rect;
        GetClientRect(state->getHWND(), &rect);
        POINT center = {
            (rect.right - rect.left) / 2,
            (rect.bottom - rect.top) / 2
        };
        ClientToScreen(state->getHWND(), &center);
        SetCursorPos(center.x, center.y);
    } else  {
        ShowCursor(TRUE);
        ReleaseCapture();
    }

    mouseState.setCaptured(captured);
}


void Event::setKeyDown(u32 vkCode, bool isDown) {
    if (vkCode < 256) {
        keyState.set(vkCode, isDown);
    }
}
bool Event::isKeyDown(u32 vkCode) const {
    // printf("Event::isKeyDown called for vkCode: %u\n", vkCode);
    if (vkCode < 256)
        return keyState.test(vkCode);
    return false;
}
bool Event::isKeyPressed(u32 vkCode) const {
    if (vkCode < 256) {
        return (keyState[vkCode] == true && prevKeyState[vkCode] == false);
    }
    return false;
}
bool Event::isKeyReleased(u32 vkCode) const {
    if (vkCode < 256) {
        return (keyState[vkCode] == false && prevKeyState[vkCode] == true);
    }
    return false;
}

void Event::swapKeyBuffer() {
    prevKeyState = keyState;
}