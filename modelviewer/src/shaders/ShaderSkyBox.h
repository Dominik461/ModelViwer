#ifndef SHADERDEFINITIONSKYBOX_H_
#define SHADERDEFINITIONSKYBOX_H_

#include <string>

const std::string SKYBOX_VERTEX_SHADER_SRC = R"(
    #version 460 core
    layout (location = 0) in vec3 position;

    out vec3 TexCoords;

    uniform mat4 u_ViewProjection;
    uniform mat4 u_Model;

    void main() {
        vec4 worldPos = u_Model * vec4(position, 1.0);
        TexCoords = position;
        gl_Position = u_ViewProjection* worldPos;
    }
)";

const std::string SKYBOXL_FRAGMENT_SHADER_SRC = R"(
    #version 460 core
    out vec4 FragColor;

    in vec3 TexCoords;

    layout(binding=0) uniform samplerCube skybox;

    void main() {
        FragColor = texture(skybox, TexCoords);
    }
)";

#endif