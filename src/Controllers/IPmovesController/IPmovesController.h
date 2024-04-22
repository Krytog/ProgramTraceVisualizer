#pragma once

#include <memory>

class UIManager;       // Forward-declaration
class IPmovesHandler;  // Forward-declaration

namespace controllers::ipmoves {
std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager);

void Synchronize(UIManager* ui_manager, IPmovesHandler* handler);
}