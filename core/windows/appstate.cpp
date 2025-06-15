#include "appstate.h"

State* state = nullptr;

State::State(HWND hwnd_in, HDC hdc_in, u32 width, u32 height) {
    passedTime = 0.0;
    deltaTime = 0.0;

    hwnd = hwnd_in;
    hdc = hdc_in;

    State::width = width;
    State::height = height;

    running = true;
}

void State::resize(u32 width, u32 height) {
    State::width = width;
    State::height = height;
}