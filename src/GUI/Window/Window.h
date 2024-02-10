#pragma once

#include <stddef.h>
#include <utility>

class Window {
public:
	Window(int width, int height, const char* title);

	/* Returns whether the window is pending to be closed(the user has pressed alt + f4 or cross) */
	bool IsPendingClose() const;

	/* Makes the context of the window the current OpenGL contex. If should_init_opengl = true,
	then it also inits (finds the function pointers) OpenGL (via GLEW). The initialization should be done at least once. */
	void CaptureContext(bool should_init_opengl = false) const;

	/* Returns the current size of the window */
	[[nodiscard]] std::pair<int, int> GetWindowSize() const;

	/* Swaps rendering buffers, thereby showing updated buffer on the screen */
	void Render() const;

	/* Returns the underlaying GLFWwindow pointer, should be used only to syncronize with other rendering libraries */
	[[nodiscard]] struct GLFWwindow* GetInnerWindowPointer() const;

	~Window();
private:
	/* Set hints for GLFW */
	static void SetHints();

	static size_t kCounter_;

	class GLFWwindow* window_;
};