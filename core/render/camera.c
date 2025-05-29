#include "camera.h"
#include "windows/event.h"
#include "windows/appstate.h"

#include <math.h>

Camera camera;
extern AppState appstate;
extern Event event;

void CameraInit(void) {
    glm_vec3((vec3){0.0f, 0.0f, 3.0f}, camera.position);
    glm_vec3((vec3){0.0f, 0.0f, -1.0f}, camera.front);
    glm_vec3((vec3){0.0f, 1.0f, 0.0f}, camera.up);

    camera.yaw = 0.0f;
    camera.pitch = 0.0f;
    camera.roll = 0.0f;
}

void CameraEvent(void) {
    static const float defaultCameraSpeed = 1.5f;
    static float cameraSpeed;
    if (IS_KEY_DOWN(VK_CONTROL)) {
        cameraSpeed = defaultCameraSpeed * 2;
    } else {
        cameraSpeed = defaultCameraSpeed;
    }

    if (IS_KEY_DOWN('W')) {
        vec3 posChanged;
        glm_vec3_scale(camera.front, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_add(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN('S')) {
        vec3 posChanged;
        glm_vec3_scale(camera.front, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_sub(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN('A')) {
        vec3 posChanged;
        glm_vec3_cross(camera.front, camera.up, posChanged);
        glm_vec3_normalize(posChanged);
        glm_vec3_scale(posChanged, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_sub(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN('D')) {
        vec3 posChanged;
        glm_vec3_cross(camera.front, camera.up, posChanged);
        glm_vec3_normalize(posChanged);
        glm_vec3_scale(posChanged, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_add(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN(VK_SPACE)) {
        vec3 posChanged;
        glm_vec3_scale(camera.up, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_add(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN(VK_SHIFT)) {
        vec3 posChanged;
        glm_vec3_scale(camera.up, cameraSpeed * appstate.deltaTime, posChanged);
        glm_vec3_sub(camera.position, posChanged, camera.position);
    }
}

void CameraMouseEvent(void) {
    static const float sensitivity = 0.1f;
    float xoffset = (float) event.mouseState.deltaX * sensitivity;
    float yoffset = (float) event.mouseState.deltaY * sensitivity;

    camera.yaw += xoffset;
    camera.pitch -= yoffset;

    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
    if (camera.pitch < -89.0f) camera.pitch = -89.0f;

    vec3 direction;
    direction[0] = cosf(glm_rad(camera.yaw)) * cosf(glm_rad(camera.pitch));
    direction[1] = sinf(glm_rad(camera.pitch));
    direction[2] = sinf(glm_rad(camera.yaw)) * cosf(glm_rad(camera.pitch));

    glm_normalize(direction);
    memcpy(camera.front, direction, sizeof(vec3));
}

void getCameraView(mat4* view_out) {
    vec3 center;
    glm_vec3_add(camera.position, camera.front, center);
    glm_lookat(camera.position, center, camera.up, *view_out);
}
