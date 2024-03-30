#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Utils/LightTimer/LightTimer.h>
#include <UI/UIManager/UIManager.h>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/Primitives/Cube/Cube.h>
#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/SlidingWindow/SlidingWindowIterator.h>
#include <Utils/FrameRater/FrameRater.h>
#include <memory>

int main(int argc, char** argv) {
	Window window(1600, 960, "ProgramTraceVisualizer!");
	window.CaptureContext(true);

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr, window.GetWindowSize());

    glEnable(GL_DEPTH_TEST);

    const size_t cells = 1024;

    std::shared_ptr<Plot2DMesh> plot = std::make_shared<Plot2DMesh>(cells);

    ui_manager.GetViewScene().AddObject(plot);

    LightTimer run_timer;
    HilbertCurve2D hilbert_curve(10);
    std::cout << "Hilbert Curve took " << run_timer.EvaluateTime() << std::endl;

    std::vector<int> values;
    for (int i = 0; i < cells * cells; ++i) {
        values.push_back(i);
    }
    std::shared_ptr<DataClamper<int>> data_clamper = std::make_shared<MinMaxDataClamper<int>>(values);

    const size_t sliding_window_size = 1000;
    SlidingWindowIterator sliding_window(values, sliding_window_size);

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
        if (!detail_scene.GetInnerBuffer().empty()) {
            for (int i = 0; i < sliding_window_size; ++i) {
                detail_scene.PopFrontLine();
            }
        }
        
        auto [start, end] = *sliding_window;
        for (auto cur = start; cur != end; ++cur) {
            size_t seq_number = data_clamper->GetClamped(*cur) * (cells * cells - 1);
            const auto point = hilbert_curve.Seq2XY(seq_number);
            data.push_back(2 * point.x - 1);
            data.push_back(2 * point.y - 1);
            data.push_back(1 - static_cast<float>(cur - start + 1) / (end - start));
            detail_scene.PushLine(std::to_string(*cur));
        }
        ++sliding_window;
        if (!sliding_window) {
            sliding_window.Restart();
        }

        plot->LoadData(data.data(), data.size() * sizeof(GLfloat));
        
        ui_manager.DrawUI();

		window.Render();
		glfwPollEvents();

        ++frames_counter;
        if (run_timer.EvaluateTime() >= 0.5) {
            run_timer.ResetTime();
            std::cout << frames_counter << std::endl;
            ui_manager.GetViewScene().SetFrametime(0.5 / frames_counter);
            frames_counter = 0;
        }
        frame_rater.Sleep();
	}
    std::cout << "IT TOOK " << run_timer.EvaluateTime() << std::endl;
	return 0;
}