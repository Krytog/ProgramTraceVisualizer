#include "App.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>
#include <Controllers/IPmovesController/IPmovesController.h>
#include "Core/IPmoves/IPmovesHandler/IPmovesHandler.h"
#include <portable-file-dialogs.h>
#include <stdexcept>

#define WINDOW_NAME "ProgramTraceVisualizer"

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
    controllers::ipmoves::Synchronize(ui_manager_.get(), ip_moves_handler_.get());
    ip_moves_handler_->Update();
}

void App::CountFPS() {
    static size_t frames_count = 0;
    ++frames_count;
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
    InitializeOpenGL();
    InitializePFD();
    InitializeHilbertCurves();
    InitializeIPmoves();
}

void App::InitializeOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void App::InitializeHilbertCurves() {
    const constexpr int kMinHilbertDegree = 1;
    const constexpr int kMaxHilbertDegree = 10;
    for (int degree = kMinHilbertDegree; degree <= kMaxHilbertDegree; ++degree) {
        HilbertCurveManager::PrepareHilbertCurve(degree);
    }
}

void App::InitializeIPmoves() {
    static const constexpr char* kDefaultName = "captured_ip.trace";
    ip_moves_handler_ = controllers::ipmoves::Initialize(ui_manager_.get(), kDefaultName);
    controllers::ipmoves::SetCallbacks(&ip_moves_handler_, ui_manager_.get());
}

void App::InitializePFD() {
    if (!pfd::settings::available()) {
        throw std::runtime_error("Portable file dialog failed to locate any suitable backend.");
    }
}