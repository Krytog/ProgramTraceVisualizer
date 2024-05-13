#include "TrajectoryController.h"

#include <Core/Trajectory/TrajectoryHandler.h>
#include <GUI/UI/UIManager/UIManager.h>

namespace {
    inline void SynchronizeDimension(UIManager* ui_manager, TrajectoryHandler* handler) {
        const auto handler_dimension = handler->GetDimension();
        auto* controls = &ui_manager->GetTrajectoryControlScene();
        const auto [is_new, required_dimension] = controls->GetInputDimension();
        if (is_new && required_dimension != handler_dimension) {
            ui_manager->GetViewScene().RemoveObject(handler->GetPlot());
            handler->SetDimension(required_dimension);
            controls->SetRealDimension(required_dimension);
            ui_manager->GetViewScene().AddObject(handler->GetPlot());
            const auto state = controls->GetState();
            handler->SetBeginColor(state.begin_color);
            handler->SetEndColor(state.end_color);
        }
    }

    inline void SynchronizeBeginColor(TrajectoryControlScene& controls, TrajectoryHandler* handler) {
        const auto [has_new_input, begin_color] = controls.GetInputBeginColor();
        if (has_new_input) {
            handler->SetBeginColor(begin_color);
        }
    }

    inline void SynchronizeEndColor(TrajectoryControlScene& controls, TrajectoryHandler* handler) {
        const auto [has_new_input, end_color] = controls.GetInputEndColor();
        if (has_new_input) {
            handler->SetEndColor(end_color);
        }
    }

    inline void SynchronizeHilbertDegree(TrajectoryControlScene& controls, TrajectoryHandler* handler) {
        const auto [has_new_input, degree] = controls.GetInputHilbertDegree();
        if (has_new_input) {
            handler->SetHilbertDegree(degree);
        }
    }

    inline void ApplyInput(UIManager* ui_manager, TrajectoryHandler* handler) {
        const auto* view_port = &ui_manager->GetViewScene();
        const auto [width, height] = view_port->GetViewPortSize();
        handler->SetViewPortSize(width, height);
        const auto [delta_x, delta_y] = view_port->GetMouseDelta();
        const auto scroll = view_port->GetMouseScroll();
        if (view_port->IsMousePressed()) {
            handler->PassMouseInput(delta_x, delta_y, scroll);
        } else {
            handler->PassMouseInput(0, 0, scroll);
        }
    }
}

namespace controllers::trajectory {

void Synchronize(UIManager* ui_manager, TrajectoryHandler* handler) {
    auto& controls = ui_manager->GetTrajectoryControlScene();
    SynchronizeDimension(ui_manager, handler);
    SynchronizeBeginColor(controls, handler);
    SynchronizeEndColor(controls, handler);
    SynchronizeHilbertDegree(controls, handler);
    ApplyInput(ui_manager, handler);
}

std::unique_ptr<TrajectoryHandler> Initialize(UIManager* ui_manager, const std::string& filename) {
    auto handler = std::make_unique<TrajectoryHandler>(filename);
    const auto state = ui_manager->GetTrajectoryControlScene().GetState();
    handler->SetDimension(state.dimension);
    handler->SetHilbertDegree(state.hilbert_degree);
    handler->SetBeginColor(state.begin_color);
    handler->SetEndColor(state.end_color);
    return std::move(handler);
}
}  // namespace controllers::trajectory