#include "WaitingWidget.h"
#include <Graphics/Primitives/ShaderWidget/ShaderWidget.h>
#include <Resources/ShadersEmbed/WaitingWidgetShader.h>

WaitingWidget::WaitingWidget() : ShaderWidget(kWaitingWidgetShaderFrag) {
}
