set(DearImGui_DIR ../../../3rdparty/DearImGui)

add_library(UI
	${DearImGui_DIR}/imgui.cpp
	${DearImGui_DIR}/imgui_draw.cpp
	${DearImGui_DIR}/imgui_tables.cpp
	${DearImGui_DIR}/imgui_widgets.cpp
	${DearImGui_DIR}/backends/imgui_impl_opengl3.cpp
	${DearImGui_DIR}/backends/imgui_impl_glfw.cpp
	
	UIManager/UIManager.cpp
		)

target_link_libraries(UI PRIVATE glfw)
target_link_libraries(UI PRIVATE Utils)

target_include_directories(UI PRIVATE ${DearImGui_DIR})