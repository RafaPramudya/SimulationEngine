#ifndef EVENT_H
#define EVENT_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "utils/types.h"

#include <stdio.h>

// 256 key for 8 bits per byte
#define KEY_ARRAY_SIZE 32

#define SET_KEY_STATE(vkCode, isDown) \
    if (vkCode < 256) { \
        int byteIdx = vkCode / 0x8; \
        int bitIdx = vkCode % 0x8; \
        if (isDown) event.keyStates[byteIdx] |= (1 << bitIdx); \
        else event.keyStates[byteIdx] &= ~(1 << bitIdx); \
    }
#define IS_KEY_DOWN(vkCode) \
    ((vkCode < 256) ? (event.keyStates[vkCode / 0x8] & (1 << (vkCode % 0x8))) != 0 : 0)
#define IS_KEY_PRESSED(vkCode) \
    ((vkCode < 256) && ((event.keyStates[vkCode / 8] & (1 << (vkCode % 8))) && !((event.prevKeyStates[vkCode / 8] & (1 << (vkCode % 8))))))
#define IS_KEY_RELEASED(vkCode) \
    ((vkCode < 256) && !((event.keyStates[vkCode / 8] & (1 << (vkCode % 8))) && ((event.prevKeyStates[vkCode / 8] & (1 << (vkCode % 8))))))

#define TOGGLE_SWITCH_ON_PRESS(vkCode) \
    if (IS_KEY_PRESSED(vkCode)) { \
        int byteIdx = vkCode / 8; \
        int bitIdx = vkCode % 8; \
        event.keySwitches[byteIdx] ^= (1 << bitIdx); \
    } \

#define IS_SWITCH_ON(vkCode) \
    ((vkCode < 256) ? (event.keySwitches[vkCode / 8] & (1 << (vkCode % 8))) != 0 : 0)

typedef struct MouseState {
    bool isCaptured;
    i32 deltaX, deltaY;
} MouseState;
typedef struct Event {
    u8 keyStates[KEY_ARRAY_SIZE];
    u8 keySwitches[KEY_ARRAY_SIZE];
    u8 prevKeyStates[KEY_ARRAY_SIZE];

    bool isWindowActive;
    MouseState mouseState;
} Event;

void EventInit(void);
void CaptureMouse(bool captured);

#endif