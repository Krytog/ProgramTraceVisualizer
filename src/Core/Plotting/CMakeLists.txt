add_library(Plotting
	PlotMesh/PlotMesh.cpp
	PlotMesh/Plot2DMesh.cpp
	PlotMesh/Plot3DMesh.cpp
	HilbertCurve/HilbertCurve.cpp
	HilbertCurve/HilbertCurveManager.cpp
		)

target_link_libraries(Plotting PRIVATE Utils)
target_link_libraries(Plotting PRIVATE glm::glm)
target_link_libraries(Plotting PRIVATE Graphics)

target_include_directories(Plotting PUBLIC ${BASEPATH}/src/GUI)
target_include_directories(Plotting PUBLIC ${BASEPATH}/3rdparty/GLM)
target_include_directories(Plotting PUBLIC ${BASEPATH}/3rdparty/glew/include)