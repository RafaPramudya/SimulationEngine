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

    if (engineInitialization(&engineInstance, "Tung Tung Sahur") == false) {
        return -1;
    }

    while (engineInstance.isRunning) {
        engineEvent(&engineInstance);
        engineUpdate(&engineInstance);
        engineRender(&engineInstance);
    }

    engineDeInitialization(&engineInstance);
   return 0;
}