add_library(Plotting
	Plot2DMesh/Plot2DMesh.cpp
	HillbertCurve/HillbertCurve.cpp
		)

target_link_libraries(Plotting PRIVATE Utils)
target_link_libraries(Plotting PRIVATE glm::glm)
target_link_libraries(Plotting PRIVATE Graphics)

target_include_directories(Plotting PUBLIC ../../GUI)
target_include_directories(Plotting PUBLIC ../../../3rdparty/GLM/glm)
target_include_directories(Plotting PUBLIC ../../../3rdparty/glew/include)