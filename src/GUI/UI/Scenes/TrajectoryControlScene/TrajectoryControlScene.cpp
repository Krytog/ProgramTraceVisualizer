#include "TrajectoryControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include "UI/Helpers/ValueWithPrev.h"

#define TrajectoryControlScene_InnerName "TrajectoryControl"

#define UnpackColor(x) x[0], x[1], x[2], x[3]
#define DoubleInit(x) \
    { x, x }

#define AssignColor(x, color)         \
    do {                              \
        for (int i = 0; i < 4; ++i) { \
            x[i] = color[i];          \
        }                             \
    } while (0)

namespace {
const constexpr ImGuiColorEditFlags kColorEditFlags =
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview |
    ImGuiWindowFlags_NoSavedSettings;

static const constexpr int kInitDimension = 2;
static const constexpr float kInitBeginColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const constexpr float kInitEndColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const constexpr int kInitHilbertDegree = 10;
}  // namespace

TrajectoryControlScene::TrajectoryControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size)
    : BasicScene(position, size, TrajectoryControlScene_InnerName),
      dimension_(DoubleInit(kInitDimension)),
      begin_color_(DoubleInit(UnpackColor(kInitBeginColor))),
      end_color_(DoubleInit(UnpackColor(kInitEndColor))),
      hilbert_degree_(DoubleInit(kInitHilbertDegree)) {
}

void TrajectoryControlScene::RenderInner() const {
    DimensionInfo();
    ImGui::SameLine();
    SettingsButton();
}

std::pair<bool, int> TrajectoryControlScene::GetInputDimension() {
    return GetValueAndChange(dimension_);
}

std::pair<bool, float[4]> TrajectoryControlScene::GetInputBeginColor() {
    return GetValueAndChange(begin_color_);
}

std::pair<bool, float[4]> TrajectoryControlScene::GetInputEndColor() {
    return GetValueAndChange(end_color_);
}

std::pair<bool, int> TrajectoryControlScene::GetInputHilbertDegree() {
    return GetValueAndChange(hilbert_degree_);
}

void TrajectoryControlScene::DimensionInfo() const {
    if (!real_dimension_) {
        ALIGNMENT_AT_BEGIN(ImGui::Text("Visualization is not ready"));
        return;
    }
    ALIGNMENT_AT_BEGIN(ImGui::Text("Dimension: %dD", real_dimension_));
}

void TrajectoryControlScene::SettingsButton() const {
    if (ALIGNMENT_AT_END(ImGui::Button("Settings"))) {
        ImGui::OpenPopup("Trajectory Settings");
    }
    if (ImGui::BeginPopup("Trajectory Settings")) {
        ImGui::Text("Size degree: ");
        ImGui::SliderInt("##Precision degree: ", &hilbert_degree_.cur_value_, 1, 10);

        ImGui::Text("Color of the first elements: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Leftmost color", begin_color_.cur_value_, kColorEditFlags);

        ImGui::Text("Color of the last elements: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Rightmost color", end_color_.cur_value_, kColorEditFlags);

        ImGui::Text("Dimension: ");
        ImGui::SliderInt("##Dimension: ", &dimension_.cur_value_, 2, 3);

        ImGui::EndPopup();
    }
}


TrajectoryControlScene::TrajectoryControlState TrajectoryControlScene::GetState() const {
    TrajectoryControlState state{.dimension = dimension_.cur_value_,
                          .begin_color = {UnpackColor(begin_color_.cur_value_)},
                          .end_color = {UnpackColor(end_color_.cur_value_)},
                          .hilbert_degree = hilbert_degree_.cur_value_};
    return state;
}

void TrajectoryControlScene::ResetState() {
    dimension_ = DoubleInit(kInitDimension);
    begin_color_ = DoubleInit(UnpackColor(kInitBeginColor));
    end_color_ = DoubleInit(UnpackColor(kInitEndColor));
    hilbert_degree_ = DoubleInit(kInitHilbertDegree);
}

void TrajectoryControlScene::SetRealDimension(int real_dimension) {
    real_dimension_ = real_dimension;
}