#pragma once

#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/IPmovesHandler/AddressBuffer.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/SlidingWindow/SlidingWindowIteratorExtended.h>
#include <memory>

class IPmovesHandler {
public:
    IPmovesHandler(const std::string& filename);

    /* Sets the max amount of memory that can be used for caching */
    void SetMaxMemory(size_t max_memory);

    /* Returns the max amount of memory that can be used for caching */
    [[nodiscard]] size_t GetMaxMemory() const;

    /* Sets current progress (relative position of the sliding window in the file) */
    void SetProgress(float alpha);

    /* Returns current progress (relative position of the sliding window in the file)*/
    [[nodiscard]] float GetProgress() const;

    /* Sets the Hilbert Curve degree. If new_degree != old_degree, then a corresponding curve is aquired and prepared for usage */
    void SetHilbertDegree(unsigned degree);

    /* Returns the degree of the current Hilbert Curve */
    [[nodiscard]] unsigned GetHilbertDegree() const;

private:
    /* Goes through the entire file and sets max and min addresses */
    void FindAndSetMaxMin();

    /* Data from file */
    AddressBuffer data_buffer_;
    uintptr_t max_address_;
    uintptr_t min_address_;

    /* For graphics */
    DataClamper<uintptr_t> data_clamper_;
    HilbertCurve2D* hilbert_curve_;
    SlidingWindowIteratorExtended<AddressBuffer> sliding_window_;
    std::shared_ptr<Plot2DMesh> plot_;

    /* Parameters */
    size_t max_memory_;
    unsigned hilbert_degree_;
};