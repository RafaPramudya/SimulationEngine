#include "renderer.h"
#include "camera.h"
#include "experiment.h"
#include "windows/appstate.h"
#include "windows/event.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Global variable for renderstate
Renderer* renderer = nullptr;

Renderer::Renderer() {
    // compileProgram("assets/shader/basic.vert", "assets/shader/basic.frag", &renderstate.main_shader);
    // compileProgram("assets/shader/light.vert", "assets/shader/light.frag", &renderstate.light_shader);

    Shader basic_vrt(GL_VERTEX_SHADER, "assets/shader/basic.vert");
    Shader basic_frg(GL_FRAGMENT_SHADER, "assets/shader/basic.frag");
    main_prog.attachShader(basic_vrt);
    main_prog.attachShader(basic_frg);
    main_prog.linkProgram();

    Shader light_vrt(GL_VERTEX_SHADER, "assets/shader/light.vert");
    Shader light_frg(GL_FRAGMENT_SHADER, "assets/shader/light.frag");
    light_prog.attachShader(light_vrt);
    light_prog.attachShader(light_frg);
    light_prog.linkProgram();

    // basic.emplace(quadVerts, sizeof(quadVerts), quadInds, sizeof(quadInds));
    // basic->addTextureFirstMesh("assets/images/orang_jelek.jpg", TextureType::DIFFUSE);
    basic.emplace("assets/model/wop.gltf");

    light.emplace("light", quadVerts, sizeof(quadVerts), quadInds, sizeof(quadInds));
    light->setLight(1.0, 0.09, 0.032, glm::vec3(1.0f, 0.95f, 0.8f));
    // vec3 lightColor = {1.0f, 0.95f, 0.8f}; // sunlight color (warm white)
    // vec3 lightPos = {2.0f, 1.0f, 1.5f};
    // glm::vec3 lightColor(1.0f, 0.95f, 0.8f); // sunlight color (warm white)
    light->transform.translate(glm::vec3(2.0f, 1.0f, 1.5f));
    light->transform.scale(glm::vec3(0.5, 1.0, 0.25));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    main_prog.use();

    glViewport(0, 0, state->getWidth(), state->getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    ShowCursor(FALSE);
}

void Renderer::renderEvent() {
    static bool polygonFill = true;
    static bool usingMouse = true;

    if (event->isKeyPressed('P')) {
        if (polygonFill) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); polygonFill = !polygonFill; }
        else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); polygonFill = !polygonFill; }
    }

    if (event->isKeyPressed(VK_F5)) {
        usingMouse = !usingMouse;
        event->captureMouse(usingMouse);
    }
    camera->update();
}

void Renderer::render() {
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    glClearColor(0.05f, 0.07f, 0.15f, 1.0f); // Dark night sky color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 view = camera->getView();
    glm::mat4 projection = glm::perspective(glm::radians(50.0f), (float)state->getWidth() / (float)state->getHeight(), 0.1f, 100.0f);

    basic->transform.rotate(90 * state->deltaTime, glm::vec3(0, 1, 0));

    main_prog.use();
    auto model = basic->transform.getMatrix();
    // auto model = glm::mat4(1.0f);

    main_prog.setUniform("model", model);
    main_prog.setUniform("view", view);
    main_prog.setUniform("projection", projection);
    main_prog.setUniform(light.value());
    main_prog.setUniform("viewPos", camera->getPos());
    
    basic->draw(main_prog.getPId());

    light_prog.use();

    model = light->transform.getMatrix();

    light_prog.setUniform("model", model);
    light_prog.setUniform("view", view);
    light_prog.setUniform("projection", projection);
    light_prog.setUniform("color", light->getColor());

    light->draw(light_prog.getPId());

    SwapBuffers(state->getHDC());
}

void Renderer::resize(u32 width, u32 height) {
    glViewport(0,0,width,height);
}