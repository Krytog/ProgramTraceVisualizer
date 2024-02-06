#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Graphics/Shader/Shader.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>
#include <Graphics/VertexObjects/VertexObjectIndexed.h>
#include <UI/UIManager/UIManager.h>
#include <UI/ViewScene/ViewScene.h>
#include <Graphics/RenderBuffer/RenderBuffer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <Graphics/Primitives/Cube/Cube.h>

const GLchar* vertexShaderSource = "../src/Resources/Shaders/default.vert";
const GLchar* fragmentShaderSource = "../src/Resources/Shaders/default.frag";

int main(int argc, char** argv) {
	Window window(1000, 800, "Test window!");
	window.CaptureContext(true);

	const int FPS = 144;
	const double frametime = 1.0f / FPS;
	LightTimer time;

    Shader::ShaderConfig config;
    config.VertexShaderSourceType = Shader::SourceType::FILE;
    config.VertexShaderSource = vertexShaderSource;
    config.FragmentShaderSourceType = Shader::SourceType::FILE;
    config.FragmentShaderSource = fragmentShaderSource;
    config.GeometryShaderSourceType = Shader::SourceType::NONE;
    config.GeometryShaderSource = "aboba/none.nothing";

    Shader shader(config);

    GLfloat vertices[] = { -0.3, -0.3, 0.0, 1.0, 1.0,
                            0.3, -0.3, 0.0, 1.0, 0.0, 
                            0.3, 0.3, 0.0, 0.0, 1.0, 
                            -0.3, 0.3, 0.0, 0.0, 0.0 };

    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    VertexObjectIndexed::ArgPack arg_pack;
    arg_pack.data = vertices;
    arg_pack.data_size = sizeof(vertices);
    arg_pack.args_per_vertex = 5;
    arg_pack.indices = indices;
    arg_pack.indices_size = sizeof(indices);
    arg_pack.memory_mode = VertexObject::MemoryMode::STATIC;

    VertexObjectIndexed figure(arg_pack);
    figure.SetAttribute(0, 3, static_cast<GLvoid*>(0));
    figure.SetAttribute(1, 2, reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr);
    ViewScene scene;

    glm::mat4 matrix = glm::mat4(1.0f);

    Cube cube;

    RenderBuffer render_buffer(1000, 800);

    glEnable(GL_DEPTH_TEST);

    LightTimer run_timer;
	while (!window.IsPendingClose()) {
		time.ResetTime();

        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto transform_matrix = glm::rotate(matrix, static_cast<float>(run_timer.EvaluateTime()), glm::vec3(0.3f, 1.0f, 0.0f));
        transform_matrix = glm::translate(transform_matrix, 2.5f * glm::vec3(std::sin(run_timer.EvaluateTime()), std::cos(run_timer.EvaluateTime()), 0.0f));
        auto view_matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -5.0f));
        glm::mat4 projection_matrix = glm::perspective(45.0f, static_cast<float>(size.first) / size.second, 0.1f, 100.0f);
        auto final_matrix = projection_matrix * view_matrix * transform_matrix;
        cube.SetTransform(final_matrix);
        render_buffer.Bind();
        render_buffer.Clear();
        cube.Render();
        render_buffer.Unbind();

        ui_manager.DrawUI(render_buffer.GetTextureID());

		window.Render();
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * (frametime - time.EvaluateTime()))));
	}
	return 0;
}