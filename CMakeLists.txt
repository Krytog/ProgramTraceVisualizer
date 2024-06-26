cmake_minimum_required(VERSION 3.16)

#set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_C_COMPILER clang)

set(PROJECT_NAME ProgramTraceVisualizer)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # Just for clangd to work correctly

set(glew-cmake_BUILD_SHARED FALSE)

set(BASEPATH "${CMAKE_SOURCE_DIR}")

#set(CMAKE_CXX_FLAGS -fsanitize-coverage=func,trace-pc-guard)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -funroll-loops -ftree-vectorize -DNDEBUG")

add_subdirectory(3rdparty/glew ${CMAKE_CURRENT_BINARY_DIR}/glew EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/GLM ${CMAKE_CURRENT_BINARY_DIR}/glm EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/word2vec++ ${CMAKE_CURRENT_BINARY_DIR}/word2vec++ EXCLUDE_FROM_ALL)


# the following lib is verbose as hell (and messy), so we need to shut it
function(message)
    if (NOT MESSAGE_QUIET)
        _message(${ARGN})
    endif()
endfunction()

set(BUILD_TESTING OFF)
set(MESSAGE_QUIET ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(3rdparty/umap ${CMAKE_CURRENT_BINARY_DIR}/umap EXCLUDE_FROM_ALL) 
set(MESSAGE_QUIET OFF)

# The following library has to be installed after umap. Otherwise, there will be conflict in uninstall target
add_subdirectory(3rdparty/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw EXCLUDE_FROM_ALL)

add_subdirectory(src/GUI)
add_subdirectory(src/Utils)
add_subdirectory(src/Core)
add_subdirectory(src/App)
add_subdirectory(src/Controllers)

add_executable(${PROJECT_NAME} src/main.cpp) #external-utilities/Coverage/covers.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE App)

target_include_directories(${PROJECT_NAME} PRIVATE ${BASEPATH}/src)

#add_subdirectory(external-utilities/Coverage)