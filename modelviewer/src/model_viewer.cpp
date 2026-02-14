#include "model_viewer.h"
#define TINYOBJLOADER_IMPLEMENTATION

#include "model_viewer.h"
#include "tiny_obj_loader.h"
//TinyObjLoader

tinyobj::ObjReaderConfig reader_config;
tinyobj::ObjReader reader;


ModelViewer::ModelViewer()
    : GLFWApplication("Model Viewer", "1.0", 1080, 1920), m_window(nullptr),
    m_modelData(nullptr), m_sunData(nullptr), m_skyboxData(nullptr), m_deltaTime(0),
    m_lastFrameTime(0)
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

    loadSkybox();
    glfwSetKeyCallback(m_window, keyCallback);
    
    m_renderer = std::make_unique<Renderer>();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    createPointLight();
    TextureManager::GetInstance()->LoadCubeMapArrayRGBA("Skybox", std::string(TEXTURES_DIR), skyboxFiles, 0);
    scaleSkybox();

    return 0;
}

unsigned ModelViewer::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        float time = glfwGetTime();
        m_deltaTime = time - m_lastFrameTime;
        m_lastFrameTime = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controls");

        ImGui::SetWindowSize(
            ImVec2(
                std::max(ImGui::GetWindowWidth(), 50.0f),
                std::max(ImGui::GetWindowHeight(), 150.0f)
            )
        );

        if (ImGui::Button("Select model"))
        {
            const char* folder = tinyfd_selectFolderDialog(
                "Select Folder",
                ""
            );

            if (folder)
            {
                // User selected a folder
                std::string selectedPath = folder;
                // Continue your logic here
                loadCustomModel(selectedPath);
            }
        }

        ImGui::Text("Model Rotation:");
        ImGui::Text("Q & E: -/+ Pitch");
        ImGui::Text("A & S: -/+ Yaw");
        ImGui::Text("Z & X: -/+ Roll");
        ImGui::SliderFloat("Model rotation speed", &model_rotation_speed, 10.0f, 255.0f);

        ImGui::Text("Camera controls");
        ImGui::Text("N & M: Rotate around center");
        ImGui::SliderFloat("Camera rotation speed", &cam_rotation_speed, 10.0f, 1024.0f);
        if (ImGui::SliderFloat("Camera height", &camPosY, -3500.0f, 3500.0f))
        {
            m_camera.SetHeight(camPosY);
        }
        if (ImGui::SliderFloat("Camera distance", &camDistance, 10.0f, 5000.0f))
        {
            m_camera.SetDistance(camDistance);
        }

        ImGui::Text("Light controls");
        ImGui::Text("Left & Right: Rotate around center");
        ImGui::SliderFloat("Light rotation speed", &light.AngularSpeed, 10.0f, 2048.0f);
        if (ImGui::SliderFloat("Light height", &light.Height, -3500.0f, 3500.0f))
        {
            UpdateLight();
        }
        if(ImGui::SliderFloat("Light distance", &light.Radius, 0.0f, 5000.0f))
        {
            UpdateLight();
        }
        if(ImGui::ColorEdit3("Light color", glm::value_ptr(light.Color)))
        {
            UpdateLight();
        }
        ImGui::SliderInt("Light radius", &light.LightRadius, 1.0f, 10.0f);

        ImGui::Text("Shader controls");
        ImGui::Checkbox("Check normals", &checkNormals);
        ImGui::End();

        ImGui::Render();

        m_cam_pos.y = camPosY;

        m_renderer->draw(m_camera.GetViewProjectionMatrix(), m_camera.GetPosition(), light, m_modelData, modelMatrix, m_materials, modelDrawCalls, m_skyboxData, skyboxMatrix, m_sunData, sunMatrix, isCelShadingEnabled, celShadingSteps, checkNormals);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
    return 0;
}

