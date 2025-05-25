#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "utils/types.h"

static float quadVerts[] = {
    // positions      // colors       // tex coords
    -0.5, -0.5, 0,  1.0, 0.0, 0.0,         0.0, 0.0, // bottom left
     0.5, -0.5, 0,  0.0, 1.0, 0.0,         1.0, 0.0, // bottom right
     0.5,  0.5, 0,  0.0, 1.0, 1.0,         1.0, 1.0, // top right
    -0.5,  0.5, 0,  0.0, 0.0, 1.0,         0.0, 1.0  // top left
};
static u32 quadInds[] = {
    0, 1, 2,
    0, 2, 3
};

#endif