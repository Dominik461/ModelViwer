add_executable(ModelViewer
    main.cpp
)

target_link_libraries(ModelViewer
    ModelViewerLib
)

# Set output directory for the test executable
set_target_properties(ModelViewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/bottom.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/bottom.jpg
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/back.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/back.jpg
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/front.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/front.jpg
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/left.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/left.jpg
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/right.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/right.jpg
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/top.jpg
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/top.jpg
)



add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/models/Billy/Billy.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models/Billy/Billy.png
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/models/Billy/Billy.obj
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models/Billy/Billy.obj
)

add_custom_command(
  TARGET ModelViewer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/models/Billy/Billy.mtl
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models/Billy/Billy.mtl
)