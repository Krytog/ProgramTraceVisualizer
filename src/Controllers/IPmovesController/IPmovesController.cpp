#include "IPmovesController.h"

#include <Core/IPmoves/IPmovesHandler/IPmovesHandler.h>
#include <GUI/UI/UIManager/UIManager.h>

#include <sstream>
#include "UI/Scenes/IPmovesControlScene/IPmovesControlScene.h"

namespace {
template <typename T>
inline std::string GetNumberAsHex(T number) {
    std::stringstream sstream;
    sstream << "0x" << std::hex << number;
    return sstream.str();
}

inline void UpdateDetails(UIManager* ui_manager, IPmovesHandler* handler) {
    auto& details = ui_manager->GetDetailsScene();
    while (!details.GetInnerBuffer().empty()) {
        details.PopBackLine();
    }
    for (const auto& address : handler->GetCurrentAddresses()) {
        const std::string hex_address = GetNumberAsHex(address);
        details.PushLine(hex_address);
    }
}

inline void SynchronizeProgress(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, input] = controls.GetInputProgress();
    if (has_new_input) {
        handler->SetProgress(input);
    } else {
        const float progress = handler->GetProgress();
        controls.SetProgressValue(progress);
    }
}

inline void SynchronizePuase(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, is_paused] = controls.GetInputPause();
    if (has_new_input) {
        if (is_paused) {
            handler->Pause();
        } else {
            handler->Unpause();
        }
    }
}

inline void SynchronizeNewColor(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, new_color] = controls.GetInputNewColor();
    if (has_new_input) {
        handler->SetColorForNewest(new_color);
    }
}

inline void SynchronizeOldColor(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, old_color] = controls.GetInputOldColor();
    if (has_new_input) {
        handler->SetColorForOldest(old_color);
    }
}

inline void SynchronizeSlidingWindowSize(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, sliding_window_size] = controls.GetInputSlidingWindowSize();
    if (has_new_input) {
        handler->SetWindowSize(sliding_window_size);
    }
}

inline void SynchronizeAdvance(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, advance] = controls.GetInputAdvance();
    if (has_new_input) {
        handler->SetAdvanceCount(advance);
    }
}

inline void SynchronizeHilbertDegree(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, hilbert_degree] = controls.GetInputHilbertDegree();
    if (has_new_input) {
        handler->SetHilbertDegree(hilbert_degree);
    }
}

inline void SynchronizeMaxMemory(IPmovesHandler* handler, IPmovesControlScene& controls) {
    const auto [has_new_input, max_memory] = controls.GetInputMaxMemory();
    if (has_new_input) {
        handler->SetMaxMemory(max_memory);
    }
}
}  // namespace

namespace controllers::ipmoves {

void Synchronize(UIManager* ui_manager, IPmovesHandler* handler) {
    auto& controls = ui_manager->GetIPmovesControlScene();
    SynchronizeProgress(handler, controls);
    SynchronizePuase(handler, controls);
    SynchronizeNewColor(handler, controls);
    SynchronizeOldColor(handler, controls);
    SynchronizeSlidingWindowSize(handler, controls);
    SynchronizeAdvance(handler, controls);
    SynchronizeHilbertDegree(handler, controls);
    SynchronizeMaxMemory(handler, controls);
    UpdateDetails(ui_manager, handler);
}

std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager, const std::string& filename) {
    auto& controls = ui_manager->GetIPmovesControlScene();
    std::unique_ptr<IPmovesHandler> handler = std::make_unique<IPmovesHandler>(filename);
    static const constexpr float kInitProgress = 0.0f;
    controls.SetProgressValue(kInitProgress);
    const auto state = controls.GetState();
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
    handler->SetProgress(state.progress);
    return handler;
}
}  // namespace controllers::ipmoves