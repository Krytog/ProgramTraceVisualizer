#include "ViewScene.h"

#include <Graphics/Primitives/IRenderable.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <limits>
#include "Graphics/RenderBuffer/RenderBuffer.h"

#define ViewScene_InnerName "View"

namespace {
    static const constexpr float kTextHeightOffset = 30.0f;  // it's magic constant

    bool AreEqual(float lhs, float rhs) {
        static const constexpr float kPrecision = 1e-6;
        return std::abs(lhs - rhs) < kPrecision;
    }
}

ViewScene::ViewScene(const std::pair<float, float>& position, const std::pair<float, float>& size)
    : BasicScene(position, size, ViewScene_InnerName),
      render_buffer_(size.first, size.second),
      frametime_(std::numeric_limits<double>::infinity()) {
}

bool ViewScene::AddObject(const IRenderable* object) {
    bool output = true;
    if (renderable_objects_.contains(object)) {
        output = false;
    }
    renderable_objects_.insert(object);
    return output;
}

bool ViewScene::RemoveObject(const IRenderable* object) {
    auto iterator = renderable_objects_.find(object);
    if (iterator == renderable_objects_.end()) {
        return false;
    }
    renderable_objects_.erase(iterator);
    return true;
}

void ViewScene::RenderInner() const {
    RenderObjects();
    RenderUI();
}

void ViewScene::RenderObjects() const {
    render_buffer_.Bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    render_buffer_.Clear();
    for (const auto& renderable : renderable_objects_) {
        renderable->Render();
    }
    render_buffer_.Unbind();
}

void ViewScene::RenderUI() const {
    const unsigned long long framerate = 1.0 / frametime_;
    ImGui::Text(ViewScene_InnerName " \t FrameTime: %.8f \t FPS: %llu", frametime_, framerate);

    const GLuint texture_id = render_buffer_.GetTextureID();
    const long long unsigned texture_id_upcast = static_cast<long long unsigned>(
        texture_id);  // upcasting GLuint to a bigger intenger type for safety reasons
    ImGui::Image(reinterpret_cast<void*>(texture_id_upcast),
                 ImVec2{size_.first, size_.second - kTextHeightOffset}, ImVec2{0, 1}, ImVec2{1, 0});
}

void ViewScene::UpdateOnResize(const std::pair<float, float>& posiiton, const std::pair<float, float>& size) {
    position_ = posiiton;
    if (!AreEqual(size.first, size_.first) || !AreEqual(size.second, size_.second)) {
        render_buffer_.Resize(size.first, size.second);
        size_ = size;
    }
}

void ViewScene::SetFrametime(double frametime) {
    frametime_ = frametime;
}

std::pair<float, float> ViewScene::GetViewPortSize() const {
    return {size_.first, size_.second - kTextHeightOffset};
}
