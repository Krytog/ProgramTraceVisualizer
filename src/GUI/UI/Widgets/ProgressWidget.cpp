#include "ProgressWidget.h"

#include <Resources/ShadersEmbed/ProgressWidgetShader.h>

#define PROGRESS_MODE_UNIFORM_NAME "progress_mode"

ProgressWidget::ProgressWidget(): ShaderWidget(kProgressWidgetShaderFrag) {
    GoToProgressMode();
}

void ProgressWidget::SetProgress(float progress) {
    if (is_in_progress_mode_) {
        SetTime(progress);
        return;
    }
    SetTime(timer_.EvaluateTime());
}

void ProgressWidget::GoToWaitingMode() {
    shader_.SetUniform(PROGRESS_MODE_UNIFORM_NAME, false);
    is_in_progress_mode_ = false;
}

void ProgressWidget::GoToProgressMode() {
    shader_.SetUniform(PROGRESS_MODE_UNIFORM_NAME, true);
    is_in_progress_mode_ = true;
}