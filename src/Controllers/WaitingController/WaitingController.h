#pragma once

class UIManager;      // forward-declaration
class WaitingWidget;  // forward-declaration

namespace controllers::waiting {
/* Updates the widget by passing actual time and ViewScene resolution into it */
void Update(UIManager* ui_manager, WaitingWidget* widget);
}  // namespace controllers::waiting