#include "context.h"
#include "appstate.h"
#include "utils/types.h"


#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN
#include <Windows.h>
#include <wchar.h>
#include <assert.h>
#include <stdio.h>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

extern AppState appstate;

void context_init(void) {
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;

    i32 pixelFormat = ChoosePixelFormat(appstate.hdc, &pfd);
    SetPixelFormat(appstate.hdc, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(appstate.hdc);
    wglMakeCurrent(appstate.hdc, tempRC);

    assert(gladLoadGL());
    assert(gladLoadWGL(appstate.hdc));

    const i32 attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 4,
        WGL_CONTEXT_FLAGS_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };

    HGLRC hglrc = wglCreateContextAttribsARB(appstate.hdc, 0, attribList);
    appstate.hglrc = hglrc;
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(appstate.hdc, appstate.hglrc);

    #ifdef DEBUG
    printf("OpenGL Berhasil diload\n");
    printf("Versi: %s\n", glGetString(GL_VERSION));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    #endif

    bool swapControlSupported = strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;
    i32 vsync = 0;

    if (swapControlSupported) {
        #ifdef VSYNC
        if (wglSwapIntervalEXT(1)) {
            #ifdef DEBUG
            printf("VSync diaktifkan\n");
            #endif
        }
        #endif
    } else {
        #ifdef DEBUG
        printf("Aplikasi tidak support VSync");
        #endif
    }
}

void context_destroy(void) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(appstate.hglrc);
    ReleaseDC(appstate.hwnd, appstate.hdc);
}