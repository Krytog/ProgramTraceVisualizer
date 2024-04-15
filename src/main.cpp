#include <GL/glew.h>

#include <cstdint>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Utils/LightTimer/LightTimer.h>
#include <GUI/UI/UIManager/UIManager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/Primitives/Cube/Cube.h>
#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/SlidingWindow/SlidingWindowIteratorExtended.h>
#include <Core/IPmoves/IPmovesHandler/AddressBuffer.h>
#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <Utils/FrameRater/FrameRater.h>
#include <limits>
#include <memory>

int main(int argc, char** argv) {
	Window window(1600, 960, "ProgramTraceVisualizer!");
	window.CaptureContext(true);

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr, window.GetWindowSize());

    glEnable(GL_DEPTH_TEST);

    LightTimer run_timer;
    HilbertCurveManager::PrepareHilbertCurve(10);
    HilbertCurveManager::PrepareHilbertCurve(5);
    std::cout << "Hilbert Curve took " << run_timer.EvaluateTime() << std::endl;

    const std::string filename = "captured_ip.trace";
    IPmovesHandler ip_moves_handler(filename);
    ip_moves_handler.SetHilbertDegree(5);
    ip_moves_handler.SetWindowSize(10);
    ip_moves_handler.SetAdvanceCount(1);

    ui_manager.GetViewScene().AddObject(ip_moves_handler.GetPlot());

    run_timer.ResetTime();
    size_t frames_counter = 0;
    FrameRater<144> frame_rater;
    size_t counter = 0;
	while (!window.IsPendingClose()) {
        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto& detail_scene = ui_manager.GetDetailsScene();
        while (!detail_scene.GetInnerBuffer().empty()) {
            detail_scene.PopFrontLine();
        }
        
        ip_moves_handler.Update();

        detail_scene.PushLine(std::to_string(ip_moves_handler.GetProgress()));

        ui_manager.DrawUI();

		window.Render();
		glfwPollEvents();
            
        ++frames_counter;
        if (run_timer.EvaluateTime() >= 0.5) {
            run_timer.ResetTime();
            ui_manager.GetViewScene().SetFrametime(0.5 / frames_counter);
            frames_counter = 0;
        }
        frame_rater.Sleep();
	}
	return 0;
}