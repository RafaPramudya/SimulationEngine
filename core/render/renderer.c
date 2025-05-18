#include "renderer.h"
#include "windows/appstate.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

extern AppState appstate_ins;

void PrepareRenderer(void) {
    glViewport(0, 0, appstate_ins.width, appstate_ins.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderLoop(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SwapBuffers(appstate_ins.hdc);
}

