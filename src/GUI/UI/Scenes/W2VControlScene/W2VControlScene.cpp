#include "W2VControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>
#include "UI/Helpers/ValueWithPrev.h"

#define W2VControlScene_InnerName "W2VControl"

#define UnpackColor(x) x[0], x[1], x[2], x[3]
#define DoubleInit(x) \
    { x, x }

namespace {
const constexpr ImGuiColorEditFlags kColorEditFlags =
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiWindowFlags_NoSavedSettings;

static const constexpr int kInitDimension = 2;
static const constexpr float kInitColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const constexpr int kInitNeighbours = 15;
static const constexpr int kInitEpochs = 300;
static const constexpr int kInitCells = 1024;
static const constexpr float kInitMinDist = 0.1f;
static const constexpr bool kInitIsPendingRecalculation = false;
}  // namespace

W2VControlScene::W2VControlScene(const std::pair<float, float>& position,
                                         const std::pair<float, float>& size)
    : BasicScene(position, size, W2VControlScene_InnerName),
      dimension_(DoubleInit(kInitDimension)),
      color_(DoubleInit(UnpackColor(kInitColor))),
      neighbours_(DoubleInit(kInitNeighbours)),
      epochs_(DoubleInit(kInitEpochs)),
      cells_(DoubleInit(kInitCells)),
      min_dist_(DoubleInit(kInitMinDist)),
      is_pending_recalculation_(DoubleInit(kInitIsPendingRecalculation)) {
}

void W2VControlScene::RenderInner() const {
    DimensionInfo();
    ImGui::SameLine();
    CalculateButton();
    ImGui::SameLine();
    SettingsButton();
}

std::pair<bool, int> W2VControlScene::GetInputDimension() {
    return GetValueAndChange(dimension_);
}

std::pair<bool, W2VControlScene::Color> W2VControlScene::GetInputColor() {
    return GetValueAndChange(color_);
}

std::pair<bool, int> W2VControlScene::GetInputNeighbours() {
    return GetValueAndChange(neighbours_);
}

std::pair<bool, int> W2VControlScene::GetInputEpochs() {
    return GetValueAndChange(epochs_);
}

std::pair<bool, int> W2VControlScene::GetInputCells() {
    return GetValueAndChange(cells_);
}

std::pair<bool, float> W2VControlScene::GetInputMinDist() {
    return GetValueAndChange(min_dist_);
}


void W2VControlScene::DimensionInfo() const {
    ALIGNMENT_AT_BEGIN(ImGui::Text("Dimension: %dD", dimension_.cur_value_));
}

void W2VControlScene::SettingsButton() const {
    if (ALIGNMENT_AT_END(ImGui::Button("Settings"))) {
        ImGui::OpenPopup("W2V Settings");
    }
    if (ImGui::BeginPopup("W2V Settings")) {
        ImGui::Text("Output dimension: ");
        ImGui::SliderInt("##Output dimension", &dimension_.cur_value_, 1, 2);

        ImGui::Text("Color: ");
        ImGui::SameLine();
        ImGui::ColorEdit4("##Color", color_.cur_value_, kColorEditFlags);

        ImGui::Text("Cells: ");
        ImGui::InputInt("##Cells", &cells_.cur_value_);

        ImGui::Text("Neighbours: ");
        ImGui::InputInt("##Neighbours", &neighbours_.cur_value_);

        ImGui::Text("Epochs: ");
        ImGui::InputInt("##Epochs", &epochs_.cur_value_);

        ImGui::Text("Min dist: ");
        ImGui::InputFloat("##Min dist", &min_dist_.cur_value_);

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

W2VControlScene::W2VControlState W2VControlScene::GetState() const {
    W2VControlState state{
        .dimension = dimension_.cur_value_,
        .color = {UnpackColor(color_.cur_value_)},
        .neighbours = neighbours_.cur_value_,
        .epochs = epochs_.cur_value_,
        .cells = cells_.cur_value_,
        .min_dist = min_dist_.cur_value_,
        .is_pending_recalculation = is_pending_recalculation_.cur_value_,
    };
    return state;
}

void W2VControlScene::ResetState() {
    dimension_ = DoubleInit(kInitDimension);
    color_ = DoubleInit(UnpackColor(kInitColor));
    neighbours_ = DoubleInit(kInitNeighbours);
    epochs_ = DoubleInit(kInitEpochs);
    cells_ = DoubleInit(kInitCells);
    min_dist_ = DoubleInit(kInitMinDist);
    is_pending_recalculation_ = DoubleInit(kInitIsPendingRecalculation);
}