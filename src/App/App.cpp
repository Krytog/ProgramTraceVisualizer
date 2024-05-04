#include "App.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>
#include <Controllers/IPmovesController/IPmovesController.h>
#include "App/AppStateMachine.h"
#include "Core/IPmoves/IPmovesHandler/IPmovesHandler.h"
#include "Core/w2v/w2v.h"
#include "UI/UIManager/UIManager.h"
#include <portable-file-dialogs.h>
#include <memory>
#include <stdexcept>

#define WINDOW_NAME "ProgramTraceVisualizer"

#define ERROR_MESSAGE_STATE_MACHINE_FAILED "App: StateMachine failed: incorrect state after opening a file"

namespace {
/* This function is needed to initialize UIManager because window.CaptureContext(true) should be called before
 * any UIManager is created */
static UIManager GetInitializedUIManager(Window& window) {
    window.CaptureContext(true);
    return {window.GetInnerWindowPointer(), window.GetWindowSize()};
}
}  // namespace

App::App(int width, int height)
    : window_(width, height, WINDOW_NAME), ui_manager_(GetInitializedUIManager(window_)) {
    Initialization();
}

void App::BeginFrame() {
    const auto size = window_.GetWindowSize();
    glViewport(0, 0, size.first, size.second);
    ui_manager_.Resize(size);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::EndFrame() {
    ui_manager_.DrawUI();
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
    using States = AppStateMachine::States;
    switch (state_machine_.GetState()) {
        case States::FILE_IP: {
            controllers::ipmoves::Synchronize(&ui_manager_, ip_moves_handler_.get());
            ip_moves_handler_->Update();
            break;
        }
        default: {
            return;
        }
    }
}

void App::CountFPS() {
    static size_t frames_count = 0;
    ++frames_count;
    const constexpr float kFpsPeriod = 0.2f;
    if (run_timer_.EvaluateTime() >= kFpsPeriod) {
        run_timer_.ResetTime();
        ui_manager_.GetViewScene().SetFrametime(kFpsPeriod / frames_count);
        frames_count = 0;
    }
}

void App::Initialization() {
    InitializeOpenGL();
    InitializePFD();
    InitializeStateMachine();
    InitializeUI();
    InitializeIPmovesMode();
}

void App::InitializeStateMachine() {
    using States = AppStateMachine::States;
    const constexpr States kStartState = States::NO_FILE_IP;
    state_machine_ = AppStateMachine(kStartState);
    state_machine_.AddCallback({States::NO_FILE_IP, States::FILE_IP}, [this]() { EnterIPmovesMode(); });
    state_machine_.AddCallback({States::NO_FILE_W2V, States::FILE_W2V}, [this]() { EnterW2VMode(); });
    state_machine_.AddCallback({States::NO_FILE_IP, States::NO_FILE_W2V},
                               []() { std::cout << "IP -> W2V: NO FILE" << std::endl; });
    state_machine_.AddCallback({States::NO_FILE_W2V, States::NO_FILE_IP},
                               []() { std::cout << "W2V -> IP: NO FILE" << std::endl; });
    state_machine_.AddCallback({States::FILE_IP, States::FILE_W2V}, [this]() {
        LeaveIPmovesMode();
        EnterW2VMode();
    });
    state_machine_.AddCallback({States::FILE_W2V, States::FILE_IP}, [this]() {
        LeaveW2VMode();
        EnterIPmovesMode();
    });
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

void App::InitializeIPmovesMode() {
    InitializeHilbertCurves();
}

void App::InitializePFD() {
    if (!pfd::settings::available()) {
        throw std::runtime_error("Portable file dialog failed to locate any suitable backend.");
    }
}

void App::InitializeUICallbacks() {
    auto& options_scene = ui_manager_.GetOptionsScene();
    options_scene.SetOpenFileCallback([this](const std::string& filename) {
        DiscardAllModules();
        current_filename_ = filename;
        const auto prev_state = state_machine_.GetState();
        state_machine_.GoToStateWithFile();
        /* If the mode hasn't changed, then we need to manualy recreate the appropriate module */
        switch (state_machine_.GetState()) {
            case AppStateMachine::States::FILE_IP: {
                if (prev_state == AppStateMachine::States::FILE_IP) {
                    RecreateIPmovesModule();
                }
                break;
            }
            case AppStateMachine::States::FILE_W2V: {
                if (prev_state == AppStateMachine::States::FILE_W2V) {
                    RecreateW2VModule();
                }
                break;
            }
            default: {
                throw std::runtime_error(ERROR_MESSAGE_STATE_MACHINE_FAILED);
            }
        }
    });
    options_scene.SetModeIPCallback(
        [this]() { state_machine_.GoToStateIgnoringFile(AppStateMachine::States::FILE_IP); });
    options_scene.SetModeW2VCallback(
        [this]() { state_machine_.GoToStateIgnoringFile(AppStateMachine::States::FILE_W2V); });
}

void App::InitializeUI() {
    ui_manager_.GoToWaitingForFileMode();
    InitializeUICallbacks();
}

void App::EnterIPmovesMode() {
    if (!ip_moves_handler_) {
        ip_moves_handler_ = std::move(controllers::ipmoves::Initialize(&ui_manager_, current_filename_));
    }
    ui_manager_.GetViewScene().AddObject(ip_moves_handler_->GetPlot());
    ui_manager_.GoToIPmovesMode();
}

void App::LeaveIPmovesMode() {
    if (ip_moves_handler_) {
        ui_manager_.GetViewScene().RemoveObject(ip_moves_handler_->GetPlot());
    }
}

void App::RecreateIPmovesModule() {
    if (ip_moves_handler_) {
        ui_manager_.GetViewScene().RemoveObject(ip_moves_handler_->GetPlot());
    }
    ip_moves_handler_ = std::move(controllers::ipmoves::Initialize(&ui_manager_, current_filename_));
    ui_manager_.GetViewScene().AddObject(ip_moves_handler_->GetPlot());
}

void App::EnterW2VMode() {
    if (!w2v_handler_) {
        w2v_handler_ = std::make_unique<W2VHandler>(current_filename_);
    }
    ui_manager_.GetViewScene().AddObject(w2v_handler_->GetPlot());
    ui_manager_.GoToW2VMode();
}

void App::LeaveW2VMode() {
    if (w2v_handler_) {
        ui_manager_.GetViewScene().RemoveObject(w2v_handler_->GetPlot());
    }
}

void App::RecreateW2VModule() {
    if (w2v_handler_) {
        ui_manager_.GetViewScene().RemoveObject(w2v_handler_->GetPlot());
    }
    w2v_handler_ = std::move(std::make_unique<W2VHandler>(current_filename_));
    ui_manager_.GetViewScene().AddObject(w2v_handler_->GetPlot());
}

void App::DiscardAllModules() {
    if (ip_moves_handler_) {
        ui_manager_.GetViewScene().RemoveObject(ip_moves_handler_->GetPlot());
    }
    ip_moves_handler_ = nullptr;
    if (w2v_handler_) {
        ui_manager_.GetViewScene().RemoveObject(w2v_handler_->GetPlot());
    }
    w2v_handler_ = nullptr;
}