#include "App.h"

#include <GLFW/glfw3.h>
#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>

#define WINDOW_NAME "ProgramTRaceVisualizer"

App::App(int width, int height): window_(width, height, WINDOW_NAME) {
    Initialization();
}

void App::BeginFrame() {
    const auto size = window_.GetWindowSize();
    glViewport(0, 0, size.first, size.second);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::EndFrame() {
    ui_manager_->DrawUI();
    window_.Render();
    glfwPollEvents();
    CountFPS();
}

void App::LoopIteration() {
    BeginFrame();
    FrameMainLogic();
    EndFrame();
}

void App::FrameMainLogic() {
    auto& detail_scene = ui_manager_->GetDetailsScene();
    while (!detail_scene.GetInnerBuffer().empty()) {
        detail_scene.PopFrontLine();
    }
    ip_moves_handler_->Update();
    detail_scene.PushLine(std::to_string(ip_moves_handler_->GetProgress()));
}

void App::CountFPS() {
    static size_t frames_count = 0;
    const constexpr float kFpsPeriod = 0.2f;
    if (run_timer_.EvaluateTime() >= kFpsPeriod) {
        run_timer_.ResetTime();
        ui_manager_->GetViewScene().SetFrametime(kFpsPeriod / frames_count);
        frames_count = 0;
    }
}

void App::Initialization() {
    window_.CaptureContext(true);
    ui_manager_ = std::make_unique<UIManager>(window_.GetInnerWindowPointer(), window_.GetWindowSize());
    glEnable(GL_DEPTH_TEST);
    InitializeHilbertCurves();
    InitializationIPmoves();
}

void App::InitializeHilbertCurves() {
    const constexpr int kMinHilbertDegree = 1;
    const constexpr int kMaxHilbertDegree = 10;
    for (int degree = kMinHilbertDegree; degree <= kMaxHilbertDegree; ++degree) {
        HilbertCurveManager::PrepareHilbertCurve(degree);
    }
}

void App::InitializationIPmoves() {
    ip_moves_handler_ = std::make_unique<IPmovesHandler>("captured_ip.trace");
    ip_moves_handler_->SetAdvanceCount(1);
    ip_moves_handler_->SetHilbertDegree(5);
    ip_moves_handler_->SetWindowSize(10);
    ui_manager_->GetViewScene().AddObject(ip_moves_handler_->GetPlot());
}