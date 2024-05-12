#pragma once

#include <memory>
#include <string>

class UIManager;   // Forward-declaration
class TrajectoryHandler;  // Forward-declaration

namespace controllers::trajectory {
/* Returns a new TrajectoryHandler, initialized with args from UIManager and binded to the file named filename */
std::unique_ptr<TrajectoryHandler> Initialize(UIManager* ui_manager, const std::string& filename);

/* Synchronizes states of UIManager and TrajectoryHandler. In particular, it loads relevant info into DetailsScene
 * and applies user's input */
void Synchronize(UIManager* ui_manager, TrajectoryHandler* handler);
}  // namespace controllers::trajectory
