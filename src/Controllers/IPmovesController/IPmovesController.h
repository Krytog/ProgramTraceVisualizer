#pragma once

#include <memory>
#include <string>

class UIManager;       // Forward-declaration
class IPmovesHandler;  // Forward-declaration

namespace controllers::ipmoves {
std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager, const std::string& filename);

void Synchronize(UIManager* ui_manager, IPmovesHandler* handler);

void SetCallbacks(std::unique_ptr<IPmovesHandler>* handler_keeper, UIManager* ui_manager);

}  // namespace controllers::ipmoves