#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/window.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>



int main(int argc, char** argv) {
	Window window(640, 480, "Test window!");

	const int FPS = 144;
	const double frametime = 1.0f / FPS;
	LightTimer time;

	while (!window.ShouldWindowBeClosed()) {
		time.ResetTime();

		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * (frametime - time.EvaluateTime()))));
	}
	return 0;
}