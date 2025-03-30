#include "macro.h"

#include <stdio.h>

#include "SDL3/SDL.h"

void __err_log(const char* log) {
    fprintf(stderr, "ERROR : %s, %s, %d", log, __FILE__, __LINE__);
}

void __sdl_err_log(const char* log ) {
    __err_log(log);
    fprintf(stderr, "%s", SDL_GetError());
}