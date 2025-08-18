#version 440 core

out vec4 FragColor;
in vec3 mNormal;

uniform vec3 color;
uniform vec3 camFront;

void main() {
    if (dot(camFront, mNormal) > 0.0) {
        discard;
    }
    FragColor = vec4(color, 1.0);
}