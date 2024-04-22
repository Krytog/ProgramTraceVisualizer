#include "UIManager.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#define GLSL_VERSION "#version 330"

namespace {
static const constexpr float kOptionsWidgetHeight = 35.0f;

static const constexpr float kViewSceneWidthCoef = 0.75f;

static const constexpr float kIPmovesControlSceneHeight = 30.0f;

static const constexpr std::pair<float, float> GetViewScenePos(const std::pair<int, int>& window_size) {
    return {(1.0f - kViewSceneWidthCoef) * window_size.first, kOptionsWidgetHeight};
}

static const constexpr std::pair<float, float> GetViewSceneSize(const std::pair<int, int>& window_size) {
    const float height = window_size.second - kOptionsWidgetHeight - kIPmovesControlSceneHeight;
    const float width = kViewSceneWidthCoef * window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetDetailsScenePos(const std::pair<int, int>& window_size) {
    return {0.0f, kOptionsWidgetHeight};
}

static const constexpr std::pair<float, float> GetDetailsSceneSize(const std::pair<int, int>& window_size) {
    const float height = window_size.second - kOptionsWidgetHeight;
    const float width = (1.0f - kViewSceneWidthCoef) * window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetOptionsScenePos(const std::pair<int, int>& window_size) {
    (void)window_size;
    return {0.0f, 0.0f};
}

static const constexpr std::pair<float, float> GetOptionsSceneSize(const std::pair<int, int>& window_size) {
    const float height = kOptionsWidgetHeight;
    const float width = window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetIPmovesControlScenePos(const std::pair<int, int>& window_size) {
    return {(1.0f - kViewSceneWidthCoef) * window_size.first, window_size.second - kIPmovesControlSceneHeight};
}

static const constexpr std::pair<float, float> GetIPmovesControlSceneSize(const std::pair<int, int>& window_size) {
    const float height = kIPmovesControlSceneHeight;
    const float width = kViewSceneWidthCoef * window_size.first;
    return {width, height};
}

}  // namespace

UIManager::UIManager(GLFWwindow* glfw_window, const std::pair<int, int>& window_size)
    : view_scene_(GetViewScenePos(window_size), GetViewSceneSize(window_size)),
      details_scene_(GetDetailsScenePos(window_size), GetDetailsSceneSize(window_size)),
      options_scene_(GetOptionsScenePos(window_size), GetOptionsSceneSize(window_size)),
      ipmovescontrol_scene_(GetIPmovesControlScenePos(window_size), GetIPmovesControlSceneSize(window_size)) {
    InitImGui(glfw_window);
}

void UIManager::DrawUI() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    RenderAllScenes();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::InitImGui(GLFWwindow* glfw_window) const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ViewScene& UIManager::GetViewScene() {
    return view_scene_;
}

DetailsScene& UIManager::GetDetailsScene() {
    return details_scene_;
}

OptionsScene& UIManager::GetOptionsScene() {
    return options_scene_;
}

IPmovesControlScene& UIManager::GetIPmovesControlScene() {
    return ipmovescontrol_scene_;
}

void UIManager::RenderAllScenes() const {
    view_scene_.Render();
    details_scene_.Render();
    options_scene_.Render();
    ipmovescontrol_scene_.Render();
}