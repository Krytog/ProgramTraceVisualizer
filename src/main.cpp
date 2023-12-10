#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Graphics/Shader/Shader.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>
#include <Graphics/VertexObjects/VertexObjectIndexed.h>
#include <UI/UIManager/UIManager.h>

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main(int argc, char** argv) {
	Window window(1000, 800, "Test window!");
	window.CaptureContext(true);

	const int FPS = 144;
	const double frametime = 1.0f / FPS;
	LightTimer time;

    Shader::ShaderConfig config;
    config.VertexShaderSourceType = Shader::SourceType::STRING;
    config.VertexShaderSource = vertexShaderSource;
    config.FragmentShaderSourceType = Shader::SourceType::STRING;
    config.FragmentShaderSource = fragmentShaderSource;
    config.GeometryShaderSourceType = Shader::SourceType::NONE;
    config.GeometryShaderSource = "aboba/none.nothing";

    Shader shader(config);

    GLfloat vertices[] = { -0.3, -0.3, 0.0, 0.3, -0.3, 0.0, 0.3, 0.3, 0.0, -0.3, 0.3, 0.0 };

    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    VertexObjectIndexed::ArgPack arg_pack;
    arg_pack.data = vertices;
    arg_pack.data_size = sizeof(vertices);
    arg_pack.args_per_vertex = 3;
    arg_pack.indices = indices;
    arg_pack.indices_size = sizeof(indices);
    arg_pack.memory_mode = VertexObject::MemoryMode::STATIC;

    VertexObjectIndexed figure(arg_pack);

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr);

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