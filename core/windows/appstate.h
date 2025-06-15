#ifndef APPSTATE_H
#define APPSTATE_H

#include "utils/types.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class State {
public:
    State(HWND hwnd_in, HDC hdc_in, u32 width, u32 height);
    ~State() = default;

    f64 deltaTime;
    f64 passedTime;

    inline bool isRunning() const { return running; }
    inline void setRunning(bool status) { running = status; }

    inline u32 getWidth() const { return width; }
    inline u32 getHeight() const { return height; }

    inline HWND& getHWND() {return hwnd;}
    inline HDC& getHDC() {return hdc;}
    inline HGLRC& getHGLRC() {return hglrc;}

    void resize(u32 width, u32 height);
private:
    bool running;
    
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;

    u32 width;
    u32 height;
};

extern State* state;
#endif // APPSTATE_H