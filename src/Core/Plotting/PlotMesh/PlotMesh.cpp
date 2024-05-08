#include "PlotMesh.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/VertexObjects/VertexObject.h"
#include "glm/fwd.hpp"

#define ShaderUniformName_GridSize "grid_size"

namespace {
static const std::string kUniformNameHighColor = "high_color";
static const std::string kUniformNameLowColor = "low_color";

glm::vec4 GetGLMColorFromArray(const float* color) {
    return {color[0], color[1], color[2], color[3]};
}
}  // namespace

PlotMesh::PlotMesh(const VertexObject::ArgPack& vertex_obj_args, const Shader::ShaderConfig& shader_config)
    : shader_(shader_config), mesh_(vertex_obj_args) {
}

PlotMesh::~PlotMesh() = default;

void PlotMesh::Render() const {
    shader_.Use();
    mesh_.Draw(VertexObject::DrawMode::POINTS);
}

void PlotMesh::SetGridSize(const size_t grid_size) {
    grid_size_ = grid_size;
    shader_.SetUniform(ShaderUniformName_GridSize,
                       1.0f / grid_size);  // it's 1.0f instead of 0.5f, becasuse OpenGl has coords [-1, 1]
}

size_t PlotMesh::GetGridSize() const {
    return grid_size_;
}

void PlotMesh::LoadData(const GLfloat* data, GLsizei data_size) {
    mesh_.LoadData(data, data_size, VertexObject::MemoryMode::STREAM);
}

void PlotMesh::SetHighColor(const float* color) {
    const glm::vec4 ready_color = GetGLMColorFromArray(color);
    shader_.SetUniform(kUniformNameHighColor, ready_color);
}

void PlotMesh::SetLowColor(const float* color) {
    const glm::vec4 ready_color = GetGLMColorFromArray(color);
    shader_.SetUniform(kUniformNameLowColor, ready_color);
}
