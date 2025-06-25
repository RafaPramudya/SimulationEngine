#ifndef SHADER_H
#define SHADER_H

#include "utils/types.h"
#include "light.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;
class Shader {
public:
    Shader(GLenum type, const char* filename);
    ~Shader();

    friend class ShaderProg;
private:
    u32 id;
};

class ShaderProg {
public:
    ShaderProg();
    ~ShaderProg();

    void use();

    void setUniform(const char* location, f32 value);
    void setUniform(const char* location, glm::mat4& matrix);
    void setUniform(const char* location, glm::vec3& vector);
    void setUniform(Light& light);

    void attachShader(Shader& shader);
    void linkProgram();
private:
    u32 pId;
};

#endif // SHADER_H