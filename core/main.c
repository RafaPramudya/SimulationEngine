#include "windows/win32.h"
#include "windows/appstate.h"
#include "render/renderer.h"
#include "wchar.h"

extern AppState appstate_ins;

static f64 get_hp_time(void) {
    LARGE_INTEGER frequency, time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
    return (f64)time.QuadPart / frequency.QuadPart;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    HWND hwnd;
    if (!InitializeWindow(&hwnd, hInstance, nCmdShow, 800, 600)) {
        return 0;
    }

    PrepareRenderer();

    f64 lastTime = get_hp_time();
    f64 currentTime, fps;
    f64 passedFPS = 0.0;
    int frameCount = 0;

    while (appstate_ins.isRunning) {
        currentTime = get_hp_time();
        appstate_ins.deltaTime = currentTime - lastTime;
        appstate_ins.passedTime += appstate_ins.deltaTime;
        lastTime = currentTime;

        #ifdef DEBUG
        frameCount++;
        passedFPS += appstate_ins.deltaTime;

        if (passedFPS >= 1.0) {

            wchar_t buffer[128];
            swprintf(buffer, 128, L"Tung Tung Sahur - %d FPS, %lf dT", frameCount, appstate_ins.deltaTime);
            SetWindowText(hwnd, buffer);

            frameCount = 0;
            passedFPS = 0.0;
        }
        #endif

        MSG msg;
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                appstate_ins.isRunning = false;
                continue;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        RenderLoop();
    }

    return 0;
}