add_library(GUI
	Window/Window.cpp
	Shader/Shader.cpp
		)

target_link_libraries(GUI PRIVATE glfw)
target_link_libraries(GUI PRIVATE libglew_static)
target_link_libraries(GUI PRIVATE Utils)
target_include_directories(GUI PUBLIC ..)
target_include_directories(GUI PUBLIC ../../3rdparty/glwf/includes)
target_include_directories(GUI PUBLIC ../../3rdparty/glew/include)