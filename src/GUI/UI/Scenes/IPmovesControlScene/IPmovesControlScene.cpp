#include "IPmovesControlScene.h"

#include <imgui.h>
#include <UI/Helpers/Alignment.h>

#define IPmovesControlScene_InnerName "IPmovesControl"

IPmovesControlScene::IPmovesControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size) :
	BasicScene(position, size, IPmovesControlScene_InnerName) {}

void IPmovesControlScene::RenderInner() const {
	static float f1 = 12.3f;
    ALIGNMENT_AT_BEGIN(ImGui::Button("Pause"));
	ImGui::SameLine();
    ALIGNMENT_AT_CENTER(ImGui::SliderFloat("##Progress", &f1, 0.0f, 100.0f, "%.2f%%"));
    ImGui::SameLine();
    ALIGNMENT_AT_END(ImGui::Button("Settings"));
    ImGui::SameLine();
    //ImGui::ShowDemoWindow();
}