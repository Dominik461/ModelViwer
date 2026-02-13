#ifndef MODELVIEWER_H_
#define MODELVIEWER_H_

#include "GLFWApplication.h"
#include "GeometricTools.h"
#include "PerspectiveCamera.h"
#include "MatrixOperations.h"
#include "glm/glm.hpp"
#include "TextureManager.h"
#include "VertexArray.h"
#include "renderer.h"
#include "util.h"
#include "drawcall.h"
#include "imgui.h"
#include "material.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "tinyfiledialogs.h"
#include "pointlight.h"
#include <iostream>
#include <filesystem>
#include <string>


class ModelViewer : GLFWApplication
{
public:
	ModelViewer();
	~ModelViewer();

	unsigned init() override;
	unsigned run() override;
	void shutdown() override;

private:
	GLFWwindow* m_window;
	std::unique_ptr<Renderer> m_renderer;

	PerspectiveCamera m_camera;
	float camPosY = 0.0f;
	float camDistance = 200.0f;
	float camAngle = 0.0f;
	glm::vec3 m_cam_pos = glm::vec3(0.0f, camPosY, camDistance);


	std::shared_ptr<VertexArray> m_sunData;
	std::shared_ptr<VertexArray> m_modelData;
    std::vector<DrawCall> modelDrawCalls;
	std::shared_ptr<VertexArray> m_skyboxData;
    std::vector<Material> m_materials;


	glm::vec3 modelPos = glm::vec3(0);
	glm::vec3 modelScale = glm::vec3(1);
	float modelAngleX = 0.0f;
	float modelAngleY = 0.0f;
	float modelAngleZ = 0.0f;

	const float ROTATION_SPEED = 0.1f;
	float cam_rotation_speed = 100.0f;
	float model_rotation_speed = 100.0f;
	float sensitvity= 50.0f;
	bool rotateZ = false;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 skyboxMatrix = glm::mat4(1.0f);
	glm::mat4 sunMatrix = glm::mat4(1.0f);

	bool checkNormals = false;
	bool isCelShadingEnabled = false;
	int celShadingSteps = 4;
	
	PointLight light;

	float m_deltaTime;
	float m_lastFrameTime;

    std::array<std::string, 6> skyboxFiles = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

	void loadSkybox();
	void scaleSkybox();

	void createCamera();
	void createPointLight();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void rotateModel();
	void UpdateLight();

    void loadCustomModel(std::string path);

    std::string findObiFile(std::string path);

    void SanitizeObjAndMtlFilenames(const fs::path& objPath);
    void SanitizeMtlTextureFilenames(const fs::path& mtlPath);
};
#endif // !MODELVIEWER_H_