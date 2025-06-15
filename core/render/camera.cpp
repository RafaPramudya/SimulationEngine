#include "camera.h"
#include "windows/event.h"
#include "windows/appstate.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Camera* camera = nullptr;

void Camera::update() {
    const f32 baseSpeed = event->isKeyDown(VK_CONTROL) ? 5.0f : 2.5f;
    const f32 velocity = baseSpeed * state->deltaTime;

    if (event->isKeyDown('W')) {
        position += front * velocity;
    }
    if (event->isKeyDown('S')) {
        position -= front * velocity;
    }
    if (event->isKeyDown('A')) {
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    }
    if (event->isKeyDown('D')) {
        position += glm::normalize(glm::cross(front, up)) * velocity;
    }
    if (event->isKeyDown(VK_SPACE)) {
        // vec3 posChanged;
        // glm_vec3_scale(camera.up, cameraSpeed * state.deltaTime, posChanged);
        // glm_vec3_add(camera.position, posChanged, camera.position);
        position += up * velocity;
    }
    if (event->isKeyDown(VK_SHIFT)) {
        position -= up * velocity;
    }
}

void Camera::mouseEvent() {
    static const float sensitivity = 0.1f;
    float xoffset = (float) event->mouseState.deltaX * sensitivity;
    float yoffset = (float) event->mouseState.deltaY * sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction(
        cosf(glm::radians(yaw)) * cosf(glm::radians(pitch)),
        sinf(glm::radians(pitch)),
        sinf(glm::radians(yaw)) * cosf(glm::radians(pitch))
    );

    front = glm::normalize(direction);
}

glm::mat4 Camera::getView() {
    return glm::lookAt(position, position + front, up);
}
