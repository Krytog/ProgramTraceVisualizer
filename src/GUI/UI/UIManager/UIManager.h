#pragma once

class UIManager {
public:
	UIManager(struct GLFWwindow* glfw_window);

	~UIManager();

	/* Draws all the UI into the render buffer */
	void DrawUI();

private:
	/* Inits underlying ImGui library that is used for UI handling. Should be called in the c-tor */
	void InitImGui(struct GLFWwindow* glfw_window) const;

	/* This functions is called inside DrawUI to create the UI that will be rendered */
	void MakeUIMarkUp();

	float color_;
};