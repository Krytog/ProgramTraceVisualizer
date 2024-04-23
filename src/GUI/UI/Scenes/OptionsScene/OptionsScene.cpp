#include "OptionsScene.h"

#include <imgui.h>
#include <portable-file-dialogs.h>
#include <optional>

#define OptionsScene_InnerName "Options"

namespace {
static constexpr const ImGuiWindowFlags kWidgetHint =
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

std::optional<std::string> GetFilename() {
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                            {"Trace Files (.trace)", "*.trace", "All Files", "*"}, pfd::opt::none);
    if (f.result().empty()) {
        return std::nullopt;
    }
    return f.result().back();
}
}  // namespace

OptionsScene::OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size)
    : BasicScene(position, size, OptionsScene_InnerName) {
    widget_hint_ = kWidgetHint;
}

void OptionsScene::RenderInner() const {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            OpenFileButton();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Mode")) {
            if (ImGui::MenuItem("IP move mode")) {
                std::cout << "dummy" << std::endl;
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMenuBar();
    ImGui::ShowDemoWindow();
}

void OptionsScene::OpenFileButton() const {
    if (ImGui::MenuItem("Open File")) {
        const auto filename = GetFilename();
        if (filename.has_value()) {
            std::cout << *filename << std::endl;
        } else {
            std::cout << "user canceled" << std::endl;
        }
    }
}