#include "IPmovesControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include <cmath>
#include "UI/Helpers/ValueWithPrev.h"

#define IPmovesControlScene_InnerName "IPmovesControl"

namespace {
const constexpr ImGuiColorEditFlags kColorEditFlags =
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;
}

IPmovesControlScene::IPmovesControlScene(const std::pair<float, float>& position,
                                         const std::pair<float, float>& size)
    : BasicScene(position, size, IPmovesControlScene_InnerName),
      progress_(0),
      pause_(false),
      new_color_({1.0f, 0.0f, 0.0f, 1.0f}),
      old_color_({0.0f, 0.0f, 1.0f, 1.0f}),
      sliding_window_size_(10),
      advance_(1),
      hilbert_degree_(5),
      max_memory_(1) {
}

void IPmovesControlScene::RenderInner() const {
    PauseButton();
    ImGui::SameLine();
    ProgressSlider();
    ImGui::SameLine();
    SettingsButton();
    // ImGui::ShowDemoWindow();
}

std::pair<bool, float> IPmovesControlScene::GetInputProgress() {
    auto progress_pair = GetValueAndChange(progress_);
    progress_pair.second /= 100;  // because the user inputs it in percents
    return progress_pair;
}

void IPmovesControlScene::SetProgressValue(float progress) {
    const float progress_in_percents = 100 * progress;
    progress_.cur_value_ = progress_in_percents;
    progress_.prev_value_ = progress_in_percents;
}

std::pair<bool, bool> IPmovesControlScene::GetInputPause() {
    return GetValueAndChange(pause_);
}

std::pair<bool, float[4]> IPmovesControlScene::GetInputNewColor() {
    return GetValueAndChange(new_color_);
}

std::pair<bool, float[4]> IPmovesControlScene::GetInputOldColor() {
    return GetValueAndChange(old_color_);
}

std::pair<bool, int> IPmovesControlScene::GetInputSlidingWindowSize() {
    return GetValueAndChange(sliding_window_size_);
}

std::pair<bool, int> IPmovesControlScene::GetInputAdvance() {
    return GetValueAndChange(advance_);
}

std::pair<bool, int> IPmovesControlScene::GetInputHilbertDegree() {
    return GetValueAndChange(hilbert_degree_);
}

std::pair<bool, size_t> IPmovesControlScene::GetInputMaxMemory() {
    auto max_memory_raw = GetValueAndChange(max_memory_);
    std::pair<bool, size_t> output;
    output.first = max_memory_raw.first;
    output.second = (1ull << 20) * max_memory_raw.second;  // because the user inputs it in megabytes
    return output;
}

void IPmovesControlScene::PauseButton() const {
    static const char* pause_text = "Pause";
    static const char* resume_text = "Resume";
    static auto get_button_text = [=, this] { return pause_.cur_value_ ? resume_text : pause_text; };
    if (ALIGNMENT_AT_BEGIN(ImGui::Button(get_button_text()))) {
        pause_.cur_value_ = !pause_.cur_value_;
    }
}

void IPmovesControlScene::SettingsButton() const {
    if (ALIGNMENT_AT_END(ImGui::Button("Settings"))) {
        ImGui::OpenPopup("IPmoves Settings");
    }
    if (ImGui::BeginPopup("IPmoves Settings")) {
        ImGui::Text("Size of sliding window: ");
        ImGui::InputInt("##Size of sliding window", &sliding_window_size_.cur_value_);

        ImGui::Text("Advance of sliding window: ");
        ImGui::InputInt("##Advance of sliding window", &advance_.cur_value_);

        ImGui::Text("Degree of Hilbert Curve: ");
        ImGui::SliderInt("##Degree of Hilbert Curve", &hilbert_degree_.cur_value_, 1, 10);

        ImGui::Text("Max memory for caching: ");
        ImGui::InputInt("##Max memory for caching", &max_memory_.cur_value_);

        ImGui::Text("Color of the newest element: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Rightmost color", new_color_.cur_value_, kColorEditFlags);

        ImGui::Text("Color of the oldest element: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Leftmost color", old_color_.cur_value_, kColorEditFlags);

        ImGui::EndPopup();
    }
}

void IPmovesControlScene::ProgressSlider() const {
    ALIGNMENT_AT_CENTER(ImGui::SliderFloat("##Progress", &progress_.cur_value_, 0.0f, 100.0f, "%.2f%%"));
}
