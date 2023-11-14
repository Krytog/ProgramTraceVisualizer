cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME CubeDataVisualizer)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdparty/glfw)

add_executable(CubeDataVisualizer src/main.cpp)
