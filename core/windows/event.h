#ifndef EVENT_H
#define EVENT_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "utils/types.h"

#include <cstdio>
#include <bitset>

#ifdef __cplusplus

class MouseState {
    private:
    bool captured;
    public:
    i32 deltaX, deltaY;

    MouseState() = default;
    ~MouseState() = default;

    bool isCaptured() { return captured; }
    void setCaptured(bool status) { captured = status; }
};

class Event {
private:
    std::bitset<256> keyState;
    std::bitset<256> prevKeyState;

    bool windowActive;
public:
    static Event& instance() {
        static Event instance;
        return instance;
    }    
    Event();
    ~Event() = default;

    MouseState mouseState;

    void captureMouse(bool captured);

    void setKeyDown(u32 vkCode, bool isDown);
    bool isKeyDown(u32 vkCode) const;
    bool isKeyPressed(u32 vkCode) const;
    bool isKeyReleased(u32 vkCode) const;

    void swapKeyBuffer();
    inline void clearKeyBuffer() {
        keyState.reset();
        prevKeyState.reset();
    }

    bool isWindowActive(void) const { return windowActive; }
    void setWindowActive(bool status) { windowActive = status; }    
};

extern Event* event;

#endif
#endif