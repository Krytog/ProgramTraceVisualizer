#pragma once

#include <GUI/Window/Window.h>
#include <GUI/UI/UIManager/UIManager.h>
#include <Utils/FrameRater/FrameRater.h>
#include <Utils/LightTimer/LightTimer.h>
#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <memory>

class App {
public:
    App(int width, int height);

    /* Runs the app. Returns when the app is closed */
    template <uint32_t FPS>
    void Run() {
        run_timer_.ResetTime();
        FrameRater<FPS> frame_rater;
        while (!window_.IsPendingClose()) {
            LoopIteration();
            frame_rater.Sleep();
        }
    }

private:
    /* A single iteration of the program loop */
    void LoopIteration();

    /* Completes all the technical work that should be done before the main logic of an iteration */
    void BeginFrame();

    /* Completes all the technical work that should be done after the main logic of an iteration */
    void EndFrame();

    /* Completes the main logic of an iteration */
    void FrameMainLogic();

    /* Counts FPS of the previous delta time and passes it into the ViewScene */
    void CountFPS();

    /* Initializes all systems of the app. Should be called before Run() */
    void Initialization();

    /* Initializes HilbertCurves that will be used in runtime */
    void InitializeHilbertCurves();

    /* Initializes IPmovesHandler module */
    void InitializationIPmoves();

    LightTimer run_timer_;
    Window window_;
    std::unique_ptr<UIManager> ui_manager_;
    std::unique_ptr<IPmovesHandler> ip_moves_handler_; // temp
};