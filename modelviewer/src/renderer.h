#ifndef RENDERER_H_
#define RENDERER_H_

#include "PerspectiveCamera.h"
#include "RenderCommands.h"
#include "Shader.h"
#include "ShaderDefenitionsModel.h"
#include "ShaderSkyBox.h"
#include "glm/glm.hpp"
#include "drawcall.h"
#include "material.h"
#include "pointlight.h"
#include <iostream>
#include <iomanip>

class Renderer
{
private:
	const glm::vec3 m_LIGHT_COLOR_DAY = glm::vec3{ 0.839f, 0.753f, 0.753f };
	const glm::vec3 m_LIGHT_COLOR_NIGHT = glm::vec3{ 0.123f, 0.04f, 0.136f };
	const float m_AMBIENT_STRENGTH = 0.75;
	const float m_SPECULAR_STRENGTH = 1.0;

	std::shared_ptr<Shader> m_modelShader;
	std::shared_ptr<Shader> m_skyboxShader;

	void setupShaders();

	void renderModel(std::shared_ptr<VertexArray> modelData);
	void renderSkybox(std::shared_ptr<VertexArray> skyboxData);
	void uploadCommonUniforms(glm::mat4 viewProjectionMatrix, PointLight light, glm::vec3 viewPos, glm::mat4 modelMat, glm::mat4 skyboxMat);

public:
	Renderer();

	unsigned init();
	void draw(glm::mat4 viewProjectionMat, glm::vec3 cameraPos, PointLight light, std::shared_ptr<VertexArray> modelData, glm::mat4 modelMat, std::vector<Material> modelMaterials, std::vector<DrawCall> modelDrawCalls, std::shared_ptr<VertexArray> skyboxData, glm::mat4 skyboxMat);
	unsigned shutdown();
};

#endif // !RENDERER_H_
