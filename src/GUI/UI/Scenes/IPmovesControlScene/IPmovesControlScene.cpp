#include "IPmovesControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include <cmath>

#define IPmovesControlScene_InnerName "IPmovesControl"

IPmovesControlScene::IPmovesControlScene(const std::pair<float, float>& position,
                                         const std::pair<float, float>& size)
    : BasicScene(position, size, IPmovesControlScene_InnerName) {
}

void IPmovesControlScene::RenderInner() const {
    ALIGNMENT_AT_BEGIN(ImGui::Button("Pause"));
    ImGui::SameLine();
    ALIGNMENT_AT_CENTER(ImGui::SliderFloat("##Progress", &cur_value_, 0.0f, 100.0f, "%.2f%%"));
    ImGui::SameLine();
    ALIGNMENT_AT_END(ImGui::Button("Settings"));
    ImGui::SameLine();
    // ImGui::ShowDemoWindow();
}

std::pair<bool, float> IPmovesControlScene::GetUserInput() {
    const constexpr float kPrecision = 1e-6;
    if (std::abs(cur_value_ - prev_value_) < kPrecision) {
        return {false, cur_value_ / 100};
    }
    prev_value_ = cur_value_;
    return {true, cur_value_ / 100};
}

void IPmovesControlScene::SetProgressValue(float progress) {
    const float progress_in_percents = 100 * progress;
    cur_value_ = progress_in_percents;
    prev_value_ = progress_in_percents;
}