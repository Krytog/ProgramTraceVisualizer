#include "ViewScene.h"

#include <Graphics/Primitives/IRenderable.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

ViewScene::ViewScene(GLint width, GLint height): render_buffer_(width, height) {}

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

void ViewScene::Render() {
	RenderObjects();
	RenderUI();
}

void ViewScene::RenderObjects() const {
	render_buffer_.Bind();
	render_buffer_.Clear();
	for (const auto& renderable : renderable_objects_) {
		renderable->Render();
	}
	render_buffer_.Unbind();
}

void ViewScene::RenderUI() {
	ImGui::Begin("View");
	auto current_size = TryResize();
	const GLuint texture_id = render_buffer_.GetTextureID();
	const ImVec2 frame_size(static_cast<float>(current_size.first), static_cast<float>(current_size.second));
	ImGui::Image(reinterpret_cast<void*>(texture_id), frame_size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
}

std::pair<GLint, GLint> ViewScene::TryResize() {
	auto outer_size = ImGui::GetContentRegionAvail();
	auto inner_size = render_buffer_.GetSize();
	if (outer_size.x != inner_size.first || outer_size.y != inner_size.second) {
		render_buffer_.Resize(outer_size.x, outer_size.y);
	}
	return { outer_size.x, outer_size.y };
}