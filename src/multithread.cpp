#include "multithread.h"

#include <thread>
#include <atomic>
#include <chrono>

static std::atomic<bool> isRunning(true);

extern "C" {
    void set_is_running(bool value) {
        isRunning.store(value); // Set the atomic value
    }

    bool get_is_running() {
        return isRunning.load(); // Get the atomic value
    }

    void multithread_run(Engine* engine) {
        set_is_running(true);
        
        Uint64 now = SDL_GetPerformanceCounter();
        Uint64 last = 0;

        std::thread updateThread([&]() {
            while (get_is_running()) {
                last = now;
                now = SDL_GetPerformanceCounter();
                f64 dT = (double)((now - last) / (double) SDL_GetPerformanceFrequency());
                
                if (dT < (1.0 / 120.0)) {
                    std::this_thread::sleep_for(std::chrono::duration<f64>(1.0 / 120.0 - dT));
                    now = SDL_GetPerformanceCounter();
                    dT = (double)((now - last) / (double) SDL_GetPerformanceFrequency());
                }

                engine_update(engine, dT);
            }
        });

        Uint64 now_render = SDL_GetPerformanceCounter();
        Uint64 last_render = 0;

        std::thread renderThread([&]() {
            while (get_is_running()) {
                engine_render(engine);

                #ifdef LOG_FPS
                last_render = now_render;
                now_render = SDL_GetPerformanceCounter();
                f64 dT = (double)((now_render - last_render) / (double) SDL_GetPerformanceFrequency());
                // printf("FPS : %lf\n", 1 / dT);

                static i32 frameCount = 0;
                static f64 elapsedTime = 0.0;

                frameCount++;
                elapsedTime += dT;

                if (elapsedTime >= 1.0) {
                    char title[256];
                    snprintf(title, sizeof(title), "Tung Tung Sahur - %d fps", frameCount);
                    SDL_SetWindowTitle(engine->window, title);

                    frameCount = 0;
                    elapsedTime = 0.0;
                }
                #endif
            }
        });

        while (engine->isRunning) {
            engine_event(engine);
        }

        set_is_running(false); // Stop the threads
        updateThread.join();
        renderThread.join();
    }
}