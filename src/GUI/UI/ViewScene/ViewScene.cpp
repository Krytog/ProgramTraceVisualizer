#include "ViewScene.h"

#include <Graphics/Primitives/IRenderable.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <Utils/LightTimer/LightTimer.h>

ViewScene::ViewScene(const std::pair<float, float>& position, const std::pair<float, float>& size): position_(position), size_(size), 
																									render_buffer_(size.first, size.second) {}

bool ViewScene::AddObject(const std::shared_ptr<IRenderable>& object) {
	bool output = true;
	if (renderable_objects_.contains(object)) {
		output = false;
	}
	renderable_objects_.insert(object);
	return output;
}

bool ViewScene::RemoveObject(const std::shared_ptr<IRenderable>& object) {
	auto iterator = renderable_objects_.find(object);
	if (iterator == renderable_objects_.end()) {
		return false;
	}
	renderable_objects_.erase(iterator);
	return true;
}

void ViewScene::Render() const {
	RenderObjects();
	RenderUI();
}

void ViewScene::RenderObjects() const {
	render_timer_.ResetTime();
	render_buffer_.Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	render_buffer_.Clear();
	for (const auto& renderable : renderable_objects_) {
		renderable->Render();
	}
	render_buffer_.Unbind();
}

void ViewScene::RenderUI() const {
	ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
								  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::SetWindowPos(ImVec2(position_.first, position_.second), 0);
	ImGui::SetWindowSize(ImVec2(size_.first, size_.second), 0);
	ImGui::GetStyle().WindowBorderSize = 0.0f;

	const double current_frametime = render_timer_.EvaluateTime();
	const unsigned long long current_framerate = 1.0 / current_frametime;
	ImGui::Text("View \t FrameTime: %.8f \t FPS: %llu", current_frametime, current_framerate);

	const GLuint texture_id = render_buffer_.GetTextureID();
	const long long unsigned texture_id_upcast = static_cast<long long unsigned>(texture_id); // upcasting GLuint to a bigger intenger type for safery reasons
	ImGui::Image(reinterpret_cast<void*>(texture_id_upcast), ImVec2{ size_.first, size_.second }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
}

void ViewScene::Resize(const std::pair<float, float>& new_size) {
	if (new_size.first != size_.first || new_size.second != size_.second) {
		render_buffer_.Resize(new_size.first, new_size.second);
		size_ = new_size;
	}
}