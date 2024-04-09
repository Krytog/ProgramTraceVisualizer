#pragma once

#include <unordered_map>
#include <memory>
#include "HilbertCurve.h"

class HilbertCurveManager {
public:
    /* Calculates a Hilbert curve and stores it into a buffer */
    static void PrepareHilbertCurve(unsigned degree);

    /* Releases the given Hilber curve (if it's present) and frees its memory */
    static void ReleaseHilbertCurve(unsigned degree);

    /* Returns a pointer to the desired HilberCurve (or nullptr if there's no such curve ready)*/
    static HilbertCurve2D* GetHilbertCurve(unsigned degree);
private:
    /* It's a singlton, the object is used only inside public static functions and is never exposed to a user */
    HilbertCurveManager() = default;

    static HilbertCurveManager& GetInstance();

public:
    /* Here we use std::unique_ptr<HilbertCurve2D> instead of just HilbertCurve2D to avoid invalidatios of returned pointers because of map reallocs*/
    std::unordered_map<unsigned, std::unique_ptr<HilbertCurve2D>> curves_ready_;
};