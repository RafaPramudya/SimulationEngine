#version 440 core

struct Light {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 32

out vec4 FragColor;

in vec2 mTexCoord;
in vec3 mNormal;
in vec3 mFragPos;

uniform sampler2D diffuse0;
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

void main() {
    vec4 objColor = texture(diffuse0, mTexCoord);
    vec3 norm = normalize(mNormal);
    vec3 viewDir = normalize(viewPos - mFragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (
            lights[i].constant == 0.0
            ) break;

        float distance    = length(lights[i].position - mFragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + 
    		    lights[i].quadratic * (distance * distance));  

        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lights[i].color;

        vec3 lightDir = normalize(lights[i].position - mFragPos);
        float diffusion = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffusion * lights[i].color;

        float specularStrength = 0.5;
        vec3 reflectDir = reflect(-lightDir, norm);

        float specularization = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = specularization * specularStrength * lights[i].color;

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        result += ambient + diffuse + specular;
        // result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0) * objColor;
}