set(DearImGui_DIR ${BASEPATH}/3rdparty/DearImGui)

add_library(UI
	${DearImGui_DIR}/imgui.cpp
	${DearImGui_DIR}/imgui_demo.cpp
	${DearImGui_DIR}/imgui_draw.cpp
	${DearImGui_DIR}/imgui_tables.cpp
	${DearImGui_DIR}/imgui_widgets.cpp
	${DearImGui_DIR}/backends/imgui_impl_opengl3.cpp
	${DearImGui_DIR}/backends/imgui_impl_glfw.cpp
	
	UIManager/UIManager.cpp
	Scenes/ViewScene/ViewScene.cpp
	Scenes/DetailsScene/DetailsScene.cpp
	Scenes/BasicScene/BasicScene.cpp
	Scenes/OptionsScene/OptionsScene.cpp
	Scenes/IPmovesControlScene/IPmovesControlScene.cpp
	Helpers/Alignment.cpp
		)

target_link_libraries(UI PRIVATE glfw)
target_link_libraries(UI PRIVATE Utils)
target_link_libraries(UI PRIVATE Graphics)

target_include_directories(UI PUBLIC ${DearImGui_DIR})
target_include_directories(UI PUBLIC ${BASEPATH}/3rdparty/glew/include)
target_include_directories(UI PUBLIC ${BASEPATH}/src/GUI)
target_include_directories(UI PUBLIC ${BASEPATH}/3rdparty/pfd)