add_library(App
	App.cpp
	AppStateMachine.cpp
		)

target_link_libraries(App PRIVATE Core)
target_link_libraries(App PRIVATE GUI)
target_link_libraries(App PRIVATE Utils)
target_link_libraries(App PRIVATE Controllers)

target_include_directories(App PUBLIC ${BASEPATH}/src)
target_include_directories(App PUBLIC ${BASEPATH}/3rdparty/glew/include)
target_include_directories(App PUBLIC ${BASEPATH}/3rdparty/glfw/include)
target_include_directories(App PUBLIC ${BASEPATH}/3rdparty/pfd)
target_include_directories(App PUBLIC ${BASEPATH}/src/GUI)
target_include_directories(App PUBLIC ${BASEPATH}/3rdparty/GLM)
target_include_directories(App PUBLIC ${BASEPATH}/src/Utils)