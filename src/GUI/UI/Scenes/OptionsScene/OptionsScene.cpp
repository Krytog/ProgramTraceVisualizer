#include "OptionsScene.h"

#include <imgui.h>
#include <portable-file-dialogs.h>
#include <optional>
#include <filesystem>

#define OptionsScene_InnerName "Options"

namespace {
static constexpr const ImGuiWindowFlags kWidgetHint =
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

std::optional<std::string> GetFilename() {
    auto f = pfd::open_file("Choose files to read", std::filesystem::current_path().string(),
                            {"Trace Files (.trace)", "*.trace", "All Files", "*"}, pfd::opt::none);
    if (f.result().empty()) {
        return std::nullopt;
    }
    return f.result().back();
}

enum class Mode { IP, W2V, TRAJECTORY };

}  // namespace

OptionsScene::OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size)
    : BasicScene(position, size, OptionsScene_InnerName) {
    widget_hint_ = kWidgetHint;
}

void OptionsScene::RenderInner() const {
    if (ImGui::BeginMenuBar()) {
        FileMenu();
        ModeMenu();
    }
    ImGui::EndMenuBar();
}

void OptionsScene::OpenFileButton() const {
    if (ImGui::MenuItem("Open File")) {
        const auto filename = GetFilename();
        if (filename.has_value()) {
            openfile_callback_(*filename);
        }
    }
}

void OptionsScene::FileMenu() const {
    if (ImGui::BeginMenu("File")) {
        OpenFileButton();
        ImGui::EndMenu();
    }
}

void OptionsScene::ModeMenu() const {
    if (ImGui::BeginMenu("Mode")) {
        static Mode mode = Mode::IP;
        if (ImGui::MenuItem("IP move mode", nullptr, mode == Mode::IP)) {
            if (mode != Mode::IP) {
                mode = Mode::IP;
                mode_ip_callback_();
            }
        }
        if (ImGui::MenuItem("V2W mode", nullptr, mode == Mode::W2V)) {
            if (mode != Mode::W2V) {
                mode = Mode::W2V;
                mode_w2v_callback_();
            }
        }
        if (ImGui::MenuItem("Trajectory mode", nullptr, mode == Mode::TRAJECTORY)) {
            if (mode != Mode::TRAJECTORY) {
                mode = Mode::TRAJECTORY;
                mode_trajectory_callback_();
            }
        }
        ImGui::EndMenu();
    }
}

void OptionsScene::SetOpenFileCallback(OpenFileCallback callback) {
    openfile_callback_ = std::move(callback);
}

void OptionsScene::SetModeIPCallback(ModeCallback callback) {
    mode_ip_callback_ = std::move(callback);
}

void OptionsScene::SetModeW2VCallback(ModeCallback callback) {
    mode_w2v_callback_ = std::move(callback);
}

void OptionsScene::SetModeTrajectoryCallback(ModeCallback callback) {
    mode_trajectory_callback_ = std::move(callback);
}
