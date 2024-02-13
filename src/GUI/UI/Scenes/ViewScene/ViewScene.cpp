#include "ViewScene.h"

#include <Graphics/Primitives/IRenderable.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <Utils/LightTimer/LightTimer.h>

#define ViewScene_InnerName "View"

ViewScene::ViewScene(const std::pair<float, float>& position, const std::pair<float, float>& size): BasicScene(position, size, ViewScene_InnerName),
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

void ViewScene::RenderInner() const {
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
	const double current_frametime = render_timer_.EvaluateTime();
	const unsigned long long current_framerate = 1.0 / current_frametime;
	ImGui::Text(ViewScene_InnerName " \t FrameTime: %.8f \t FPS: %llu", current_frametime, current_framerate);
	const float text_height_offset = 30.0f; // it's magic constant 

	const GLuint texture_id = render_buffer_.GetTextureID();
	const long long unsigned texture_id_upcast = static_cast<long long unsigned>(texture_id); // upcasting GLuint to a bigger intenger type for safety reasons
	ImGui::Image(reinterpret_cast<void*>(texture_id_upcast), ImVec2{ size_.first, size_.second - text_height_offset }, 
				 ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
}

void ViewScene::Resize(const std::pair<float, float>& new_size) {
	if (new_size.first != size_.first || new_size.second != size_.second) {
		render_buffer_.Resize(new_size.first, new_size.second);
		size_ = new_size;
	}
}