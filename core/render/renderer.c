#include "renderer.h"
#include "camera.h"
#include "experiment.h"
#include "windows/appstate.h"
#include "windows/event.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include "cglm/cglm.h"

#include <math.h>

// Global variable for renderstate
RenderState renderstate;
extern AppState appstate;
extern Event event;

void PrepareRenderer(void) {
    compileProgram("assets/shader/basic.vert", "assets/shader/basic.frag", &renderstate.main_shader);
    compileProgram("assets/shader/light.vert", "assets/shader/light.frag", &renderstate.light_shader);

    renderstate.ttsTexture = createTexture("assets/images/orang_jelek.jpg");

    createGLObject(&renderstate.basic, quadVerts, sizeof(quadVerts), quadInds, sizeof(quadInds));
    startsVertsAttribs();
    addVertsAttrib(3);
    addVertsAttrib(2);
    addVertsAttrib(3);
    compileVertsAttrib();

    createGLObject(&renderstate.light, quadLightVerts, sizeof(quadLightVerts), quadInds, sizeof(quadInds));
    startsVertsAttribs();
    addVertsAttrib(3);
    compileVertsAttrib();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(renderstate.main_shader.pId);

    glViewport(0, 0, appstate.width, appstate.height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    ShowCursor(FALSE);
    CameraInit();
}

void RenderEvent(void) {
    static bool polygonFill = true;
    static bool usingMouse = true;

    if (IS_KEY_PRESSED('P')) {
        if (polygonFill) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); polygonFill = !polygonFill; }
        else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); polygonFill = !polygonFill; }
    }

    if (IS_KEY_PRESSED(VK_F5)) {
        usingMouse = !usingMouse;
        CaptureMouse(usingMouse);
    }
    CameraEvent();
}

void RenderLoop(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    // glClearColor(0.05f, 0.07f, 0.15f, 1.0f); // Dark night sky color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    vec3 lightColor = {1.0f, 0.95f, 0.8f}; // sunlight color (warm white)
    vec3 lightPos = {2.0f, 1.0f, 1.5f};

    mat4 view;
    mat4 projection;
    getCameraView(&view);
    glm_perspective(glm_rad(50.0f), (float)appstate.width / (float)appstate.height, 0.1f, 100.0f, projection);

    Shader* mainShader = &renderstate.main_shader;
    Shader* lightShader = &renderstate.light_shader;
    glUseProgram(mainShader->pId);

    mat4 model;
    glm_mat4_identity(model);
    glm_rotate(model, 75.0f * glm_rad(appstate.passedTime), (vec3){1.0f, 0.0f, 0.0f});

    glUniformMatrix4fv(glGetUniformLocation(mainShader->pId, "model"), 1, GL_FALSE, model[0]);
    glUniformMatrix4fv(glGetUniformLocation(mainShader->pId, "view"), 1, GL_FALSE, view[0]);
    glUniformMatrix4fv(glGetUniformLocation(mainShader->pId, "projection"), 1, GL_FALSE, projection[0]);
    glUniform3fv(glGetUniformLocation(mainShader->pId, "lightCol"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(mainShader->pId, "lightPos"), 1, lightPos);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, renderstate.ttsTexture.id);

    glBindVertexArray(renderstate.basic.VAO);
    glDrawElements(GL_TRIANGLES, sizeof(quadInds) / sizeof(*quadInds), GL_UNSIGNED_INT, 0);

    glUseProgram(lightShader->pId);

    mat4 lightModel;
    glm_mat4_identity(lightModel);
    glm_translate(lightModel, lightPos);
    glm_scale(lightModel, (vec3){0.5, 0.5, 0.5});

    glUniformMatrix4fv(glGetUniformLocation(lightShader->pId, "model"), 1, GL_FALSE, lightModel[0]);
    glUniformMatrix4fv(glGetUniformLocation(lightShader->pId, "view"), 1, GL_FALSE, view[0]);
    glUniformMatrix4fv(glGetUniformLocation(lightShader->pId, "projection"), 1, GL_FALSE, projection[0]);
    glUniform3fv(glGetUniformLocation(lightShader->pId, "lightCol"), 1, lightColor);

    glBindVertexArray(renderstate.light.VAO);
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