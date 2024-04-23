#include "IPmovesController.h"

#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <GUI/UI/UIManager/UIManager.h>

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
            handler->SetHilbertDegree(hilbert_degree);
        }
    }
    {  // max_memory
        const auto [has_new_input, max_memory] = controls.GetInputMaxMemory();
        if (has_new_input) {
            handler->SetMaxMemory(max_memory);
        }
    }
}

std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager, const std::string& filename) {
    auto& controls = ui_manager->GetIPmovesControlScene();
    std::unique_ptr<IPmovesHandler> handler = std::make_unique<IPmovesHandler>(filename);
    static const constexpr float kInitProgress = 0.0f;
    controls.SetProgressValue(kInitProgress);
    const auto state = controls.GetState();
    handler->SetProgress(state.progress);
    handler->SetAdvanceCount(state.advance);
    handler->SetWindowSize(state.sliding_window_size);
    handler->SetHilbertDegree(state.hilbert_degree);
    handler->SetColorForNewest(state.new_color);
    handler->SetColorForOldest(state.old_color);
    handler->SetMaxMemory(state.max_memory);
    if (state.paused) {
        handler->Pause();
    } else {
        handler->Unpause();
    }
    ui_manager->GetViewScene().AddObject(handler->GetPlot());
    return handler;
}

void SetCallbacks(std::unique_ptr<IPmovesHandler>* handler_keeper, UIManager* ui_manager) {
    ui_manager->GetOptionsScene().SetNewTraceCallback([=](const std::string& filename) mutable {
        ui_manager->GetViewScene().RemoveObject((*handler_keeper)->GetPlot());
        *handler_keeper = std::move(Initialize(ui_manager, filename));
    });
}

}  // namespace controllers::ipmoves