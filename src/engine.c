#include "engine.h"

#include "constants.h"

bool engineState_create(EngineState* state, const char* title) {
    state->render_data.width = 0;
    state->render_data.height = 0;
    state->render_data.refreshRate = 0.0f;
    state->window = NULL;
    state->gl_context = NULL;
    state->update_data.ticksPassed = 0.0f;
    state->update_data.currentTicks = 0.0f;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) == false ) {
        SDL_ERR_LOG("SDL_Init Error");
        return false;
    }

    int count;
    SDL_DisplayID* displays = SDL_GetDisplays(&count);

    if (displays == NULL || count == 0) {
        SDL_ERR_LOG("SDL_GetDisplays tidak ditemukan");
    }

    const SDL_DisplayMode* dm = SDL_GetCurrentDisplayMode(displays[0]);
    // state->render_data.width = dm->w;
    // state->render_data.height = dm->h;
    state->render_data.width = 800;
    state->render_data.height = 600;
    state->render_data.refreshRate = dm->refresh_rate;

    uint32_t window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
    state->window = SDL_CreateWindow(title, state->render_data.width, state->render_data.height, window_flags);
    // state->window = SDL_CreateWindow(title, 800, 600, window_flags);
    if (state->window == NULL) {
        SDL_ERR_LOG("SDL_Window tidak bisa dibuat");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGL_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    state->gl_context = SDL_GL_CreateContext(state->window);
    if (state->gl_context == NULL) {
        SDL_ERR_LOG("SDL_Context tidak bisa dibuat");
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();

    if (glewErr != GLEW_OK) {
        ERR_LOG("GLEW Inisiasi");
        return false;
    }

    // Set VSYNC only when VSYNC macro is defined
#ifdef V_SYNC
    if (SDL_GL_SetSwapInterval(1) < 0) {
        ERR_LOG("VSync tidak bisa diaktifkan");
        return false;
    }
#endif

#ifdef DEBUG

    int nrAttr;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr);

    printf("Versi OpenGL : %s\n", glGetString(GL_VERSION));
    printf("Platform OpenGL : %s, Renderer OpenGL: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("Maksimal Vertex Attribut : %d\n", nrAttr);

#endif

    SDL_ShowWindow(state->window);

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glViewport(0, 0, state->render_data.width, state->render_data.height);

    return true;
}

void engine_update(EngineState* state, f64 dT) {
    state->update_data.currentTicks = dT;
    state->update_data.ticksPassed += dT;
}

void engine_render(EngineState* state) {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(state->window);
}

void engineState_destroy(EngineState* state) {
    SDL_DestroyWindow(state->window);
    SDL_GL_DestroyContext(state->gl_context);
    SDL_Quit();
}