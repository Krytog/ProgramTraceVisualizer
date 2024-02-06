#include "ViewScene.h"

#include <Primitives/IRenderable.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

ViewScene::ViewScene() = default;

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
	MarkUpUI();
	RenderAll();
}

void ViewScene::RenderAll() const {
	for (const auto& renderable : renderable_objects_) {
		renderable->Render();
	}
}

void ViewScene::MarkUpUI() const {
	ImGui::Begin("View");
	ImGui::End();
}