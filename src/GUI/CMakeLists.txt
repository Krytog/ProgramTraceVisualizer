add_library(GUI
	Window/window.cpp
		)

target_link_libraries(GUI PRIVATE glfw)
target_link_libraries(GUI PRIVATE libglew_static)
target_include_directories(GUI PUBLIC ..)
target_include_directories(GUI PUBLIC ../../3rdparty/glwf/includes)
target_include_directories(GUI PUBLIC ../../3rdparty/glew/include)