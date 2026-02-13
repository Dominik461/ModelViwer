#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const Frustrum& frustrum, const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& upVector)
{
	CameraFrustrum = frustrum;
	Position = position;
	LookAt = lookAt;
	UpVector = upVector;
	RecalculateMatrix();
}

void PerspectiveCamera::RecalculateMatrix()
{
    float aspectRatio = CameraFrustrum.width / CameraFrustrum.height;

    // Convert yaw to radians
    float yawRad = glm::radians(Yaw);

    // Calculate orbit position
    Position.x = Distance * sin(yawRad);
    Position.z = Distance * cos(yawRad);
    Position.y = Height;

    ProjectionMatrix = glm::perspective(
        glm::radians(CameraFrustrum.angle),
        aspectRatio,
        CameraFrustrum.near,
        CameraFrustrum.far
    );

    ViewMatrix = glm::lookAt(Position, LookAt, UpVector);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}