#include "engine.h"

#include "constants.h"

bool engine_init(Engine* engine, const char* title) {
    engine->window = NULL;
    engine->ticksPassed = 0;
    engine->isRunning = false;
    engine->warna_lampu = MERAH;
    engine->width = 0;
    engine->height = 0;
    engine->refreshRate = 0.0f;

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
    engine->width = dm->w;
    engine->height = dm->h;
    engine->refreshRate = dm->refresh_rate;

    uint32_t window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
    engine->window = SDL_CreateWindow(title, engine->width, engine->height, window_flags);
    if (engine->window == NULL) {
        SDL_ERR_LOG("SDL_Window tidak bisa dibuat");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGL_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    engine->context = SDL_GL_CreateContext(engine->window);
    if (engine->context == NULL) {
        SDL_ERR_LOG("SDL_Context tidak bisa dibuat");
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();

    if (glewErr != GLEW_OK) {
        ERR_LOG("GLEW Inisiasi");
        return false;
    }

#ifdef DEBUG

    int nrAttr;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr);

    printf("Versi OpenGL : %s\n", glGetString(GL_VERSION));
    printf("Platform OpenGL : %s, Renderer OpenGL: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("Maksimal Vertex Attribut : %d\n", nrAttr);

#endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0,0, engine->width, engine->height);

    engine->isRunning = true;
    SDL_ShowWindow(engine->window);

    return true;
}

void engine_event(Engine* engine) {
    while (SDL_PollEvent(&engine->event)) {
        switch (engine->event.type)
        {
        case SDL_EVENT_QUIT:
            engine->isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (engine->event.key.key) {
                case SDLK_M:
                    engine->warna_lampu = MERAH;
                    break;
                case SDLK_K:
                    engine->warna_lampu = KUNING;
                    break;
                case SDLK_H:
                    engine->warna_lampu = HIJAU;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        }
    }
}

void engine_update(Engine* engine, f64 dT) {
    engine->currentTicks = dT;
    engine->ticksPassed += dT;

    #ifdef LOG_FPS
    printf("FPS : %lf\n", 1 / dT);
    #endif
}

void engine_render(Engine* engine) {
    // printf("%f\n", color);
    vec3 color;
    switch (engine->warna_lampu)
    {
    case MERAH:
        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 0.0;
        break;
    case KUNING:
        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 0.0;
        break;
    case HIJAU:
        color[0] = 0.0;
        color[1] = 1.0;
        color[2] = 0.0;
        break;
    default:
        break;
    }

    glClearColor(
        color[0], color[1], color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(engine->window);
}

void engine_deinit(Engine* engine) {
    SDL_DestroyWindow(engine->window);
    SDL_GL_DestroyContext(engine->context);
    SDL_Quit();
}