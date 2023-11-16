#pragma once

#include <stddef.h>

class Window {
public:
	Window(int width, int height, const char* title);

	bool ShouldWindowBeClosed() const;

	~Window();
private:
	static size_t kCounter_;

	class GLFWwindow* window_;
};