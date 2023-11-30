#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include "Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

#define GLFW_INIT_FAILED_MESSAGE "GLFW initializtion failed!"
#define GLFW_WINDOW_INIT_FAILED_MESSAGE "GLFW WINDOW initializtion failed!"
#define GLEW_INIT_FAILED_MESSAGE "GLEW initializtion failed!"

size_t Window::kCounter_ = 0;

Window::Window(int width, int height, const char* title) {
	if (!kCounter_) {
		if (!glfwInit()) {
			throw std::runtime_error(GLFW_INIT_FAILED_MESSAGE);
		}
	}
	SetHints();
	window_ = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window_) {
		throw std::runtime_error(GLFW_WINDOW_INIT_FAILED_MESSAGE);
	}
	++kCounter_;
}

Window::~Window() {
	--kCounter_;
	glfwDestroyWindow(window_);
	if (!kCounter_) {
		glfwTerminate();
	}
}

bool Window::IsPendingClose() const {
	return glfwWindowShouldClose(window_);
}

void Window::CaptureContext(bool should_init_opengl) const {
	glfwMakeContextCurrent(window_);
	if (should_init_opengl) {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error(GLEW_INIT_FAILED_MESSAGE);
		}
	}
}

void Window::SetHints() {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

std::pair<int, int> Window::GetWindowSize() const {
	int width;
	int height;
	glfwGetFramebufferSize(window_, &width, &height);
	return std::make_pair(width, height);
}

void Window::Render() const {
	glfwSwapBuffers(window_);
}