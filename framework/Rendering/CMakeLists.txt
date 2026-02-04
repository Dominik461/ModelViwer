add_library(Rendering 
    VertexBuffer.cpp VertexBuffer.h
    IndexBuffer.cpp IndexBuffer.h 
    BufferLayout.h 
    VertexArray.cpp VertexArray.h 
    ShadersDataTypes.h
    Shader.h Shader.cpp
    Camera.h
    OrthographicCamera.h OrthographicCamera.cpp
    PerspectiveCamera.h PerspectiveCamera.cpp
    TextureManager.h TextureManager.cpp
)

add_library(Framework::Rendering ALIAS Rendering)
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rendering PUBLIC glad glfw glm stb)