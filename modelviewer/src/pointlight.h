#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "glm/ext.hpp"

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color = glm::vec3({ 1.0f });

    float Angle = 0.0f;          // degrees
    float Radius = 200.0f;
    float Height = 0.0f;
    float AngularSpeed = 45.0f;  // degrees per second
    int LightRadius = 5;  // degrees per second
};

#endif // !POINTLIGHT_H_
