#include "Plot3DMesh.h"

#include <Resources/ShadersEmbed/Plot3DShaders.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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

static const constexpr float kFow = 45.0f;
static const constexpr float kRatio = 1.0f;
static const constexpr float kNear = 0.1f;
static const constexpr float kFar = 100.0f;

static const constexpr float kPitchLimit = 1.56f;
}  // namespace

#define ShaderUniformName_Transform "transform"

Plot3DMesh::Plot3DMesh(const size_t grid_size) : PlotMesh(kVertexObjectConfig, kShaderConfig) {
    mesh_.SetAttribute(kPositionIndex, kPositionArgs, reinterpret_cast<GLvoid*>(0));  // position
    mesh_.SetAttribute(kTemperatureIndex, kTemperatureArgs,
                       reinterpret_cast<GLvoid*>(kPositionArgs * sizeof(GLfloat)));  // color
    SetGridSize(grid_size);
}

Plot3DMesh::Camera3D::Camera3D() {
    Reset();
}

glm::mat4 Plot3DMesh::Camera3D::GetViewTransform() const {
    glm::mat4 transform_matrix = glm::mat4(1.0f);
    glm::mat4 view_matrix = GetViewMatrix();
    const auto final_matrix = projection_matrix_ * view_matrix * transform_matrix;
    return final_matrix;
}

glm::mat4 Plot3DMesh::Camera3D::GetViewMatrix() const {
    const glm::quat rotation{glm::vec3{-pitch_, -yaw_, 0.0f}};
    const constexpr glm::vec3 kForwardVector{0.0f, 0.0f, -1.0f};
    const glm::vec3 distance_direction = glm::rotate(rotation, kForwardVector);
    const glm::vec3 position_offset = target_ - distance_direction * distance_;
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), position_offset) * glm::toMat4(rotation);
    view_matrix = glm::inverse(view_matrix);
    return view_matrix;
}

void Plot3DMesh::Camera3D::SetScreenRatio(float width, float height) {
    projection_matrix_ = glm::perspective(kFow, kRatio, kNear, kFar);
}

void Plot3DMesh::Camera3D::HandleMouseMove(float x, float y, float strength) {
    const auto up_component =
        glm::rotate(glm::quat{glm::vec3{-pitch_, -yaw_, 0.0f}}, glm::vec3{0.0f, 1.0f, 0.0f});
    const float sign = up_component.y < 0 ? -1.0f : 1.0f;
    yaw_ += sign * x * strength;
    pitch_ += y * strength;
    if (pitch_ > kPitchLimit) {
        pitch_ = kPitchLimit;
    } else if (pitch_ < -kPitchLimit) {
        pitch_ = -kPitchLimit;
    }
}

void Plot3DMesh::Camera3D::HandleMouseScroll(float mouse_scroll, float strength) {
    distance_ += -mouse_scroll * strength;
}

void Plot3DMesh::Camera3D::Reset() {
    projection_matrix_ = glm::perspective(kFow, kRatio, kNear, kFar);
    pitch_ = 0;
    yaw_ = 0;
    distance_ = 3.0f;
    target_ = {0.0f, 0.0f, 0.0f};
}

void Plot3DMesh::HandleMouseMove(float x, float y) {
    camera_.HandleMouseMove(x, y);
}

void Plot3DMesh::HandleMouseScroll(float scroll) {
    camera_.HandleMouseScroll(scroll);
}

void Plot3DMesh::LoadTransformFromCamera() {
    // shader_.SetUniform(ShaderUniformName_Transform, camera_.GetViewTransform());
    cube_.SetTransform(camera_.GetViewTransform());
}

void Plot3DMesh::SetViewPortSize(float width, float height) {
    camera_.SetScreenRatio(width, height);
}

void Plot3DMesh::Render() const {
    cube_.Render();
}