#ifndef SHADERDEFINITIONMODEL_H_
#define SHADERDEFINITIONMODEL_H_

#include <string>

const std::string MODEL_VERTEX_SHADER_SRC = R"(
    #version 460 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 tcoords;
    layout(location = 3) in vec3 color;
    
    uniform mat4 u_ViewProjection;
    uniform mat4 u_Model;

    out vec3 vertexColor;
    out vec2 vertextCoords;
    out vec3 vertexNormal;
    out vec3 vertexPos;

    void main() {
        vec4 worldPos = u_Model * vec4(position, 1.0);
        gl_Position = u_ViewProjection * worldPos;
        vertexColor = color;
        vertextCoords = tcoords;
        vertexNormal = mat3(transpose(inverse(u_Model))) * normal;
        vertexPos = worldPos.xyz;
    }
)";

const std::string MODEL_FRAGMENT_SHADER_SRC = R"(
    #version 460 core
    in vec3 vertexColor;
    in vec2 vertextCoords;
    in vec3 vertexPos;
    in vec3 vertexNormal;

    uniform vec3 u_LightPos;
    uniform vec3 u_LightColor;
    uniform float u_AmbientStrength;
    uniform float u_SpecularStrength;
    uniform vec3 u_ViewPosition;
    uniform float u_LightMix;
    uniform float u_UseCelShading;
    uniform float u_UsePixelShading;

    uniform sampler2D u_DiffuseTex;
    uniform bool u_HasTexture;

    uniform vec3 u_MaterialDiffuse;
    uniform vec3 u_MaterialSpecular;
    uniform float u_MaterialShininess;

    out vec4 FragColor;

    void main() {
        vec3 baseColor;
        vec3 finalColor;
        vec3 lightColor = u_LightColor;

        if (u_HasTexture)
            baseColor = texture(u_DiffuseTex, vertextCoords).rgb;
        else
            baseColor = u_MaterialDiffuse; // fallback to MTL diffuse

        if (u_UseCelShading > 0.5)
        {
        }
        else if (u_UsePixelShading > 0.5)
        {
        }
        else
        {
            vec3 ambient = u_AmbientStrength * lightColor;
            vec3 norm = normalize(vertexNormal);
            vec3 lightDir = normalize(u_LightPos - vertexPos);

            float cosAngle = max(dot(norm, lightDir), 0.0f);
            vec3 diffuse = cosAngle * lightColor;

            vec3 viewDir = normalize(u_ViewPosition - vertexPos);
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_MaterialShininess);
            vec3 specular = u_SpecularStrength * spec * u_MaterialSpecular;

            finalColor = (ambient + diffuse + specular) * baseColor;
        }


        FragColor = vec4(finalColor, 1.0);
    }
)";

#endif