#include "Plot2DMesh.h"

#include <Resources/ShadersEmbed/Plot2DShaders.h>

namespace {
static constexpr const VertexObject::ArgPack kVertexObjectConfig{
    .data = nullptr, .data_size = 0, .args_per_vertex = 3, .memory_mode = VertexObject::MemoryMode::STREAM};

static constexpr const Shader::ShaderConfig kShaderConfig{
    .VertexShaderSourceType = Shader::SourceType::STRING,
    .VertexShaderSource = kPlot2DShaderVert,
    .FragmentShaderSourceType = Shader::SourceType::STRING,
    .FragmentShaderSource = kPlot2DShaderFrag,
    .GeometryShaderSourceType = Shader::SourceType::STRING,
    .GeometryShaderSource = kPlot2DShaderGeom};

static constexpr const GLuint kPositionIndex = 0;
static constexpr const GLuint kTemperatureIndex = 1;
static constexpr const GLuint kPositionArgs = 2;
static constexpr const GLuint kTemperatureArgs = 1;
}  // namespace

Plot2DMesh::Plot2DMesh(const size_t grid_size) : PlotMesh(kVertexObjectConfig, kShaderConfig) {
    mesh_.SetAttribute(kPositionIndex, kPositionArgs, reinterpret_cast<GLvoid*>(0));  // position
    mesh_.SetAttribute(kTemperatureIndex, kTemperatureArgs,
                       reinterpret_cast<GLvoid*>(kPositionArgs * sizeof(GLfloat)));  // color
    SetGridSize(grid_size);
}