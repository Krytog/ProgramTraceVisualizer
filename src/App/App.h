#pragma once

#include "AppStateMachine.h"
#include "Graphics/Primitives/IRenderable.h"

#include <GUI/Window/Window.h>
#include <GUI/UI/UIManager/UIManager.h>
#include <Utils/FrameRater/FrameRater.h>
#include <Utils/LightTimer/LightTimer.h>
#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <Core/w2v/w2v.h>
#include <GUI/UI/Widgets/WaitingWidget.h>
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

    /* Initializes the state machine */
    void InitializeStateMachine();

    /* Initialize and configures OpenGL */
    void InitializeOpenGL();

    /* Initializes HilbertCurves that will be used in runtime */
    void InitializeHilbertCurves();

    /* Initializes IP moves module */
    void InitializeIPmovesMode();

    /* Initializes Portable File Dialogs */
    void InitializePFD();

    /* Initializes UI callbacks. In particular, initializes callbacks that will be executed when buttons from
     * OptionsScene are pressed */
    void InitializeUICallbacks();

    /* Initializes all UI systems and prepares them to be rendered */
    void InitializeUI();

    /* Does all the required work to enter IP moves mode */
    void EnterIPmovesMode();

    /* Does all the requied work to leave IP moves mode */
    void LeaveIPmovesMode();

    /* Recreates IPmovesModule. Should be called when the underlying file is updated */
    void RecreateIPmovesModule();

    /* Does all the required work to enter W2V mode */
    void EnterW2VMode();

    /* Does all the required work to leave W2V mode */
    void LeaveW2VMode();

    /* Recreates W2VModule. Should be called when the underlying file is updated */
    void RecreateW2VModule();

    /* Discards all modules. Should be called when a new file is opened */
    void DiscardAllModules();

    /* Enters waiting mode. Creates a new waiting_handler_, adds it to the ViewScene */
    void EnterWaitingMode();

    /* Leaves waiting mode. Removes waiting_handler_ from the ViewScene and discards it */
    void LeaveWaitingMode();

    LightTimer run_timer_;
    Window window_;
    UIManager ui_manager_;
    std::unique_ptr<IPmovesHandler> ip_moves_handler_;
    std::unique_ptr<W2VHandler> w2v_handler_;
    std::unique_ptr<WaitingWidget> waiting_handler_;
    AppStateMachine state_machine_;
    std::string current_filename_;
};