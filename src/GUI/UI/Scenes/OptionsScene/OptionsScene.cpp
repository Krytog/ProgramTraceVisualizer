#include "OptionsScene.h"

#include <imgui.h>

#define OptionsScene_InnerName "Options"

OptionsScene::OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size) :
	BasicScene(position, size, OptionsScene_InnerName) {}

void OptionsScene::RenderInner() const {
	ImGui::Button("Option1");
	ImGui::SameLine();
	ImGui::Button("Option2");
	ImGui::SameLine();
	ImGui::Button("Option3");
	ImGui::SameLine();
	ImGui::Button("Option4");
}