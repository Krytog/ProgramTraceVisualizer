#pragma once

#include <memory>
#include <string>

class UIManager;   // Forward-declaration
class W2VHandler;  // Forward-declaration

namespace controllers::w2vhandler {
/* Returns a new W2VHandler, initialized with args from UIManager and binded to the file named filename */
std::unique_ptr<W2VHandler> Initialize(UIManager* ui_manager, const std::string& filename);

/* Synchronizes states of UIManager and W2VHandler. In particular, it loads relevant info into DetailsScene
 * and applies users info */
void Synchronize(UIManager* ui_manager, W2VHandler* handler);
}  // namespace controllers::w2vhandler
