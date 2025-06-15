#ifndef CAMERA_H
#define CAMERA_H

#include "utils/types.h"
#include <glm/glm.hpp>

class Camera {
public:
    Camera() : 
        position(0.0f, 0.0f, 3.0f),
        front(0.0f, 0.0f, -1.0f),
        yaw(-90.0f), pitch(0.0f), roll(0.0f)
    {};
    ~Camera() = default;

    void update();
    void mouseEvent();

    glm::mat4 getView();

    glm::vec3& getPos() { return position; }
    glm::vec3& getFront() { return front; }
    glm::vec3& getUp() { return up; }
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    f32 yaw, pitch, roll;
};

extern Camera* camera;

#endif // CAMERA_H