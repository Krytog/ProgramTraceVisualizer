#pragma once

#include <utility>
#include <string>

class BasicScene {
public:
	BasicScene(const std::pair<float, float>& position, const std::pair<float, float>& size, const std::string& name);

	/* Renders the scene */
	void Render() const;

protected:
	/* Prepares the scene for rendering: moves it to the correct position and resizes */
	void MoveAndResize() const;

	/* Renders the actual scene, should be overrided in derived classes */
	virtual void RenderInner() const = 0;

	/* It will be used as ImVec2 from DearImGui, but it's preferable to avoid leaking DearImGui into the headers */
	std::pair<float, float> position_;
	std::pair<float, float> size_;
	
	/* It will be used as ImGuiWindowFlags, but it's still preferable to avoid leaking DearImGui into the headers */
	int widget_hint_;

	std::string name_;
};