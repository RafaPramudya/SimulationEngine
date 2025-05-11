#ifndef ENGINE_H
#define ENGINE_H

#include <SDL3/SDL.h>
#include <gl/glew.h>
#include <GL/GL.h>

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>
#include "globals.h"

typedef struct EngineState {
    SDL_Window* window;
    SDL_GLContext gl_context;

    SDL_Event event;

    struct render_data_t {
        i32 width;
        i32 height;
        f32 refreshRate;
    } render_data;
    struct update_data_t {
        f64 currentTicks;
        f64 ticksPassed;
    } update_data;

} EngineState;

#ifdef __cplusplus
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

typedef struct Engine {
    std::atomic<bool> isRunning{true};
    std::mutex physicsMutex;
    
    EngineState* engineState;
} Engine;

#endif

#ifdef __cplusplus
extern "C" {
#endif
bool engineState_create(EngineState* state, const char* title);

void engine_update(EngineState* state, f64 dT);

void engine_render(EngineState* state);

void engineState_destroy(EngineState* state);

#ifdef __cplusplus
}   
#endif

#endif