void ModelViewer::shutdown()
{
    m_renderer->shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ModelViewer::createCamera()
{
    float fov = 45.0f;
    float nearPlane = 1.0f;
    float farPlane = 20000.0f;

    glm::vec3 ori = { 0, 0, 0 };
    glm::vec3 up = { 0, 1, 0 };

    m_camera = PerspectiveCamera({ fov, float(WINDOW_WIDTH) , float(WINDOW_HEIGHT), nearPlane, farPlane }, m_cam_pos, ori, up);
    m_camera.SetDistance(camDistance);
}

void ModelViewer::createPointLight()
{
    light = PointLight();
    light.Angle = 0.0f;
    light.AngularSpeed = 100.0f;
    light.Color = glm::vec3(1.0f);
    light.Height = 25.f;
    light.Radius = 15.f;
    UpdateLight();
}

void ModelViewer::loadSkybox()
{
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


    auto& sunData = GeometricTools::CreateSphere();

    auto& verticesDataSun = sunData.first;
    auto& indicesDataSun = sunData.second;

    std::shared_ptr<IndexBuffer> IBOsun = std::make_shared<IndexBuffer>(indicesDataSun.data(), indicesDataSun.size());

    BufferLayout sunLayout = BufferLayout({ {ShaderDataType::Float3, "position"}, {ShaderDataType::Float3, "normals"}, {ShaderDataType::Float2, "tcoords"}, {ShaderDataType::Float3, "color"} });
    std::shared_ptr<VertexBuffer> VBOsun = std::make_shared<VertexBuffer>(verticesDataSun.data(), verticesDataSun.size() * sizeof(verticesDataSun[0]));
    VBOsun->SetLayout(sunLayout);

    m_sunData = std::make_shared<VertexArray>();
    m_sunData->Bind();
    m_sunData->SetIndexBuffer(IBOsun);
    m_sunData->AddNormalVertexBuffer(VBOsun);
}

void ModelViewer::scaleSkybox()
{
    glm::vec3 rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
    float scaleVal = 17500.0f;
    glm::vec3 scaleVec = glm::vec3(scaleVal);
    skyboxMatrix = MatrixOperations::getTransformedMatrix(scaleVec, rotationVec, 0, modelPos);
}

void ModelViewer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* viewer = static_cast<ModelViewer*>(
        glfwGetWindowUserPointer(window)
        );

    double rotateAmountX = 0;
    double rotateAmountY = 0;

    // Left
    if (key == GLFW_KEY_Q)
    {
        viewer->modelAngleX -= viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Right
    if (key == GLFW_KEY_W)
    {
        viewer->modelAngleX += viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Up
    if (key == GLFW_KEY_A)
    {
        viewer->modelAngleY -= viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Down
    if (key == GLFW_KEY_S)
    {
        viewer->modelAngleY += viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Up
    if (key == GLFW_KEY_Z)
    {
        viewer->modelAngleZ -= viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Down
    if (key == GLFW_KEY_X)
    {
        viewer->modelAngleZ += viewer->model_rotation_speed * viewer->m_deltaTime;
    }
    // Up
    if (key == GLFW_KEY_N)
    {
        viewer->m_camera.AddYaw(viewer->cam_rotation_speed * viewer->m_deltaTime);
    }
    // Down
    if (key == GLFW_KEY_M)
    {
        viewer->m_camera.AddYaw(viewer->cam_rotation_speed * viewer->m_deltaTime);
    }
    // Up
    if (key == GLFW_KEY_LEFT)
    {
        viewer->light.Angle -= viewer->light.AngularSpeed * viewer->m_deltaTime;
        viewer->UpdateLight();
    }
    // Down
    if (key == GLFW_KEY_RIGHT)
    {
        viewer->light.Angle += viewer->light.AngularSpeed * viewer->m_deltaTime;
        viewer->UpdateLight();
    }

    viewer->rotateModel();
}

void ModelViewer::rotateModel()
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, modelAngleX, glm::vec3(1, 0, 0));
    rotation = glm::rotate(rotation, modelAngleY, glm::vec3(0, 1, 0));
    rotation = glm::rotate(rotation, modelAngleZ, glm::vec3(0, 0, 1));

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), modelPos);

    modelMatrix = translation * rotation;
}

void ModelViewer::UpdateLight()
{
    float rad = glm::radians(light.Angle);

    light.Position.x = light.Radius * sin(rad);
    light.Position.z = light.Radius * cos(rad);
    light.Position.y = light.Height;

    glm::vec3 rotationVec = glm::vec3(0.0f, 1.0f, 0.0f);
    float scaleVal = 5.f;
    glm::vec3 scaleVec = glm::vec3(scaleVal);
    sunMatrix = MatrixOperations::getTransformedMatrix(scaleVec, rotationVec, 0, light.Position);
}

void ModelViewer::loadCustomModel(std::string path)
{
    std::string objPath = findObiFile(path);
    std::cout << "OBJ File: " << objPath << std::endl;

    SanitizeObjAndMtlFilenames(objPath);

    if (objPath != "")
    {
        m_modelData = nullptr;

        std::vector<float> vertices = std::vector<float>();
        std::vector<unsigned int> indices = std::vector<unsigned int>();
        std::vector<BufferAttribute> modelAttributes = { {ShaderDataType::Float3, "position"} };
        modelDrawCalls = std::vector<DrawCall>();

        unsigned int index = 0;

        reader_config.mtl_search_path = path;
        reader_config.triangulate = true;

        if (!reader.ParseFromFile(objPath, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::unordered_map<int, std::vector<unsigned int>> materialIndexMap;

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                int materialID = shapes[s].mesh.material_ids[f];
                if (materialID < 0)
                    materialID = 0; // fallback

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    vertices.insert(vertices.end(), { vx, vy, vz });

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                        vertices.insert(vertices.end(), { nx, ny, nz });
                    }
                    else
                    {
                        std::cout << "NO NORMALS" << std::endl;
                        // TODO POST CALCULATION
                        vertices.insert(vertices.end(), { 0.f, 0.f, 1.f });
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                        vertices.insert(vertices.end(), { tx, ty });
                    }
                    else
                    {
                        std::cout << "NO TEXCOORDS" << std::endl;
                        vertices.insert(vertices.end(), { 0.f, 0.f });
                    }

                    if (!attrib.colors.empty())
                    {
                        tinyobj::real_t red = attrib.colors[3 * size_t(idx.vertex_index) + 0];
                        tinyobj::real_t green = attrib.colors[3 * size_t(idx.vertex_index) + 1];
                        tinyobj::real_t blue = attrib.colors[3 * size_t(idx.vertex_index) + 2];
                        vertices.insert(vertices.end(), { red, green, blue });
                    }
                    else
                    {
                        vertices.insert(vertices.end(), { 1.f, 1.f, 1.f });
                    }

                    indices.push_back(index);
                    materialIndexMap[materialID].push_back(index);

                    index++;
                }
                index_offset += fv;
                


                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }

        //std::cout << "attrib: " << attrib << std::endl;
        std::cout << "shapes: " << shapes.size() << std::endl;
        std::cout << "materials: " << materials.size() << std::endl;

        std::vector<unsigned int> finalIndices;
        unsigned int offset = 0;

        for (auto& [matID, matIndices] : materialIndexMap)
        {
            DrawCall dc;
            dc.indexOffset = offset;
            dc.indexCount = matIndices.size();
            dc.materialID = matID;

            modelDrawCalls.push_back(dc);

            finalIndices.insert(finalIndices.end(),
                matIndices.begin(),
                matIndices.end());

            offset += matIndices.size();
        }

        m_materials.clear();
        for (const auto& mat : materials)
        {
            Material m;

            m.diffuse = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
            m.specular = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
            m.ambient = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
            m.shininess = mat.shininess;

            if (!mat.diffuse_texname.empty())
            {
                stbi_set_flip_vertically_on_load(true);
                std::string fullPath = path + "/" + mat.diffuse_texname;
                m.diffuseTex = TextureManager::GetInstance()->LoadModelTexture2DRGBA(fullPath, true);
            }

            m_materials.push_back(m);
        }


        BufferLayout commomLayout;

            std::cout << "Has normals and texcords" << std::endl;
            std::cout << "Number of vertices: " << vertices.size()/11 << std::endl;
            commomLayout = BufferLayout({ {ShaderDataType::Float3, "position"}, {ShaderDataType::Float3, "normals"}, {ShaderDataType::Float2, "tcoords"}, {ShaderDataType::Float3, "color"} });

        std::shared_ptr<IndexBuffer> IBO = std::make_shared<IndexBuffer>(finalIndices.data(), finalIndices.size());

        
        std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(vertices[0]));
        VBO->SetLayout(commomLayout);

        m_modelData = std::make_shared<VertexArray>();
        m_modelData->Bind();
        m_modelData->SetIndexBuffer(IBO);
        m_modelData->AddNormalVertexBuffer(VBO);
    }
}

