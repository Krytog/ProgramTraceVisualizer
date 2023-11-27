cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME CubeDataVisualizer)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)


set(glew-cmake_BUILD_SHARED FALSE)


add_subdirectory(3rdparty/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(3rdparty/glew ${CMAKE_CURRENT_BINARY_DIR}/glew)
add_subdirectory(src/GUI)
add_subdirectory(src/Utils)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GUI)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)