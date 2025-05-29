#version 440 core

out vec4 FragColor;

in vec3 mCol;
in vec2 mTexCoord;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, mTexCoord);
}