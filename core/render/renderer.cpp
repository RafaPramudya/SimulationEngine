#include "renderer.h"
#include "camera.h"
#include "experiment.h"
#include "windows/appstate.h"
#include "windows/event.h"
#include "component/transform.h"
#include "component/render.h"
#include "component/light.h"

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Global variable for renderstate
Renderer* renderer = nullptr;

auto& planet (eManager.addEntity());
auto& wakakak (eManager.addEntity());
auto& matahari (eManager.addEntity());

Renderer::Renderer() {

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
    // basic.emplace("assets/model/wop.gltf");
    planet.addComponent<Transform>();
    planet.addComponent<Render>("assets/model/wop.gltf", &main_prog);

    // light.emplace("light", quadVerts, sizeof(quadVerts), quadInds, sizeof(quadInds));
    // light->setLight(1.0, 0.09, 0.032, glm::vec3(1.0f, 0.95f, 0.8f));
    auto& lightTransform = wakakak.addComponent<Transform>();
    wakakak.addComponent<Render>(quadVerts, sizeof(quadVerts), quadInds, sizeof(quadInds), &light_prog);
    auto& lightComponent = wakakak.addComponent<Light>();    
    // glm::vec3 lightColor(1.0f, 0.95f, 0.8f); // sunlight color (warm white)
    lightComponent.setLight(1.0, 0.007, 0.0002, glm::vec3(1.0f));
    lightTransform.translate(glm::vec3(2.0f, 1.0f, 1.5f));
    lightTransform.scale(glm::vec3(0.5, 1.0, 0.25));

    auto& matahariTransform = matahari.addComponent<Transform>();
    matahari.addComponent<Render>("assets/model/wop.gltf", &light_prog);
    auto& matahariComponent = matahari.addComponent<Light>();

    matahariComponent.setLight(1.0, 0.007, 0.0002, glm::vec3(1.0f, 0.95f, 0.8f));
    matahariTransform.translate(glm::vec3(-4.0f, 1.0f, -3.0f));
    matahariTransform.scale(glm::vec3(0.5f));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    main_prog.use();

    glViewport(0, 0, state->getWidth(), state->getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    ShowCursor(FALSE);
}

void Renderer::renderEventUpdate() {
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
    
    planet.getComponent<Transform>().translate(glm::vec3(0.0f, 0.25f, 0.1f) * (f32)state->deltaTime);
    planet.getComponent<Transform>().rotate(90 * state->deltaTime, glm::vec3(0, 1, 0));
    camera->update();
    eManager.update();
}

void Renderer::render() {
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    glClearColor(0.05f, 0.07f, 0.15f, 1.0f); // Dark night sky color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    eManager.render();
    eManager.refresh();

    SwapBuffers(state->getHDC());
}

void Renderer::resize(u32 width, u32 height) {
    glViewport(0,0,width,height);
}