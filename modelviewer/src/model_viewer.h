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
#include <iostream>
#include <filesystem>
#include <string>

enum RotationAxis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

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
	const glm::vec3 m_CAMERA_POS = glm::vec3(0.0f, 0.0f, 200.0f);

	std::shared_ptr<VertexArray> m_modelData;
    std::vector<DrawCall> modelDrawCalls;
	std::shared_ptr<VertexArray> m_skyboxData;

	glm::vec3 modelPos = glm::vec3(0);
	glm::vec3 modelScale = glm::vec3(1);
	RotationAxis rotaionAxis= X_AXIS;
	const float ROTATION_SPEED = 0.1f;
	float sensitvity= 50.0f;
	bool rotateZ = false;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 skyboxMatrix = glm::mat4(1.0f);

	double lastX = 0.0;
	double lastY = 0.0;
	bool firstMouse = true;

	glm::vec3 m_positionLight = glm::vec3{ 0.0f, 0.0f, 14.0f };
	float m_lightMix = 0.0f;

	float m_deltaTime;
	float m_lastFrameTime;

	std::array<float, 3 * 2 * 6> axisVertices = GeometricTools::axisVertices;

    std::array<float, 3 * 6 * 6> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    std::array<std::string, 6> skyboxFiles = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

	void loadCube();

	void createCamera();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void rotateModel(double rotateAmountX, double rotateAmountY);

    void loadCustomModel(std::string path);

    std::string findObiFile(std::string path);

    void SanitizeObjAndMtlFilenames(const fs::path& objPath);
    void SanitizeMtlTextureFilenames(const fs::path& mtlPath);
};
#endif // !MODELVIEWER_H_