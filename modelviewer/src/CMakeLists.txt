add_library(ModelViewerLib
    model_loder.cpp model_loder.h
    renderer.cpp renderer.h
    model_viewer.cpp model_viewer.h
)

# Include directories for the library
target_include_directories(ModelViewerLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/framework/GLFWApplication
    ${CMAKE_SOURCE_DIR}/framework/GeometricTools
    ${CMAKE_SOURCE_DIR}/framework/Rendering
    ${CMAKE_SOURCE_DIR}/framework/Projection
)

# Link Lab3Application with required dependencies
target_link_libraries(ModelViewerLib
    GLFWApplication
    GeometricTools
    Rendering
    Projection
    Shaders
    glad
    glfw
    stb
    glm
)

target_compile_definitions(ModelViewerLib PRIVATE
  TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

target_compile_definitions(ModelViewerLib PRIVATE STB_IMAGE_IMPLEMENTATION)