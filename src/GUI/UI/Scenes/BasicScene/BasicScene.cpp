#include "BasicScene.h"

#include <imgui.h>

namespace {
static constexpr const ImGuiWindowFlags kBasicWidgetHint =
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

BasicScene::BasicScene(const std::pair<float, float>& position, const std::pair<float, float>& size,
                       const std::string& name)
    : position_(position), size_(size), name_(name), widget_hint_(kBasicWidgetHint), visible_(true) {
}

void BasicScene::UpdateOnResize(const std::pair<float, float>& position,
                                const std::pair<float, float>& size) {
    position_ = position;
    size_ = size;
}

void BasicScene::MoveAndResize() const {
    ImGui::SetWindowPos(ImVec2(position_.first, position_.second), 0);
    ImGui::SetWindowSize(ImVec2(size_.first, size_.second), 0);
    ImGui::GetStyle().WindowBorderSize = 1.0f;
}

void BasicScene::Render() const {
    if (!visible_) {
        return;
    }
    ImGui::Begin(name_.c_str(), nullptr, static_cast<ImGuiWindowFlags>(widget_hint_));
    MoveAndResize();
    RenderInner();
    ImGui::End();
}

void BasicScene::SetVisibility(bool new_visibility) {
    visible_ = new_visibility;
}

BasicScene::~BasicScene() = default;