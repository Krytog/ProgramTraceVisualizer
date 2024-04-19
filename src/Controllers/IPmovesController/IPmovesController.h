#pragma once

class UIManager; // Forward-declaration
class IPmovesHandler; // Forward-declaration 

namespace controllers::ipmoves {
    void Synchronize(UIManager* ui_manager, IPmovesHandler* handler);
}