#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "utils/types.h"

static const char* vertexSource = 
"#version 440 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"}\n";

static const char* fragmentSource =
"#version 440 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

static f32 vertices[] = {
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f
};

#endif