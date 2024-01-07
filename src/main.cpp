#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Graphics/Shader/Shader.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>
#include <Graphics/VertexObjects/VertexObjectIndexed.h>
#include <UI/UIManager/UIManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

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

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 matrix = glm::mat4(1.0f);

    matrix = glm::translate(matrix, glm::vec3(1.0f, 0.0f, 0.0f));
    vec = matrix * vec;

	while (!window.IsPendingClose()) {
		time.ResetTime();

        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        
        figure.Draw(VertexObject::DrawMode::TRIANGLES);

        ui_manager.DrawUI();

		window.Render();
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * (frametime - time.EvaluateTime()))));
	}
	return 0;
}