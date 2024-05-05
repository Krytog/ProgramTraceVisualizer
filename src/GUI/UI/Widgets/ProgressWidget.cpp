#include "ProgressWidget.h"

#include <Resources/ShadersEmbed/ProgressWidgetShader.h>

ProgressWidget::ProgressWidget(): ShaderWidget(kProgressWidgetShaderFrag) {}

void ProgressWidget::SetProgress(float progress) {
    SetTime(progress);
}
