#ifndef EVENT_H
#define EVENT_H

#include "utils/types.h"

#include <stdio.h>

// 256 key for 8 bits per byte
#define KEY_ARRAY_SIZE 32

#define SET_KEY_STATE(keyArr, vkCode, isDown) \
    if (vkCode < 256) { \
        int byteIdx = vkCode / 0x8; \
        int bitIdx = vkCode % 0x8; \
        if (isDown) keyArr[byteIdx] |= (1 << bitIdx); \
        else keyArr[byteIdx] &= ~(1 << bitIdx); \
    }
#define IS_KEY_DOWN(keyArr, vkCode) \
    ((vkCode < 256) ? (keyArr[vkCode / 0x8] & (1 << (vkCode % 0x8))) != 0 : 0)

typedef struct Event {
    u8 keyStates[KEY_ARRAY_SIZE];
    bool isWindowActive;
} Event;

#endif