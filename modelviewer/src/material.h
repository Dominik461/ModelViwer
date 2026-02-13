#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Material
{
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
    glm::vec3 ambient = glm::vec3(0.1f);
    float shininess = 32.0f;

    GLuint diffuseTex = 0;   // OpenGL texture ID
    bool hasTexture = false;
};

#endif