std::string ModelViewer::findObiFile(std::string path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".obj")
        {
            return entry.path().string(); // full path
        }
    }

    return ""; // not found
}

void ModelViewer::SanitizeObjAndMtlFilenames(const fs::path& objPath) {
    fs::path objDir = objPath.parent_path();

    // ---- Read OBJ ----
    std::ifstream in(objPath);
    if (!in) return;

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line)) {
        std::string trimmed = trim(line);

        if (starts_with(trimmed, "mtllib ")) {
            std::string rawName = trim(trimmed.substr(7));
            fs::path fullPath = objDir / rawName;

            // If the entire string resolves to a single file → spaces in filename
            if (fs::exists(fullPath)) {
                std::string fixedName = replace_spaces(rawName);
                fs::path fixedPath = objDir / fixedName;

                if (!fs::exists(fixedPath)) {
                    fs::rename(fullPath, fixedPath);
                }

                line = "mtllib " + fixedName;

                // Sanitize the MTL file too
                SanitizeMtlTextureFilenames(fixedPath);
            }
        }

        lines.push_back(line);
    }
    in.close();

    // ---- Rewrite OBJ ----
    std::ofstream out(objPath, std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}

void ModelViewer::SanitizeMtlTextureFilenames(const fs::path& mtlPath) {
    fs::path mtlDir = mtlPath.parent_path();

    std::ifstream in(mtlPath);
    if (!in) return;

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line)) {
        std::string trimmed = trim(line);

        // Handle all common texture directives
        static const std::vector<std::string> keys = {
            "map_Kd ", "map_Ks ", "map_Ka ",
            "map_Bump ", "bump ", "disp ", "map_d "
        };

        for (const auto& key : keys) {
            if (starts_with(trimmed, key)) {
                std::string rawName = trim(trimmed.substr(key.size()));
                fs::path fullPath = mtlDir / rawName;

                if (fs::exists(fullPath)) {
                    std::string fixedName = replace_spaces(rawName);
                    fs::path fixedPath = mtlDir / fixedName;

                    if (!fs::exists(fixedPath)) {
                        fs::rename(fullPath, fixedPath);
                    }

                    line = key + fixedName;
                }
                break;
            }
        }

        lines.push_back(line);
    }
    in.close();

    // ---- Rewrite MTL ----
    std::ofstream out(mtlPath, std::ios::trunc);
    for (const auto& l : lines)
        out << l << '\n';
}