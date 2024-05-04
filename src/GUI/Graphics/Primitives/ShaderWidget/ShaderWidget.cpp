#include "ShaderWidget.h"
#include "GUI/Graphics/Shader/Shader.h"

namespace {

static constexpr const GLfloat kVerticies[] = {-1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  0.0f,
                                               -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f};

static constexpr const GLuint kIndicies[] = {0, 1, 2, 0, 1, 3};

static const constexpr char* kShaderVert = R"(
#version 330 core

layout (location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0f);
}
)";

static constexpr const VertexObjectIndexed::ArgPack kArgs{.data = kVerticies,
                                                          .data_size = sizeof(kVerticies),
                                                          .args_per_vertex = 3,
                                                          .indices = kIndicies,
                                                          .indices_size = sizeof(kIndicies),
                                                          .memory_mode = VertexObject::MemoryMode::STATIC};

static constexpr Shader::ShaderConfig GetShaderConfig(const char* shader_code) {
    Shader::ShaderConfig shader_config{.VertexShaderSourceType = Shader::SourceType::STRING,
                                       .VertexShaderSource = kShaderVert,
                                       .FragmentShaderSourceType = Shader::SourceType::STRING,
                                       .FragmentShaderSource = shader_code,
                                       .GeometryShaderSourceType = Shader::SourceType::NONE,
                                       .GeometryShaderSource = ""};
    return shader_config;
}

}  // namespace

ShaderWidget::ShaderWidget(const char* shader_code) : mesh_(kArgs), shader_(GetShaderConfig(shader_code)) {
    const constexpr GLuint kIndex = 0;
    const constexpr GLuint kArgsPerVertex = 3;
    mesh_.SetAttribute(kIndex, kArgsPerVertex, reinterpret_cast<GLvoid*>(0));  // position
}

void ShaderWidget::SetTime(float time) {
    shader_.SetUniform("time", time);
}

void ShaderWidget::SetResolution(float width, float height) {
    shader_.SetUniform("width", width);
    shader_.SetUniform("height", height);
}