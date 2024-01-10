#include "Cube.h"

static constexpr const GLfloat kVerticies[] = {
	0.5f, -0.5f, -0.5f, 113 / 255.0f, 28 / 255.0f, 145 / 255.0f,
	0.5f, 0.5f, -0.5f, 113 / 255.0f, 28 / 255.0f, 145 / 255.0f,
	-0.5f, 0.5f, -0.5f, 113 / 255.0f, 28 / 255.0f, 145 / 255.0f,
	-0.5f, -0.5f, -0.5f, 113 / 255.0f, 28 / 255.0f, 145 / 255.0f,
	0.5f, -0.5f, 0.5f, 10 / 255.0f, 189 / 255.0f, 198 / 255.0f,
	0.5f, 0.5f, 0.5f, 10 / 255.0f, 189 / 255.0f, 198 / 255.0f,
	-0.5f, 0.5f, 0.5f, 10 / 255.0f, 189 / 255.0f, 198 / 255.0f,
	-0.5f, -0.5f, 0.5f, 10 / 255.0f, 189 / 255.0f, 198 / 255.0f
};

static constexpr const GLuint kIndicies[] = {
	0, 1, 3, 1, 2, 3, 1, 2, 5, 2, 6, 5, 2, 3, 6, 3, 7, 6, 3, 4, 7, 3, 0, 4, 0, 1, 4, 1, 5, 4, 4, 5, 7, 5, 6, 7
};

static constexpr const VertexObjectIndexed::ArgPack kArgs{ .data = kVerticies, .data_size = sizeof(kVerticies), .args_per_vertex = 6, 
														   .indices = kIndicies, .indices_size = sizeof(kIndicies), 
														   .memory_mode = VertexObject::MemoryMode::STATIC };

static constexpr const GLchar* kVertexShaderPath = "../src/Resources/Shaders/Cube.vert";
static constexpr const GLchar* kFragmentShaderPath = "../src/Resources/Shaders/Cube.frag";

static constexpr const Shader::ShaderConfig kShaderConfig{ .VertexShaderSourceType = Shader::SourceType::FILE, .VertexShaderSource = kVertexShaderPath,
														   .FragmentShaderSourceType = Shader::SourceType::FILE, .FragmentShaderSource = kFragmentShaderPath,
														   .GeometryShaderSourceType = Shader::SourceType::NONE, .GeometryShaderSource = ""};

Cube::Cube() : mesh_(kArgs), shader_(kShaderConfig) {
	mesh_.SetAttribute(0, 3, reinterpret_cast<GLvoid*>(0)); // position
	mesh_.SetAttribute(1, 3, reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // color
}

void Cube::Draw() const {
	shader_.Use();
	mesh_.Draw(VertexObject::DrawMode::TRIANGLES);
}

void Cube::SetTransform(const glm::mat4& position) {
	shader_.SetUniform("transform", position);
}