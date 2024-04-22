#include "IPmovesController.h"

#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <GUI/UI/UIManager/UIManager.h>

namespace {
    void UpdatePlot(UIManager* ui_manager, IPmovesHandler* handler, int hilbert_degree) {
        auto& view_scene = ui_manager->GetViewScene();
        view_scene.RemoveObject(handler->GetPlot());
        handler->SetHilbertDegree(hilbert_degree);
        const auto state = ui_manager->GetIPmovesControlScene().GetState();
        
        /* Colors are passed into shaders, when a new plot is created, it contains a new shader, so we need to pass colors once again */
        handler->SetColorForNewest(state.new_color);
        handler->SetColorForOldest(state.old_color);

        view_scene.AddObject(handler->GetPlot());
    }
}

namespace controllers::ipmoves {
void Synchronize(UIManager* ui_manager, IPmovesHandler* handler) {
    auto& controls = ui_manager->GetIPmovesControlScene();
    {  // progress
        const auto [has_new_input, input] = controls.GetInputProgress();
        if (has_new_input) {
            handler->SetProgress(input);
        } else {
            const float progress = handler->GetProgress();
            controls.SetProgressValue(progress);
        }
    }
    {  // pause
        const auto [has_new_input, is_paused] = controls.GetInputPause();
        if (has_new_input) {
            if (is_paused) {
                handler->Pause();
            } else {
                handler->Unpause();
            }
        }
    }
    {  // new_color
        const auto [has_new_input, new_color] = controls.GetInputNewColor();
        if (has_new_input) {
            handler->SetColorForNewest(new_color);
        }
    }
    {  // old_color
        const auto [has_new_input, old_color] = controls.GetInputOldColor();
        if (has_new_input) {
            handler->SetColorForOldest(old_color);
        }
    }
    {  // sliding_window_size
        const auto [has_new_input, sliding_window_size] = controls.GetInputSlidingWindowSize();
        if (has_new_input) {
            handler->SetWindowSize(sliding_window_size);
        }
    }
    {  // advance
        const auto [has_new_input, advance] = controls.GetInputAdvance();
        if (has_new_input) {
            handler->SetAdvanceCount(advance);
        }
    }
    {  // hilbert_degree
        const auto [has_new_input, hilbert_degree] = controls.GetInputHilbertDegree();
        if (has_new_input) {
            UpdatePlot(ui_manager, handler, hilbert_degree);
        }
    }
    {  // max_memory
        const auto [has_new_input, max_memory] = controls.GetInputMaxMemory();
        if (has_new_input) {
            handler->SetMaxMemory(max_memory);
        }
    }
}

std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager) {
    auto& controls = ui_manager->GetIPmovesControlScene();
    const std::string filename = "captured_ip.trace";  // TODO : get it from ui_manager;
    std::unique_ptr<IPmovesHandler> handler = std::make_unique<IPmovesHandler>(filename);
    controls.ResetState();
    const auto state = controls.GetState();
    handler->SetProgress(state.progress);
    handler->SetAdvanceCount(state.advance);
    handler->SetWindowSize(state.sliding_window_size);
    handler->SetMaxMemory(state.max_memory);
    if (state.paused) {
        handler->Pause();
    } else {
        handler->Unpause();
    }
    UpdatePlot(ui_manager, handler.get(), state.hilbert_degree);
    return handler;
}
}  // namespace controllers::ipmoves