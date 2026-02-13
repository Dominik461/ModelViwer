#ifndef SHADERDEFINITIONSUN_H_
#define SHADERDEFINITIONSUN_H_

#include <string>

const std::string SUN_VERTEX_SHADER_SRC = R"(
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

const std::string SUN_FRAGMENT_SHADER_SRC = R"(
    #version 460 core
    in vec3 vertexColor;
    in vec2 vertextCoords;
    in vec3 vertexPos;
    in vec3 vertexNormal;

    out vec4 FragColor;

    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

#endif