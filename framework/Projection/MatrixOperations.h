#ifndef MATRIXOPERATIONS_H_
#define MATRIXOPERATIONS_H_

#include <glm/ext.hpp>

namespace MatrixOperations
{
	const glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);

	inline glm::mat4 setPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	}

	inline glm::mat4 getIdentityMatrix() { return  IDENTITY_MAT; };
	inline glm::mat4 getViewMatrix(glm::vec3 pos, glm::vec3 ori, glm::vec3 up) { return glm::lookAt(pos, ori, up); }
	inline glm::mat4 getTransformedMatrix(glm::vec3 scale = glm::vec3{1.0f}, glm::vec3 rotationVec = glm::vec3{ 1.0f, 0.0f, 0.0f }, float rotationDeg = 0.0f, glm::vec3 translate = glm::vec3{ 0.0f }, glm::mat4 matrix = IDENTITY_MAT)
	{
		auto scaleMat = glm::scale(matrix, scale);
		auto rotationMat = glm::rotate(matrix, glm::radians(rotationDeg), rotationVec);
		auto translateMat = glm::translate(matrix, translate);

		return translateMat * rotationMat * scaleMat;
	}

};



#endif // !MATRIXOPERATIONS_H_
