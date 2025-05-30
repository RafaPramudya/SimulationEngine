#version 440 core

out vec4 FragColor;

in vec2 mTexCoord;
in vec3 mNormal;
in vec3 mFragPos;

uniform sampler2D uTexture;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

void ambientShader(out vec3 ambient) {
    float ambientStrength = 0.1;
    ambient = ambientStrength * lightCol;
}

void diffuseShader(out vec3 diffuse) {
    vec3 norm = normalize(mNormal);
    vec3 lightDir = normalize(lightPos - mFragPos);
    float diffusion = max(dot(norm, lightDir), 0.0);
    diffuse = diffusion * lightCol;
}

void specularShader(out vec3 specular) {
    float specularStrength = 0.5;
    vec3 norm = normalize(mNormal);
    vec3 lightDir = normalize(lightPos - mFragPos);

    vec3 viewDir = normalize(viewPos - mFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularization = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    specular = specularization * specularStrength * lightCol;
}

void main() {
    vec4 objColor = texture(uTexture, mTexCoord);
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    ambientShader(ambient);
    diffuseShader(diffuse);
    specularShader(specular);

    vec3 combinedLight = ambient + diffuse + specular;
    vec4 result = vec4(combinedLight, 1.0) * objColor;
    FragColor = result;
}