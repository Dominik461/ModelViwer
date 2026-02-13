#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"
#include <iostream>

#include <array>

class PerspectiveCamera : public Camera
{
public:

    struct Frustrum {
        float angle;
        float width;
        float height;
        float near;
        float far;
    };

public:
    PerspectiveCamera(const Frustrum& frustrum = { 45.0f, -1.0f, 1.0f, 1.0f, -1.0f },
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& lookAt = glm::vec3(-1.0f),
        const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f));
    ~PerspectiveCamera() = default;
    PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera)
    {
        this->LookAt = camera.LookAt;
        this->UpVector = camera.UpVector;
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum)
    {
        this->CameraFrustrum = frustrum; this->RecalculateMatrix();
    }

    void SetLookAt(const glm::vec3& lookAt)
    {
        this->LookAt = lookAt; this->RecalculateMatrix();
    }

    void SetUpVector(const glm::vec3& upVector)
    {
        this->UpVector = upVector; this->RecalculateMatrix();
    }

    void SetYaw(float yaw)
    {
        Yaw = yaw;
        RecalculateMatrix();
    }

    void AddYaw(float delta)
    {
        Yaw += delta;
        RecalculateMatrix();
    }

    void SetDistance(float distance)
    {
        Distance = glm::max(distance, 0.1f); // prevent flipping
        RecalculateMatrix();
    }

    void AddDistance(float delta)
    {
        Distance = glm::max(Distance + delta, 0.1f);
        RecalculateMatrix();
    }

    void SetHeight(float height)
    {
        Height = height;
        RecalculateMatrix();
    }

    void AddHeight(float delta)
    {
        Height += delta;
        RecalculateMatrix();
    }
protected:
    void RecalculateMatrix();

protected:
    glm::vec3 LookAt;
    glm::vec3 UpVector;
    Frustrum CameraFrustrum;

private:
    float Yaw = 0.0f;        // rotation around Y axis (degrees)
    float Distance = 10.0f; // radius from origin
    float Height = 0.0f;     // Y position
};

#endif // PERSPECTIVECAMERA_H_
