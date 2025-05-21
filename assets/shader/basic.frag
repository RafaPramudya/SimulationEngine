#version 440 core

out vec4 FragColor;

in vec3 mCol;

uniform float uOpac;

void main() {
    vec3 opacedColor = mCol * uOpac;
    FragColor = vec4(opacedColor, 1.0f);
}