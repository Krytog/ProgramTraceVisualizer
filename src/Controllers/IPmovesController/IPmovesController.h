#pragma once

#include <memory>
#include <string>

class UIManager;       // Forward-declaration
class IPmovesHandler;  // Forward-declaration

namespace controllers::ipmoves {
/* Returns a new IPmovesHandler, initialized with args from UIManager and binded to the file named filename */
std::unique_ptr<IPmovesHandler> Initialize(UIManager* ui_manager, const std::string& filename);

/* Synchronizes states of UIManager and IPmovesHandler. In particular, it applies the user's input to
 * IPmovesHandler */
void Synchronize(UIManager* ui_manager, IPmovesHandler* handler);
}  // namespace controllers::ipmoves