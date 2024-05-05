#include "WaitingController.h"

#include <GUI/UI/UIManager/UIManager.h>
#include <GUI/UI/Widgets/WaitingWidget.h>
#include <Utils/LightTimer/LightTimer.h>

namespace controllers::waiting {
void Update(UIManager* ui_manager, WaitingWidget* widget) {
    static LightTimer timer;
    widget->SetTime(timer.EvaluateTime());
    const auto [width, height] = ui_manager->GetViewScene().GetViewPortSize();
    widget->SetResolution(width, height);
}
}  // namespace controllers::waiting