#include "W2VControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include "UI/Helpers/ValueWithPrev.h"

#define W2VControlScene_InnerName "W2VControl"

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
static const constexpr float kInitColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const constexpr int kInitNeighbours = 15;
static const constexpr int kInitEpochs = 250;
static const constexpr int kInitCells = 1024;
static const constexpr float kInitMinDist = 0.1f;
static const constexpr bool kInitIsPendingRecalculation = false;
}  // namespace

W2VControlScene::W2VControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size)
    : BasicScene(position, size, W2VControlScene_InnerName),
      dimension_(kInitDimension),
      color_(UnpackColor(kInitColor)),
      neighbours_(kInitNeighbours),
      epochs_(kInitEpochs),
      cells_(kInitCells),
      min_dist_(kInitMinDist),
      is_pending_recalculation_(DoubleInit(kInitIsPendingRecalculation)) {
}

void W2VControlScene::RenderInner() const {
    DimensionInfo();
    ImGui::SameLine();
    CalculateButton();
    ImGui::SameLine();
    SettingsButton();
}

int W2VControlScene::GetInputDimension() {
    return dimension_;
}

const float* W2VControlScene::GetInputColor() {
    return color_;
}

int W2VControlScene::GetInputNeighbours() {
    return neighbours_;
}

int W2VControlScene::GetInputEpochs() {
    return epochs_;
}

int W2VControlScene::GetInputCells() {
    return cells_;
}

float W2VControlScene::GetInputMinDist() {
    return min_dist_;
}

void W2VControlScene::DimensionInfo() const {
    if (!real_dimension_) {
        ALIGNMENT_AT_BEGIN(ImGui::Text("Visualization is not ready"));
        return;
    }
    ALIGNMENT_AT_BEGIN(ImGui::Text("Dimension: %dD", real_dimension_));
}

void W2VControlScene::SettingsButton() const {
    if (ALIGNMENT_AT_END(ImGui::Button("Settings"))) {
        ImGui::OpenPopup("W2V Settings");
    }
    if (ImGui::BeginPopup("W2V Settings")) {
        ImGui::SeparatorText("Runtime settings");

        ImGui::Text("Color: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Color", color_, kColorEditFlags);

        ImGui::Text("Cells: ");
        ImGui::InputInt("##Cells", &cells_);

        ImGui::SeparatorText("Recalculation settings");

        ImGui::Text("Output dimension: ");
        ImGui::SliderInt("##Output dimension", &dimension_, 2, 3);

        ImGui::Text("Neighbours: ");
        ImGui::InputInt("##Neighbours", &neighbours_);

        ImGui::Text("Epochs: ");
        ImGui::InputInt("##Epochs", &epochs_);

        ImGui::Text("Min dist: ");
        ImGui::InputFloat("##Min dist", &min_dist_);

        ImGui::EndPopup();
    }
}

std::pair<bool, bool> W2VControlScene::IsPendingRecalculation() const {
    return GetValueAndChange(is_pending_recalculation_);
}

void W2VControlScene::CalculateButton() const {
    if (ALIGNMENT_AT_CENTER(ImGui::Button("Calculate visualization"))) {
        is_pending_recalculation_.cur_value_ = true;
    }
}

void W2VControlScene::OnRecalculationFinished() {
    is_pending_recalculation_.cur_value_ = false;
}

W2VControlScene::W2VControlState W2VControlScene::GetState() const {
    W2VControlState state{.dimension = dimension_,
                          .color = {UnpackColor(color_)},
                          .neighbours = neighbours_,
                          .epochs = epochs_,
                          .cells = cells_,
                          .min_dist = min_dist_};
    return state;
}

void W2VControlScene::ResetState() {
    dimension_ = kInitDimension;
    AssignColor(color_, kInitColor);
    neighbours_ = kInitNeighbours;
    epochs_ = kInitEpochs;
    cells_ = kInitCells;
    min_dist_ = kInitMinDist;
    is_pending_recalculation_ = DoubleInit(kInitIsPendingRecalculation);
}

void W2VControlScene::SetRealDimension(int real_dimension) {
    real_dimension_ = real_dimension;
}