add_library(Graphics
	Shader/Shader.cpp
	VertexObjects/VertexObject.cpp
	VertexObjects/VertexObjectIndexed.cpp
	VertexObjects/VertexObjectCustom.cpp
	RenderBuffer/RenderBuffer.cpp
	Primitives/Cube/Cube.cpp
		)

target_link_libraries(Graphics PRIVATE glfw)
target_link_libraries(Graphics PRIVATE libglew_static)
target_link_libraries(Graphics PRIVATE Utils)
target_link_libraries(Graphics PRIVATE glm::glm)

target_include_directories(GUI PUBLIC ../../../3rdparty/glwf/includes)
target_include_directories(GUI PUBLIC ../../../3rdparty/glew/include)
target_include_directories(GUI PUBLIC ../../../3rdparty/GLM/glm)