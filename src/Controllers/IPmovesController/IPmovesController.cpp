#include "IPmovesController.h"

#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <GUI/UI/UIManager/UIManager.h>

namespace controllers::ipmoves {
    void Synchronize(UIManager* ui_manager, IPmovesHandler* handler) {
        auto& controls = ui_manager->GetIPmovesControlScene();
        const auto [has_new_input, input] = controls.GetUserInput();
        if (has_new_input) {
            handler->SetProgress(input);
        } else {
            const float progress = handler->GetProgress();
            controls.SetProgressValue(progress);
        }
    }
}