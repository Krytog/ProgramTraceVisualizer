#include "IPmovesControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include "UI/Helpers/ValueWithPrev.h"

#define IPmovesControlScene_InnerName "IPmovesControl"

#define UnpackColor(x) x[0], x[1], x[2], x[3]
#define DoubleInit(x) \
    { x, x }

namespace {
const constexpr ImGuiColorEditFlags kColorEditFlags =
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;

static const constexpr float kInitProgress = 0.0f;
static const constexpr bool kInitPause = true;
static const constexpr float kInitNewColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const constexpr float kInitOldColor[4] = {0.0f, 0.0f, 1.0f, 1.0f};
static const constexpr int kInitSlidingWindowSize = 10;
static const constexpr int kInitAdvance = 1;
static const constexpr int kInitHilbertDegree = 5;
static const constexpr int kInitMaxMemory = 1;
}  // namespace

IPmovesControlScene::IPmovesControlScene(const std::pair<float, float>& position,
                                         const std::pair<float, float>& size)
    : BasicScene(position, size, IPmovesControlScene_InnerName),
      progress_(DoubleInit(kInitProgress)),
      pause_(DoubleInit(kInitPause)),
      new_color_(DoubleInit(UnpackColor(kInitNewColor))),
      old_color_(DoubleInit(UnpackColor(kInitOldColor))),
      sliding_window_size_(DoubleInit(kInitSlidingWindowSize)),
      advance_(DoubleInit(kInitAdvance)),
      hilbert_degree_(DoubleInit(kInitHilbertDegree)),
      max_memory_(DoubleInit(kInitMaxMemory)) {
}

void IPmovesControlScene::RenderInner() const {
    PauseButton();
    ImGui::SameLine();
    ProgressSlider();
    ImGui::SameLine();
    SettingsButton();
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

        ImGui::Text("Max memory for caching (Mb): ");
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
    static const constexpr float kSliderMin = 0.0f;
    static const constexpr float kSliderMax = 100.0f;
    ALIGNMENT_AT_CENTER(
        ImGui::SliderFloat("##Progress", &progress_.cur_value_, kSliderMin, kSliderMax, "%.2f%%"));
}

IPmovesControlScene::IPmovesControlState IPmovesControlScene::GetState() const {
    IPmovesControlState state{
        .progress = progress_.cur_value_ / 100,  // as user inputs it in percents
        .paused = pause_.cur_value_,
        .new_color = {UnpackColor(new_color_.cur_value_)},
        .old_color = {UnpackColor(old_color_.cur_value_)},
        .sliding_window_size = sliding_window_size_.cur_value_,
        .advance = advance_.cur_value_,
        .hilbert_degree = hilbert_degree_.cur_value_,
        .max_memory = (1ull << 20) * max_memory_.cur_value_,  // as user inputs it in megabytes
    };
    return state;
}

void IPmovesControlScene::ResetState() {
    progress_ = DoubleInit(kInitProgress);
    pause_ = DoubleInit(kInitPause);
    new_color_ = DoubleInit(UnpackColor(kInitNewColor));
    old_color_ = DoubleInit(UnpackColor(kInitOldColor));
    sliding_window_size_ = DoubleInit(kInitSlidingWindowSize);
    advance_ = DoubleInit(kInitAdvance);
    hilbert_degree_ = DoubleInit(kInitHilbertDegree);
    max_memory_ = DoubleInit(kInitMaxMemory);
}