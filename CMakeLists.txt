cmake_minimum_required(VERSION 3.16)

#set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_C_COMPILER clang)

set(PROJECT_NAME ProgramTraceVisualizer)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # Just for clangd to work correctly

set(glew-cmake_BUILD_SHARED FALSE)

set(BASEPATH "${CMAKE_SOURCE_DIR}")

#set(CMAKE_CXX_FLAGS -fsanitize-coverage=func,trace-pc-guard)

add_subdirectory(3rdparty/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(3rdparty/glew ${CMAKE_CURRENT_BINARY_DIR}/glew)
add_subdirectory(3rdparty/GLM ${CMAKE_CURRENT_BINARY_DIR}/glm)
add_subdirectory(src/GUI)
add_subdirectory(src/Utils)
add_subdirectory(src/Core)

add_executable(${PROJECT_NAME} src/main.cpp) #external-utilities/Coverage/covers.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GUI)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE Core)

target_include_directories(${PROJECT_NAME} PRIVATE ${BASEPATH}/src)

#add_subdirectory(external-utilities/Coverage)