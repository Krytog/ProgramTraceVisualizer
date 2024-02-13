#include "Plot2DMesh.h"

namespace {
	static constexpr const VertexObject::ArgPack kVertexObjectConfig{.data = nullptr, .data_size = 0, 
		.args_per_vertex = 3,.memory_mode = VertexObject::MemoryMode::STREAM };

	static constexpr const GLchar* kVertexShaderPath = "../src/Resources/Shaders/Plot2D.vert";
	static constexpr const GLchar* kFragmentShaderPath = "../src/Resources/Shaders/Plot2D.frag";
	static constexpr const GLchar* kGeometryShaderPath = "../src/Resources/Shaders/Plot2D.geom";

	static constexpr const Shader::ShaderConfig kShaderConfig{.VertexShaderSourceType = Shader::SourceType::FILE, .VertexShaderSource = kVertexShaderPath,
		.FragmentShaderSourceType = Shader::SourceType::FILE, .FragmentShaderSource = kFragmentShaderPath,
		.GeometryShaderSourceType = Shader::SourceType::FILE, .GeometryShaderSource = kGeometryShaderPath};

	static constexpr const GLuint kPositionIndex = 0;
	static constexpr const GLuint kTemperatureIndex = 1;
	static constexpr const GLuint kPositionArgs = 2;
	static constexpr const GLuint kTemperatureArgs = 1;
}

#define ShaderUnifromName_GridSize "grid_size"

Plot2DMesh::Plot2DMesh(const size_t grid_size) : shader_(kShaderConfig), mesh_(kVertexObjectConfig) {
	mesh_.SetAttribute(kPositionIndex, kPositionArgs, reinterpret_cast<GLvoid*>(0)); // position
	mesh_.SetAttribute(kTemperatureIndex, kTemperatureArgs, reinterpret_cast<GLvoid*>(kPositionArgs * sizeof(GLfloat))); // color
	SetGridSize(grid_size);
}

void Plot2DMesh::Render() const {
	shader_.Use();
	mesh_.Draw(VertexObject::DrawMode::POINTS);
}

void Plot2DMesh::SetGridSize(const size_t grid_size) {
	grid_size_ = grid_size;
	shader_.SetUniform(ShaderUnifromName_GridSize, 0.5f / grid_size);
}

size_t Plot2DMesh::GetGridSize() const {
	return grid_size_;
}

void Plot2DMesh::LoadData(const GLfloat* data, GLsizei data_size) {
	mesh_.LoadData(data, data_size, VertexObject::MemoryMode::STREAM);
}
