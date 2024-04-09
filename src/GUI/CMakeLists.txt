add_library(GUI
	Window/Window.cpp
		)

add_subdirectory(Graphics)
add_subdirectory(UI)

target_link_libraries(GUI PRIVATE Utils)
target_link_libraries(GUI PRIVATE Graphics)
target_link_libraries(GUI PRIVATE UI)
target_link_libraries(GUI PRIVATE glfw)
target_link_libraries(GUI PRIVATE libglew_static)

target_include_directories(GUI PUBLIC ${BASEPATH}/src/GUI)
target_include_directories(GUI PUBLIC ${BASEPATH}/3rdparty/glwf/includes)
target_include_directories(GUI PUBLIC ${BASEPATH}/3rdparty/glew/include)