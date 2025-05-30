#version 440 core

out vec4 FragColor;

in vec2 mTexCoord;
in vec3 mNormal;
in vec3 mFragPos;

uniform sampler2D uTexture;
uniform vec3 lightCol;
uniform vec3 lightPos;

void ambientShader(out vec3 ambient) {
    float ambientStrength = 0.1;
    ambient = ambientStrength * lightCol;
}

void diffuseShader(out vec3 diffuse) {
    vec3 norm = normalize(mNormal);
    vec3 lightDir = normalize(lightPos - mFragPos);
    float diffusion = max(dot(norm, lightDir), 0.0);
    diffuse = diffusion * lightCol;
};

void main() {
    vec4 objColor = texture(uTexture, mTexCoord);
    vec3 ambient;
    vec3 diffuse;

    ambientShader(ambient);
    diffuseShader(diffuse);

    vec3 combinedLight = ambient + diffuse;
    vec4 result = vec4(combinedLight, 1.0) * objColor;
    FragColor = result;
}