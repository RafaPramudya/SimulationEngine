#version 440 core

out vec4 FragColor;

in vec3 mCol;

void main() {
    FragColor = vec4(mCol, 1.0f);
}