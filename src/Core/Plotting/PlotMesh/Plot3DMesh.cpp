#include "Plot3DMesh.h"

#include <Resources/ShadersEmbed/Plot3DShaders.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_transform.hpp"

namespace {
static constexpr const VertexObject::ArgPack kVertexObjectConfig{
    .data = nullptr, .data_size = 0, .args_per_vertex = 4, .memory_mode = VertexObject::MemoryMode::STREAM};

static constexpr const Shader::ShaderConfig kShaderConfig{
    .VertexShaderSourceType = Shader::SourceType::STRING,
    .VertexShaderSource = kPlot3DShaderVert,
    .FragmentShaderSourceType = Shader::SourceType::STRING,
    .FragmentShaderSource = kPlot3DShaderFrag,
    .GeometryShaderSourceType = Shader::SourceType::STRING,
    .GeometryShaderSource = kPlot3DShaderGeom};

static constexpr const GLuint kPositionIndex = 0;
static constexpr const GLuint kTemperatureIndex = 1;
static constexpr const GLuint kPositionArgs = 3;
static constexpr const GLuint kTemperatureArgs = 1;
}  // namespace

#define ShaderUniformName_Transform "transform"

Plot3DMesh::Plot3DMesh(const size_t grid_size) : PlotMesh(kVertexObjectConfig, kShaderConfig) {
    mesh_.SetAttribute(kPositionIndex, kPositionArgs, reinterpret_cast<GLvoid*>(0));  // position
    mesh_.SetAttribute(kTemperatureIndex, kTemperatureArgs,
                       reinterpret_cast<GLvoid*>(kPositionArgs * sizeof(GLfloat)));  // color
    SetGridSize(grid_size);

    transform_matrix_ = glm::mat4(1.0f);
    view_matrix_ = glm::translate(transform_matrix_, glm::vec3(0.0f, 0.0f, -5.0f));
    projection_matrix_ = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    ApplyMatrices();
}

void Plot3DMesh::ApplyMatrices() {
    const auto final_matrix = projection_matrix_ * view_matrix_ * transform_matrix_;
    shader_.SetUniform(ShaderUniformName_Transform, final_matrix);
}

glm::mat4 Plot3DMesh::GetCameraTransform() const {
    return view_matrix_;
}

void Plot3DMesh::SetCameraTransform(const glm::mat4& transform) {
    view_matrix_ = transform;
}

void Plot3DMesh::TranslateCamera(const glm::vec3& vector) {
    view_matrix_ = glm::translate(view_matrix_, vector);
}

void Plot3DMesh::RotateCamera(float strength, const glm::vec3& axis) {
    view_matrix_ = glm::rotate(view_matrix_, strength, axis);
}

void Plot3DMesh::SetViewPortSize(float width, float height) {
    projection_matrix_ = glm::perspective(45.0f, width / height, 0.1f, 100.0f);
}