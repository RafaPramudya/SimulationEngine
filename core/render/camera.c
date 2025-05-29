#include "camera.h"
#include "windows/event.h"
#include "windows/appstate.h"

Camera camera;
extern AppState appstate;
extern Event event;

void CameraInit(void) {
    glm_vec3((vec3){0.0f, 0.0f, 3.0f}, camera.position);
    glm_vec3((vec3){0.0f, 0.0f, -1.0f}, camera.front);
    glm_vec3((vec3){0.0f, 1.0f, 0.0f}, camera.up);
}

void CameraEvent(void) {
    if (IS_KEY_DOWN(event.keyStates,'W')) {
        vec3 posChanged;
        glm_vec3_scale(camera.front, appstate.deltaTime, posChanged);
        glm_vec3_add(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN(event.keyStates,'S')) {
        vec3 posChanged;
        glm_vec3_scale(camera.front, appstate.deltaTime, posChanged);
        glm_vec3_sub(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN(event.keyStates,'A')) {
        vec3 posChanged;
        glm_vec3_cross(camera.front, camera.up, posChanged);
        glm_vec3_normalize(posChanged);
        glm_vec3_scale(posChanged, appstate.deltaTime, posChanged);
        glm_vec3_sub(camera.position, posChanged, camera.position);
    }
    if (IS_KEY_DOWN(event.keyStates,'D')) {
        vec3 posChanged;
        glm_vec3_cross(camera.front, camera.up, posChanged);
        glm_vec3_normalize(posChanged);
        glm_vec3_scale(posChanged, appstate.deltaTime, posChanged);
        glm_vec3_add(camera.position, posChanged, camera.position);
    }
}

void getCameraView(mat4* view_out) {
    vec3 center;
    glm_vec3_add(camera.position, camera.front, center);
    glm_lookat(camera.position, center, camera.up, *view_out);
}
