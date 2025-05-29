#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/cglm.h"

typedef struct Camera
{
    vec3 position;
    vec3 front;
    vec3 up;
} Camera;

void CameraInit(void);
void CameraEvent(void);
void getCameraView(mat4* view_out);

#endif // CAMERA_H