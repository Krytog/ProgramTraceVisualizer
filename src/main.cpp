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
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/SlidingWindow/SlidingWindowIteratorExtended.h>
#include <Core/IPmoves/IPmovesHandler/AddressBuffer.h>
#include <Utils/FrameRater/FrameRater.h>
#include <limits>
#include <memory>

int main(int argc, char** argv) {
	Window window(1600, 960, "ProgramTraceVisualizer!");
	window.CaptureContext(true);

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr, window.GetWindowSize());

    glEnable(GL_DEPTH_TEST);

    const size_t cells = 128;

    std::shared_ptr<Plot2DMesh> plot = std::make_shared<Plot2DMesh>(cells);

    ui_manager.GetViewScene().AddObject(plot);

    LightTimer run_timer;
    HilbertCurve2D hilbert_curve(7);
    std::cout << "Hilbert Curve took " << run_timer.EvaluateTime() << std::endl;

    const uintptr_t lower = 100514797300000;
    const uintptr_t higher = 100514798300000;
    std::shared_ptr<DataClamper<uintptr_t>> data_clamper = std::make_shared<AbsoluteDataClamper<uintptr_t>>(lower, higher);

    const std::string filename = "captured_ip.trace";
    AddressBuffer buffer(filename);
    buffer.SetMaxMemory(1<<15);

    const size_t sliding_window_size = 1;
    SlidingWindowIteratorExtended sliding_window(buffer);
    sliding_window.SetSize(sliding_window_size);
    sliding_window.Restart();
    //sliding_window.SetProgress(0.98);

    run_timer.ResetTime();
    size_t frames_counter = 0;
    FrameRater<144> frame_rater;
	while (!window.IsPendingClose()) {
        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::vector<GLfloat> data;

        auto& detail_scene = ui_manager.GetDetailsScene();
        while (!detail_scene.GetInnerBuffer().empty()) {
            detail_scene.PopFrontLine();
        }
        
        auto [start, end] = *sliding_window;
        for (auto cur = start; cur != end; ++cur) {
            size_t seq_number = data_clamper->GetClamped(*cur) * (cells * cells - 1);
            const auto point = hilbert_curve.Seq2XY(seq_number);
            data.push_back(2 * point.x - 1);
            data.push_back(2 * point.y - 1);
            //data.push_back(1 - static_cast<float>(cur - start + 1) / (end - start));
            //detail_scene.PushLine(std::to_string(*cur));
            data.push_back(1);
        }
        sliding_window.Advance();
        if (!sliding_window) {
            sliding_window.Restart();
        }

        detail_scene.PushLine(std::to_string(static_cast<float>(start - buffer.begin()) / (buffer.end() - buffer.begin())));

        plot->LoadData(data.data(), data.size() * sizeof(GLfloat));

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