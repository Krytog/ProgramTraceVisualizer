#include "W2VController.h"

#include <Core/w2v/w2v.h>
#include <GUI/UI/UIManager/UIManager.h>
#include "Graphics/Primitives/IRenderable.h"
#include "UI/Scenes/DetailsScene/DetailsScene.h"

#include <memory>

namespace {
void ClearDetails(DetailsScene* details) {
    while (!details->GetInnerBuffer().empty()) {
        details->PopFrontLine();
    }
}

void UpdateParsingProgress(DetailsScene* details, const W2VHandler* handler) {
    const auto parsing_progress = handler->GetParsingProgress();
    details->PushLine("Current parsing progress: " + std::to_string(parsing_progress) + "%");
}

void UpdateTrainingProgress(DetailsScene* details, const W2VHandler* handler) {
    const auto training_progress = handler->GetTrainingProgress();
    details->PushLine("Current training progress: " + std::to_string(training_progress) + "%");
}

void UpdateDifferentWordsCount(DetailsScene* details, const W2VHandler* handler) {
    const auto different_words_count = handler->GetDifferentWordCount();
    details->PushLine("Unique addresses: " + std::to_string(different_words_count));
}

void UpdateTotalWordsCount(DetailsScene* details, const W2VHandler* handler) {
    const auto total_words_count = handler->GetTotalWordCount();
    details->PushLine("Total addresses: " + std::to_string(total_words_count));
}

bool IsParsingFinished(const W2VHandler* handler) {
    return handler->GetParsingProgress() >= 100.0f;
}

static const IRenderable* prev_plot = nullptr;
static bool is_first_time_ready = true;
}  // namespace

namespace controllers::w2vhandler {

void Synchronize(UIManager* ui_manager, W2VHandler* handler) {
    handler->Update();
    if (handler->IsReady()) {
        if (is_first_time_ready) {
            is_first_time_ready = false;
            ui_manager->GetViewScene().RemoveObject(prev_plot);
            ui_manager->GetViewScene().AddObject(handler->GetPlot());
        }
        // apply input;
    }
    prev_plot = handler->GetPlot();
    DetailsScene* details = &ui_manager->GetDetailsScene();
    ClearDetails(details);
    UpdateParsingProgress(details, handler);
    if (!IsParsingFinished(handler)) {
        return;
    }
    UpdateDifferentWordsCount(details, handler);
    UpdateTotalWordsCount(details, handler);
    UpdateTrainingProgress(details, handler);
}

std::unique_ptr<W2VHandler> Initialize(UIManager* ui_manager, const std::string& filename) {
    prev_plot = nullptr;
    is_first_time_ready = true;
    return std::make_unique<W2VHandler>(filename);
}
}  // namespace controllers::w2vhandler