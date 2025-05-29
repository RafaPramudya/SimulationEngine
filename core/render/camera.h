#ifndef CAMERA_H
#define CAMERA_H

#include "utils/types.h"
#include "cglm/cglm.h"

typedef struct Camera
{
    vec3 position;
    vec3 front;
    vec3 up;

    f32 yaw, pitch, roll;
} Camera;

void CameraInit(void);
void CameraEvent(void);
void CameraMouseEvent(void);
void getCameraView(mat4* view_out);

#endif // CAMERA_H