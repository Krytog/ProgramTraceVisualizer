#pragma once

#include <UI/ViewScene/ViewScene.h>

class UIManager {
public:
	UIManager(struct GLFWwindow* glfw_window);

	~UIManager();

	/* Draws all the UI into the render buffer */
	void DrawUI();

	/* Returns the reference to the ViewScene of the UI */
	ViewScene& GetViewScene();

private:
	/* Inits underlying ImGui library that is used for UI handling. Should be called in the c-tor */
	void InitImGui(struct GLFWwindow* glfw_window) const;

	/* This functions is called inside DrawUI to create the UI that will be rendered */
	void MakeUIMarkUp();

	ViewScene view_scene_;
};