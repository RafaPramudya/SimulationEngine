#version 440 core

out vec4 FragColor;

in vec2 mTexCoord;

uniform sampler2D uTexture;
uniform vec3 lightCol;

void ambientShader(out vec3 ambient) {
    float ambientStrength = 0.1;
    ambient = ambientStrength * lightCol;
}

void main() {
    vec4 objColor = texture(uTexture, mTexCoord);
    vec3 ambient;
    ambientShader(ambient);
    vec4 result = vec4(ambient, 1.0) * objColor;
    FragColor = result;
}