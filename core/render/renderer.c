#include "renderer.h"
#include "experiment.h"
#include "windows/appstate.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include "cglm/cglm.h"

#include <math.h>

// Global variable for renderstate
RenderState renderstate;
extern AppState appstate;

void PrepareRenderer(void) {
    compileProgram("assets/shader/basic.vert", "assets/shader/basic.frag", &renderstate.main_shader);

    renderstate.ttsTexture = createTexture("assets/images/kelinci.png");

    glGenVertexArrays(1, &renderstate.VAO);
    glBindVertexArray(renderstate.VAO);

    glGenBuffers(1, &renderstate.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderstate.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glGenBuffers(1, &renderstate.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderstate.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadInds), quadInds, GL_STATIC_DRAW);

    // Lokasi Layout 0 vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    // Lokasi Layout 1 vec3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Lokasi Layout 2 vec2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(renderstate.main_shader.pId);

    glViewport(0, 0, appstate.width, appstate.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderLoop(void) {
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    glClearColor(0.05f, 0.07f, 0.15f, 1.0f); // Dark night sky color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Shader* mainShader = &renderstate.main_shader;
    glUseProgram(mainShader->pId);

    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, (vec3){0.3, 0.2, 0.0});
    glm_rotate(transform, 50.0f * glm_rad(appstate.passedTime), (vec3){0.0, 0.0, 1.0});

    glUniformMatrix4fv(glGetUniformLocation(mainShader->pId, "transform"), 1, GL_FALSE, transform[0]);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, renderstate.ttsTexture.id);

    glBindVertexArray(renderstate.VAO);
    glDrawElements(GL_TRIANGLES, sizeof(quadInds) / sizeof(*quadInds), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    SwapBuffers(appstate.hdc);
}

void RendererResize(u32 width, u32 height) {
    appstate.width = width;
    appstate.height = height;
    glViewport(0, 0, width, height);
}

void QuitRenderer(void) {
    freeShader(renderstate.main_shader);
    freeTexture(renderstate.ttsTexture);
}