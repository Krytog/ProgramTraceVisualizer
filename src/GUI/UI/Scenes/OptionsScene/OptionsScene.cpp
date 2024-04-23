#include "OptionsScene.h"

#include <imgui.h>
#include <portable-file-dialogs.h>
#include <optional>

#define OptionsScene_InnerName "Options"

namespace {
	std::optional<std::string> GetFilename() {
		auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                            { "Trace Files (.trace)", "*.trace",
                              "All Files", "*" },
                            pfd::opt::none);
		if (f.result().empty()) {
			return std::nullopt;
		}
		return f.result().back();
	}
}

OptionsScene::OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size) :
	BasicScene(position, size, OptionsScene_InnerName) {}

void OptionsScene::RenderInner() const {
	OpenFileButton();
	ImGui::SameLine();
	ImGui::Button("Option2");
	ImGui::SameLine();
	ImGui::Button("Option3");
	ImGui::SameLine();
	if (ImGui::Button("Option4")) {
		
	}
}

void OptionsScene::OpenFileButton() const {
	if (ImGui::Button("Open File")) {
		const auto filename = GetFilename();
		if (filename.has_value()) {
			std::cout << *filename << std::endl;
		} else {
			std::cout << "user canceled" << std::endl;
		}	
	}
}