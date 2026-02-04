#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const Frustrum& frustrum,
    const glm::vec3& position, float rotation)
    : CameraFrustrum(frustrum), Rotation(rotation)
{
    this->Position = position;
    RecalculateMatrix();
}

void OrthographicCamera::RecalculateMatrix()
{
    // Create the projection matrix
    this->ProjectionMatrix = glm::ortho(
        CameraFrustrum.left,
        CameraFrustrum.right,
        CameraFrustrum.bottom,
        CameraFrustrum.top,
        CameraFrustrum.near,
        CameraFrustrum.far
    );

    // Create the view matrix
    // Start with identity
    glm::mat4 transform = glm::mat4(1.0f);

    // Apply translation
    transform = glm::translate(transform, this->Position);

    // Apply rotation around Z-axis
    transform = glm::rotate(transform, this->Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

    // View matrix is the inverse of the transform
    this->ViewMatrix = glm::inverse(transform);

    // Combine them
    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
}