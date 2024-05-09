#include "W2VController.h"

#include <Core/w2v/w2v.h>
#include <GUI/UI/UIManager/UIManager.h>
#include "Graphics/Primitives/IRenderable.h"
#include "UI/Scenes/DetailsScene/DetailsScene.h"
#include "UI/Scenes/W2VControlScene/W2VControlScene.h"

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

bool IsTrainingFinished(const W2VHandler* handler) {
    return handler->GetTrainingProgress() >= 100.0f;
}

void UpdateUmapEmbeddingStatus(DetailsScene* details, const W2VHandler* handler) {
    if (handler->IsReady()) {
        details->PushLine("Umap is ready and rendered!");
    } else {
        details->PushLine("Waiting for umap embedding...");
    }
}

void SynchronizeDetails(UIManager* ui_manager, W2VHandler* handler) {
    DetailsScene* details = &ui_manager->GetDetailsScene();
    ClearDetails(details);
    UpdateParsingProgress(details, handler);
    if (!IsParsingFinished(handler)) {
        return;
    }
    UpdateDifferentWordsCount(details, handler);
    UpdateTotalWordsCount(details, handler);
    UpdateTrainingProgress(details, handler);
    if (!IsTrainingFinished(handler)) {
        return;
    }
    UpdateUmapEmbeddingStatus(details, handler);
}

static const IRenderable* prev_plot = nullptr;
static bool is_first_time_ready = true;

void SynchronizePlot(UIManager* ui_manager, W2VHandler* handler) {
    if (handler->IsReady()) {
        if (is_first_time_ready) {
            is_first_time_ready = false;
            ui_manager->GetViewScene().RemoveObject(prev_plot);
            ui_manager->GetViewScene().AddObject(handler->GetPlot());
            ui_manager->GetW2VControlScene().SetRealDimension(handler->GetDimension());
            ui_manager->GetW2VControlScene().OnRecalculationFinished();
        }
    } else {
        if (!is_first_time_ready) {
            ui_manager->GetViewScene().RemoveObject(prev_plot);
            ui_manager->GetViewScene().AddObject(handler->GetPlot());
            is_first_time_ready = true;
        }
    }
    const auto [width, height] = ui_manager->GetViewScene().GetViewPortSize();
    handler->SetViewPortSize(width, height);
    prev_plot = handler->GetPlot();
}

W2VHandler::Params GetW2VParams(const W2VControlScene::W2VControlState& state) {
    W2VHandler::Params params{.dimension = static_cast<size_t>(state.dimension),
                              .neighbours = static_cast<size_t>(state.neighbours),
                              .epochs = static_cast<size_t>(state.epochs),
                              .cells = static_cast<size_t>(state.cells),
                              .min_dist = state.min_dist};
    for (int i = 0; i < 4; ++i) {
        params.color[i] = state.color[i];
    }
    return params;
}

void ApplyInput(UIManager* ui_manager, W2VHandler* handler) {
    auto* controlls = &ui_manager->GetW2VControlScene();
    const auto [is_new, is_pending] = controlls->IsPendingRecalculation();
    if (is_new && is_pending) {
        if (!IsTrainingFinished(handler)) { // user pressed the button too early
            controlls->OnRecalculationFinished(); // resets the button
        }
        const auto controlls_state = controlls->GetState();
        const auto params = GetW2VParams(controlls_state);
        handler->StartRecalculate(params);
        controlls->SetRealDimension(0);
    }
    if (handler->IsReady()) {
        handler->SetPlotSize(controlls->GetInputCells());
        handler->SetColor(controlls->GetInputColor());
    }
    const auto* view_port = &ui_manager->GetViewScene();
    const auto [delta_x, delta_y] = view_port->GetMouseDelta();
    const auto scroll = view_port->GetMouseScroll();
    if (view_port->IsMousePressed()) {
        handler->PassMouseInput(delta_x, delta_y, scroll);
    } else {
        handler->PassMouseInput(0, 0, scroll);
    }
 }

}  // namespace

namespace controllers::w2vhandler {

void Synchronize(UIManager* ui_manager, W2VHandler* handler) {
    ApplyInput(ui_manager, handler);
    handler->Update();
    SynchronizePlot(ui_manager, handler);
    SynchronizeDetails(ui_manager, handler);
}

std::unique_ptr<W2VHandler> Initialize(UIManager* ui_manager, const std::string& filename) {
    prev_plot = nullptr;
    is_first_time_ready = true;
    return std::make_unique<W2VHandler>(filename);
}
}  // namespace controllers::w2vhandler