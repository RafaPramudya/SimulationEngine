#include "windows/win32.h"
#include "windows/appstate.h"
#include "windows/event.h"
#include "windows/context.h"
#include "render/renderer.h"
#include "render/camera.h"
#include "component/component.h"
#include "wchar.h"

#include <cstdio>

static f64 get_hp_time(void) {
    LARGE_INTEGER frequency, time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
    return (f64)time.QuadPart / frequency.QuadPart;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    #ifdef DEBUG
    AllocConsole();

    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    freopen_s(&dummy, "CONOUT$", "w", stderr);
    #endif

    Event event_ins;
    event = &event_ins;

    HWND hwnd;
    if (!InitializeWindow(&hwnd, hInstance, nCmdShow, 800, 600)) {
        return 0;
    }

    State state_ins(hwnd, GetDC(hwnd), 800, 600);
    state = &state_ins;
    
    Context::init();

    Renderer render_ins;
    renderer = &render_ins;

    Camera camera_ins;
    camera = &camera_ins;

    f64 lastTime = get_hp_time();
    f64 currentTime, fps;
    f64 passedFPS = 0.0;
    int frameCount = 0;

    while (state->isRunning()) {
        currentTime = get_hp_time();
        state->deltaTime = currentTime - lastTime;
        state->passedTime += state->deltaTime;
        lastTime = currentTime;

        #ifdef DEBUG
        frameCount++;
        passedFPS += state->deltaTime;

        if (passedFPS >= 1.0) {

            wchar_t buffer[128];
            swprintf(buffer, 128, L"Tung Tung Sahur - %d FPS, %lf dT", frameCount, state->deltaTime);
            SetWindowText(hwnd, buffer);

            frameCount = 0;
            passedFPS = 0.0;
        }
        #endif

        MSG msg;
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                state->setRunning(false);
                continue;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        
        renderer->renderEventUpdate();
        renderer->render();
        event->swapKeyBuffer();
        printf("");
    }

    Context::destroy();
    return 0;
}