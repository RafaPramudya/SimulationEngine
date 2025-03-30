#ifndef ENGINE_H
#define ENGINE_H

#include <SDL3/SDL.h>
#include <gl/glew.h>
#include <GL/GL.h>

#include <stdio.h>
#include <stdbool.h>


typedef struct Engine {
    int width;
    int height;
    float refreshRate;

    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;

    bool isRunning;
    double ticksPassed;
    double currentTicks;

} Engine;

extern Engine engineInstance;

bool engineInitialization(Engine* engine, const char* title);

void engineEvent(Engine* engine);

void engineUpdate(Engine* engine);

void engineRender(Engine* engine);

void engineDeInitialization(Engine* engine);

#endif