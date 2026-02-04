add_library(Projection INTERFACE)
add_library(Framework::Projection ALIAS Projection)
target_include_directories(Projection INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Projection INTERFACE glad glfw glm)