#include "UIManager.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#define GLSL_VERSION "#version 330"

#define FOREACH_SCENE(x)         \
    do {                         \
        options_scene_.x;        \
        view_scene_.x;           \
        details_scene_.x;        \
        ipmovescontrol_scene_.x; \
    } while (false)

#define GET_SCENE_POS_AND_SIZE(name, arg) Get##name##ScenePos(arg, mode_), Get##name##SceneSize(arg, mode_)

namespace {
using Mode = UIManager::Mode;

static const constexpr float kOptionsWidgetHeight = 20.0f;

static const constexpr float kViewSceneWidthCoef = 0.75f;

static const constexpr float kIPmovesControlSceneHeight = 30.0f;

static const constexpr std::pair<float, float> GetViewScenePos(const std::pair<int, int>& window_size,
                                                               Mode mode = Mode::Waiting) {
    (void)mode;
    return {(1.0f - kViewSceneWidthCoef) * window_size.first, kOptionsWidgetHeight};
}

static const constexpr std::pair<float, float> GetViewSceneSize(const std::pair<int, int>& window_size,
                                                                Mode mode = Mode::Waiting) {
    float height = window_size.second - kOptionsWidgetHeight;
    if (mode == Mode::IP) {
        height = window_size.second - kOptionsWidgetHeight - kIPmovesControlSceneHeight;
    }
    const float width = kViewSceneWidthCoef * window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetDetailsScenePos(const std::pair<int, int>& window_size,
                                                                  Mode mode = Mode::Waiting) {
    (void)mode;
    return {0.0f, kOptionsWidgetHeight};
}

static const constexpr std::pair<float, float> GetDetailsSceneSize(const std::pair<int, int>& window_size,
                                                                   Mode mode = Mode::Waiting) {
    (void)mode;
    const float height = window_size.second - kOptionsWidgetHeight;
    const float width = (1.0f - kViewSceneWidthCoef) * window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetOptionsScenePos(const std::pair<int, int>& window_size,
                                                                  Mode mode = Mode::Waiting) {
    (void)window_size;
    (void)mode;
    return {0.0f, 0.0f};
}

static const constexpr std::pair<float, float> GetOptionsSceneSize(const std::pair<int, int>& window_size,
                                                                   Mode mode = Mode::Waiting) {
    (void)mode;
    const float height = kOptionsWidgetHeight;
    const float width = window_size.first;
    return {width, height};
}

static const constexpr std::pair<float, float> GetIPmovesControlScenePos(
    const std::pair<int, int>& window_size, Mode mode = Mode::Waiting) {
    if (mode != Mode::IP) {
        return {0, 0};
    }
    return {(1.0f - kViewSceneWidthCoef) * window_size.first,
            window_size.second - kIPmovesControlSceneHeight};
}

static const constexpr std::pair<float, float> GetIPmovesControlSceneSize(
    const std::pair<int, int>& window_size, Mode mode = Mode::Waiting) {
    if (mode != Mode::IP) {
        return {0, 0};
    }
    const float height = kIPmovesControlSceneHeight;
    const float width = kViewSceneWidthCoef * window_size.first;
    return {width, height};
}

}  // namespace

UIManager::UIManager(GLFWwindow* glfw_window, const std::pair<int, int>& window_size)
    : mode_(Mode::Waiting),
      current_size_(window_size),
      view_scene_(GetViewScenePos(window_size), GetViewSceneSize(window_size)),
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
    FOREACH_SCENE(Render());
}

void UIManager::Resize(const std::pair<int, int>& new_size) {
    current_size_ = new_size;
    options_scene_.UpdateOnResize(GET_SCENE_POS_AND_SIZE(Options, new_size));
    view_scene_.UpdateOnResize(GET_SCENE_POS_AND_SIZE(View, new_size));
    details_scene_.UpdateOnResize(GET_SCENE_POS_AND_SIZE(Details, new_size));
    ipmovescontrol_scene_.UpdateOnResize(GET_SCENE_POS_AND_SIZE(IPmovesControl, new_size));
}

void UIManager::GoToWaitingForFileMode() {
    while (!details_scene_.GetInnerBuffer().empty()) {
        details_scene_.PopBackLine();
    }
    details_scene_.PushLine("Waiting for trace file...");
    details_scene_.PushLine("Press \"File\" menu button to open a file");
    ipmovescontrol_scene_.SetVisibility(false);
    mode_ = Mode::Waiting;
}

void UIManager::GoToIPmovesMode() {
    while (!details_scene_.GetInnerBuffer().empty()) {
        details_scene_.PopBackLine();
    }
    ipmovescontrol_scene_.SetVisibility(true);
    mode_ = Mode::IP;
}

void UIManager::GoToW2VMode() {
    while (!details_scene_.GetInnerBuffer().empty()) {
        details_scene_.PopBackLine();
    }
    details_scene_.PushLine("Oops! This feature is not implemeted yet!");
    details_scene_.PushLine("There is nothing interesting here now... Change the mode");
    ipmovescontrol_scene_.SetVisibility(false);
    mode_ = Mode::Waiting;
}