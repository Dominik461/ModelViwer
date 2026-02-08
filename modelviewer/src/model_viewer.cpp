#include "model_viewer.h"
#include "model_viewer.h"

ModelViewer::ModelViewer()
    : GLFWApplication("Model Viewer", "1.0", 768, 1024), m_window(nullptr),
    m_modelData(nullptr)
{}

ModelViewer::~ModelViewer()
{
    shutdown();
}

unsigned ModelViewer::init()
{
    if (!getIsOpenGLInitialized())
        return 1;

    m_window = getWindow();
    createCamera();

    loadCube();
    glfwSetKeyCallback(m_window, keyCallback);
    m_renderer = std::make_unique<Renderer>();

    TextureManager::GetInstance()->LoadCubeMapArrayRGBA("Skybox", std::string(TEXTURES_DIR), skyboxFiles, 0);

    return 0;
}

unsigned ModelViewer::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        float time = glfwGetTime();
        m_deltaTime = time - m_lastFrameTime;
        m_lastFrameTime = time;

        m_renderer->draw(m_camera.GetViewProjectionMatrix(), m_camera.GetPosition(), m_positionLight, m_lightMix, m_modelData, modelMatrix, m_skyboxData, skyboxMatrix);

        // Swap buffers and poll events
        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
    return 0;
}

void ModelViewer::shutdown()
{
    m_renderer->shutdown();
}

void ModelViewer::createCamera()
{
    float fov = 45.0f;
    float nearPlane = 1.0f;
    float farPlane = 50.0f;

    glm::vec3 ori = { 0, 0, 0 };
    glm::vec3 up = { 0, 1, 0 };

    m_camera = PerspectiveCamera({ fov, float(WINDOW_WIDTH) , float(WINDOW_HEIGHT), nearPlane, farPlane }, m_CAMERA_POS, ori, up);
}

void ModelViewer::loadCube()
{
    auto data = GeometricTools::GetUnitCube3D();

    auto& verticesData = data.first;
    auto& indicesData = data.second;

    std::shared_ptr<IndexBuffer> IBO = std::make_shared<IndexBuffer>(indicesData.data(), indicesData.size());

    BufferLayout commomLayout = BufferLayout({ {ShaderDataType::Float3, "position"}, {ShaderDataType::Float3, "normals"}, {ShaderDataType::Float2, "tcoords"}, {ShaderDataType::Float3, "color"}});
    std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(verticesData.data(), verticesData.size() * sizeof(verticesData[0]));
    VBO->SetLayout(commomLayout);

    m_modelData = std::make_shared<VertexArray>();
    m_modelData->Bind();
    m_modelData->SetIndexBuffer(IBO);
    m_modelData->AddNormalVertexBuffer(VBO);


    auto& verticesDataSky = GeometricTools::UnitCube3DVertices;
    auto& indicesDataSky = GeometricTools::UnitCube3DIndices;

    std::shared_ptr<IndexBuffer> IBOskybox = std::make_shared<IndexBuffer>(indicesDataSky.data(), indicesDataSky.size());

    BufferLayout skyboxLayout = BufferLayout({ {ShaderDataType::Float3, "position"} });
    std::shared_ptr<VertexBuffer> VBOskybox = std::make_shared<VertexBuffer>(verticesDataSky.data(), verticesDataSky.size() * sizeof(verticesDataSky[0]));
    VBOskybox->SetLayout(skyboxLayout);

    m_skyboxData = std::make_shared<VertexArray>();
    m_skyboxData->Bind();
    m_skyboxData->SetIndexBuffer(IBOskybox);
    m_skyboxData->AddNormalVertexBuffer(VBOskybox);

    glm::vec3 rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scaleVec = glm::vec3(50.0f);
    skyboxMatrix *= MatrixOperations::getTransformedMatrix(scaleVec, rotationVec, 0, modelPos);
}

void ModelViewer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* viewer = static_cast<ModelViewer*>(
        glfwGetWindowUserPointer(window)
        );

    double rotateAmountX = 0;
    double rotateAmountY = 0;

    // Left
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
    {
        rotateAmountY -= viewer->ROTATION_SPEED;
    }
    // Right
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
    {
        rotateAmountY += viewer->ROTATION_SPEED;
    }
    // Up
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
    {
        rotateAmountX += viewer->ROTATION_SPEED;
    }
    // Down
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
    {
        rotateAmountX -= viewer->ROTATION_SPEED;
    }


    viewer->rotateModel(rotateAmountX, rotateAmountY);
}

void ModelViewer::rotateModel(double rotateAmountX, double rotateAmountY)
{
    glm::vec3 rotationVec;
    double x = rotateAmountX * sensitvity;
    double y = rotateAmountY * sensitvity;

    rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
    modelMatrix *= MatrixOperations::getTransformedMatrix(modelScale, rotationVec, y, modelPos);

    rotationVec = glm::vec3(1.0f, 0.0f, 0.0f);

    modelMatrix *= MatrixOperations::getTransformedMatrix(modelScale, rotationVec, x, modelPos);

    std::cout << "rotateAmountX: " << x << " rotateAmountY: " << y << std::endl;
}