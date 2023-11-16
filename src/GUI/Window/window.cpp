#include "window.h"
#include <GLFW/glfw3.h>

#include <stdexcept>

#define GLFW_INIT_FAILED_MESSAGE "GLFW initializtion failed!"
#define GLFW_WINDOW_INIT_FAILED_MESSAGE "GLFW WINDOW initializtion failed!"

size_t Window::kCounter_ = 0;

Window::Window(int width, int height, const char* title) {
	if (!kCounter_) {
		if (!glfwInit())
		{
			throw std::runtime_error(GLFW_INIT_FAILED_MESSAGE);
		}
	}
	window_ = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window_) {
		throw std::runtime_error(GLFW_WINDOW_INIT_FAILED_MESSAGE);
	}
	++kCounter_;
}

Window::~Window() {
	--kCounter_;
	if (!kCounter_) {
		glfwTerminate();
	}
}

bool Window::ShouldWindowBeClosed() const {
	return glfwWindowShouldClose(window_);
}