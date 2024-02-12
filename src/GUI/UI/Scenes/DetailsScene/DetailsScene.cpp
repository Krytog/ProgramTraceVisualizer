#include "DetailsScene.h"

#include <imgui.h>

#define DetailsScene_InnerName "Details"

namespace {
	static constexpr const ImGuiWindowFlags kOverrideWidgerHint = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar;
}

DetailsScene::DetailsScene(const std::pair<float, float>& position, const std::pair<float, float>& size) :
	BasicScene(position, size, DetailsScene_InnerName) {
	widget_hint_ = kOverrideWidgerHint;
}

std::deque<std::string>& DetailsScene::GetInnerBuffer() {
	return lines_;
}

void DetailsScene::PushLine(const std::string& line) {
	lines_.push_back(line);
}

void DetailsScene::PopFrontLine() {
	lines_.pop_front();
}

void DetailsScene::PopBackLine() {
	lines_.pop_back();
}

void DetailsScene::RenderInner() const {
	RenderStaticUI();
	RenderText();
}

void DetailsScene::RenderText() const {
	for (auto line : lines_) {
		ImGui::Text(line.c_str());
	}
}

void DetailsScene::RenderStaticUI() const {
	ImGui::Text(DetailsScene_InnerName);
}