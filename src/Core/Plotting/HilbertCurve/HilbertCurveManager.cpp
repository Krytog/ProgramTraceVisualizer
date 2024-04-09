#include "HilbertCurveManager.h"
#include <memory>
#include "Core/Plotting/HilbertCurve/HilbertCurve.h"

HilbertCurveManager& HilbertCurveManager::GetInstance() {
    static HilbertCurveManager instance;
    return instance;
}

void HilbertCurveManager::PrepareHilbertCurve(unsigned int degree) {
    HilbertCurveManager& manager = GetInstance();
    if (manager.curves_ready_.contains(degree)) {
        return;
    }
    manager.curves_ready_[degree] = std::make_unique<HilbertCurve2D>(degree);
}

void HilbertCurveManager::ReleaseHilbertCurve(unsigned int degree) {
    HilbertCurveManager& manager = GetInstance();
    if (manager.curves_ready_.contains(degree)) {
        return;
    }
    manager.curves_ready_.erase(degree);
}

HilbertCurve2D* HilbertCurveManager::GetHilbertCurve(unsigned int degree) {
    HilbertCurveManager& manager = GetInstance();
    if (manager.curves_ready_.contains(degree)) {
        return manager.curves_ready_.at(degree).get();
    }
    return nullptr;
}