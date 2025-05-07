#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine.h"

Engine engineInstance;

int main(int argc, char* argv[]) {

#ifdef DEBUG
    printf("Running on Debug Mode\n");
#else
    printf("Running on Something-Else mode\n");
#endif

#ifdef DEBUG
    if (sizeof(void*) == 8) {
        printf("Runs on 64-bit architecture\n");
    } else if (sizeof(void*) == 4) {
        printf("Runs on 32-bit architecture\n");
    } else {
        printf("What the fuck are you using?\n");
    }
#endif

    if (engine_init(&engineInstance, "Tung Tung Sahur") == false) {
        return -1;
    }

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;

    while (engineInstance.isRunning) {
        last = now;
        now = SDL_GetPerformanceCounter();

        engine_event(&engineInstance);
        engine_update(&engineInstance, (double)((now - last) / (double) SDL_GetPerformanceFrequency()));
        engine_render(&engineInstance);
    }

    engine_deinit(&engineInstance);
   return 0;
}