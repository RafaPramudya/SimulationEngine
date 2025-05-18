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

extern AppState appstate_ins;

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

    i32 pixelFormat = ChoosePixelFormat(appstate_ins.hdc, &pfd);
    SetPixelFormat(appstate_ins.hdc, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(appstate_ins.hdc);
    wglMakeCurrent(appstate_ins.hdc, tempRC);

    assert(gladLoadGL());
    assert(gladLoadWGL(appstate_ins.hdc));

    const i32 attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 4,
        WGL_CONTEXT_FLAGS_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };

    HGLRC hglrc = wglCreateContextAttribsARB(appstate_ins.hdc, 0, attribList);
    appstate_ins.hglrc = hglrc;
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(appstate_ins.hdc, appstate_ins.hglrc);


    #ifdef DEBUG
    char buffer[128];
    snprintf(buffer, 128, "OpenGL Sukses diLoad\nVersion: %d.%d", GLVersion.major, GLVersion.minor);
    MessageBox(0, buffer, "Informasi", MB_OK);
    #endif

    bool swapControlSupported = strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;
    i32 vsync = 0;

    if (swapControlSupported) {
        #ifdef VSYNC
        if (wglSwapIntervalEXT(1)) {
            #ifdef DEBUG
            MessageBox(appstate_ins.hdc, L"VSync diaktifkan", L"Informasi", MB_OK);
            #endif
        }
        #endif
    } else {
        #ifdef DEBUG
        MessageBox(appstate_ins.hdc, L"Tidak Support VSync", L"Informasi", MB_OK);
        #endif
    }
}

void context_destroy(void) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(appstate_ins.hglrc);
    ReleaseDC(appstate_ins.hwnd, appstate_ins.hdc);
}