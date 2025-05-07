#ifndef ENGINE_H
#define ENGINE_H

#include <SDL3/SDL.h>
#include <gl/glew.h>
#include <GL/GL.h>

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>
#include "globals.h"

typedef enum WARNA {
    MERAH,
    KUNING,
    HIJAU
} WARNA;
typedef struct Engine {
    i32 width;
    i32 height;
    f32 refreshRate;

    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;
    WARNA warna_lampu;

    bool isRunning;
    f64 ticksPassed;
    f64 currentTicks;

} Engine;

extern Engine engineInstance;

bool engine_init(Engine* engine, const char* title);

void engine_event(Engine* engine);

void engine_update(Engine* engine, f64 dT);

void engine_render(Engine* engine);

void engine_deinit(Engine* engine);

#endif