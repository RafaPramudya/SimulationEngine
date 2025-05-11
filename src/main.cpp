#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine.h"
#include "threading.h"

#include "constants.h"

int main(int argc, char* argv[]) {

#pragma region debug_segment
#ifdef DEBUG
    printf("Running on Debug Mode\n");
#else
    printf("Running on Something-Else mode\n");
#endif

#ifdef DEBUG
    printf("Berjalan di Arsitektur %d-bit\n", sizeof(void*) * 8);
#endif
#pragma endregion
    Engine engine;
    EngineState engine_state;

    if (engineState_create(&engine_state, "Tung Tung Sahur") == false) {
        return -1;
    }

    engine.isRunning = true;
    engine.engineState = &engine_state;

    std::thread update_thread(update_thread_func, &engine);

    Uint64 last_frame_time = SDL_GetPerformanceCounter();
    
    while (engine.isRunning.load()) {
        // Calculate delta time for frame pacing
        Uint64 now = SDL_GetPerformanceCounter();
        float delta_time = (float)(now - last_frame_time) / SDL_GetPerformanceFrequency();
        last_frame_time = now;

        // Calculate and display FPS in the window title
#if defined (LOG_FPS) && defined (DEBUG)
        static int frame_count = 0;
        frame_count++;
        static float elapsed_time = 0;
        elapsed_time += delta_time;

        if (elapsed_time >= 1.0f) {
            int fps = frame_count;
            frame_count = 0;
            elapsed_time = 0;

            char title[256];
            snprintf(title, sizeof(title), WINDOW_TITLE " - %d fps", fps);
            SDL_SetWindowTitle(engine_state.window, title);
        }
#endif
        
        while (SDL_PollEvent(&engine.engineState->event)) {
            switch (engine.engineState->event.type) {
            case SDL_EVENT_QUIT:
                // Set Running to false
                engine.isRunning.store(false);
                break;
            case SDL_EVENT_KEY_DOWN:
                break;
            }
        }

        // Lock and render
        {
            std::lock_guard<std::mutex> lock(engine.physicsMutex);
            engine_render(engine.engineState);
        }
    }

    update_thread.join();

    engineState_destroy(&engine_state);

    return 0;
}