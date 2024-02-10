#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>
#include <UI/UIManager/UIManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/Primitives/Cube/Cube.h>

int main(int argc, char** argv) {
	Window window(1000, 800, "Test window!");
	window.CaptureContext(true);

	const int FPS = 144;
	const double frametime = 1.0f / FPS;
	LightTimer time;

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr);

    glm::mat4 matrix = glm::mat4(1.0f);

    std::shared_ptr<Cube> cube = std::make_shared<Cube>();

    ui_manager.GetViewScene().AddObject(cube);

    glEnable(GL_DEPTH_TEST);

    LightTimer run_timer;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    std::shared_ptr<Cube> cube2 = std::make_shared<Cube>();

    ui_manager.GetViewScene().AddObject(cube2);

    LightTimer run_timer2;

	while (!window.IsPendingClose()) {
		time.ResetTime();

        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto transform_matrix = glm::rotate(matrix, static_cast<float>(run_timer.EvaluateTime()), glm::vec3(0.3f, 1.0f, 0.0f));
            transform_matrix = glm::translate(transform_matrix, 2.5f * glm::vec3(std::sin(run_timer.EvaluateTime()), std::cos(run_timer.EvaluateTime()), 0.0f));
            auto view_matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -5.0f));
            glm::mat4 projection_matrix = glm::perspective(45.0f, static_cast<float>(size.first) / size.second, 0.1f, 100.0f);
            auto final_matrix = projection_matrix * view_matrix * transform_matrix;
            cube->SetTransform(final_matrix);
        }
        {
            auto transform_matrix = glm::rotate(matrix, static_cast<float>(run_timer2.EvaluateTime()), glm::vec3(0.3f, 1.0f, 0.0f));
            transform_matrix = glm::translate(transform_matrix, 2.5f * glm::vec3(std::sin(run_timer2.EvaluateTime()), std::cos(run_timer2.EvaluateTime()), 0.0f));
            auto view_matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -5.0f));
            glm::mat4 projection_matrix = glm::perspective(45.0f, static_cast<float>(size.first) / size.second, 0.1f, 100.0f);
            auto final_matrix = projection_matrix * view_matrix * transform_matrix;
            cube2->SetTransform(final_matrix);
        }
        

        ui_manager.DrawUI();

		window.Render();
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * (frametime - time.EvaluateTime()))));
	}
	return 0;
}