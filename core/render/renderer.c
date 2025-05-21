#include "renderer.h"
#include "experiment.h"
#include "windows/appstate.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

// Global variable for renderstate
RenderState renderstate;
extern AppState appstate;

void PrepareRenderer(void) {
    compileProgram("assets/shader/vertex.glsl", "assets/shader/fragment.glsl", &renderstate.main_shader);

    glGenVertexArrays(1, &renderstate.VAO);
    glBindVertexArray(renderstate.VAO);

    glGenBuffers(1, &renderstate.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderstate.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Lokasi Layout 0 vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(renderstate.main_shader.pId);

    glViewport(0, 0, appstate.width, appstate.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderLoop(void) {
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    glClearColor(0.05f, 0.07f, 0.15f, 1.0f); // Dark night sky color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(renderstate.main_shader.pId);
    glBindVertexArray(renderstate.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SwapBuffers(appstate.hdc);
}

void RendererResize(u32 width, u32 height) {
    appstate.width = width;
    appstate.height = height;
    glViewport(0, 0, width, height);
}