#version 440 core

struct Light {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 mTexCoord;
in vec3 mNormal;
in vec3 mFragPos;

uniform sampler2D diffuse0;
uniform Light light;
uniform vec3 viewPos;

void ambientShader(out vec3 ambient) {
    float ambientStrength = 0.1;
    ambient = ambientStrength * light.color;
}

void diffuseShader(out vec3 diffuse) {
    vec3 norm = normalize(mNormal);
    vec3 lightDir = normalize(light.position - mFragPos);
    float diffusion = max(dot(norm, lightDir), 0.0);
    diffuse = diffusion * light.color;
}

void specularShader(out vec3 specular) {
    float specularStrength = 0.5;
    vec3 norm = normalize(mNormal);
    vec3 lightDir = normalize(light.position - mFragPos);

    vec3 viewDir = normalize(viewPos - mFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularization = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    specular = specularization * specularStrength * light.color;
}

void main() {
    vec4 objColor = texture(diffuse0, mTexCoord);
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