#include "UIManager.h"
#include "UIManager.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#define GLSL_VERSION "#version 330"

UIManager::UIManager(GLFWwindow* glfw_window): view_scene_(0, 0) {
	InitImGui(glfw_window);
}

void UIManager::DrawUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	MakeUIMarkUp();
	view_scene_.Render();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::InitImGui(GLFWwindow* glfw_window) const {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void UIManager::MakeUIMarkUp() {
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), 0);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 35.0f), 0);
	ImGui::Button("Option1");
	ImGui::SameLine();
	ImGui::Button("Option2");
	ImGui::SameLine();
	ImGui::Button("Option3");
	ImGui::SameLine();
	ImGui::Button("Option4");
	ImGui::End();
}

UIManager::~UIManager() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

ViewScene& UIManager::GetViewScene() {
	return view_scene_;
}