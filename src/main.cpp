#include <GL/glew.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GUI/Window/Window.h>
#include <Utils/LightTimer/LightTimer.h>
#include <thread>
#include <UI/UIManager/UIManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/Primitives/Cube/Cube.h>
#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Utils/FrameRater/FrameRater.h>

int main(int argc, char** argv) {
	Window window(1600, 960, "ProgramTraceVisualizer!");
	window.CaptureContext(true);

    auto ptr = window.GetInnerWindowPointer();
    UIManager ui_manager(ptr, window.GetWindowSize());

    glm::mat4 matrix = glm::mat4(1.0f);

    std::shared_ptr<Cube> cube = std::make_shared<Cube>();

    ui_manager.GetViewScene().AddObject(cube);

    glEnable(GL_DEPTH_TEST);

    std::shared_ptr<Cube> cube2 = std::make_shared<Cube>();

    ui_manager.GetViewScene().AddObject(cube2);

    const size_t cells = 64;

    std::shared_ptr<Plot2DMesh> plot = std::make_shared<Plot2DMesh>(cells);

    ui_manager.GetViewScene().AddObject(plot);

    LightTimer run_timer;
    HilbertCurve2D hilbert_curve(6);
    std::cout << "Hilbert Curve took " << run_timer.EvaluateTime() << std::endl;

    std::vector<int> values;
    for (int i = 0; i < cells * cells; ++i) {
        values.push_back(i);
    }
    std::shared_ptr<DataClamper<int>> data_clamper = std::make_shared<MinMaxDataClamper<int>>(values);

    size_t step = 0;
    run_timer.ResetTime();
    FrameRater<144> frame_rater;
	while (!window.IsPendingClose()) {

        const auto size = window.GetWindowSize();
        glViewport(0, 0, size.first, size.second);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        std::vector<GLfloat> data;
        
        size_t seq_number = data_clamper->GetClamped(values[step]) * (cells * cells - 1);
        const auto point = hilbert_curve.Seq2XY(seq_number);
        data.push_back(2 * point.x - 1);
        data.push_back(2 * point.y - 1);
        data.push_back(1.0f);
        ++step;
        if (step == values.size()) {
            step = 0;
            break;
        }

        plot->LoadData(data.data(), data.size() * sizeof(GLfloat));

        auto& detail_scene = ui_manager.GetDetailsScene();
        if (!detail_scene.GetInnerBuffer().empty()) {
            detail_scene.PopFrontLine();
        }
        detail_scene.PushLine(std::to_string(step));

        ui_manager.DrawUI();

		window.Render();
		glfwPollEvents();

        frame_rater.Sleep();
	}
    std::cout << "IT TOOK " << run_timer.EvaluateTime() << std::endl;
	return 0;
}