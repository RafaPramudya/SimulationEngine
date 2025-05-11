#include "threading.h"

void update_thread_func(Engine* engine) {
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;

    while (engine->isRunning.load()) {
        last = now;
        now = SDL_GetPerformanceCounter();
        f64 dT = (double)((now - last) / (double) SDL_GetPerformanceFrequency());

        // Lock the mutex and update the engine state
        {
            std::unique_lock<std::mutex> lock(engine->physicsMutex);
            engine_update(engine->engineState, dT);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // ~120 FPS
    
    }
}