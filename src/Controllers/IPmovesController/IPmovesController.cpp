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
            // TODO: pass new_color to the corresponding shader
        }
    }
    {  // old_color
        const auto [has_new_input, old_color] = controls.GetInputOldColor();
        if (has_new_input) {
            // TODO: pass old_color to the corresponding shader
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
            auto& view_scene = ui_manager->GetViewScene();
            view_scene.RemoveObject(handler->GetPlot());  // remove old plot
            handler->SetHilbertDegree(hilbert_degree);
            view_scene.AddObject(handler->GetPlot());  // add new plot
        }
    }
    {  // max_memory
        const auto [has_new_input, max_memory] = controls.GetInputMaxMemory();
        if (has_new_input) {
            handler->SetMaxMemory(max_memory);
        }
    }
}
}  // namespace controllers::ipmoves