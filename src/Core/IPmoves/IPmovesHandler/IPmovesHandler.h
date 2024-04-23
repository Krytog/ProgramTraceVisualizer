#pragma once

#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/IPmovesHandler/AddressBuffer.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/IPmoves/SlidingWindow/SlidingWindowIteratorExtended.h>

class IPmovesHandler {
public:
    IPmovesHandler(const std::string& filename);

    /* Sets the max amount of memory that can be used for caching */
    void SetMaxMemory(size_t max_memory);

    /* Returns the max amount of memory that can be used for caching */
    [[nodiscard]] size_t GetMaxMemory() const;

    /* Sets the sliding window size */
    void SetWindowSize(size_t window_size);

    [[nodiscard]] size_t GetSlidingWindowSize() const;

    /* Sets current progress (relative position of the sliding window in the file) */
    void SetProgress(float alpha);

    /* Returns current progress (relative position of the sliding window in the file) */
    [[nodiscard]] float GetProgress() const;

    /* Sets the Hilbert Curve degree. If new_degree != old_degree, then a new curve is created */
    void SetHilbertDegree(unsigned degree);

    /* Returns the degree of the current Hilbert Curve */
    [[nodiscard]] unsigned GetHilbertDegree() const;

    /* Sets the number by which the sliding window moves each update */
    void SetAdvanceCount(size_t count);

    /* Returns the number by which the sliding window moves each update */
    [[nodiscard]] size_t GetAdvanceCount() const;

    /* Returns a shared_ptr to the current plot */
    [[nodiscard]] const Plot2DMesh* GetPlot() const;

    /* Sets the color for the newest elements */
    void SetColorForNewest(const float* color);

    /* Sets the color for the oldest elements */
    void SetColorForOldest(const float* color);

    /* Returns a reference to an array of current addresses in the sliding window */
    [[nodiscard]] const std::vector<uintptr_t>& GetCurrentAddresses() const;

    /* Pauses the sliding window */
    void Pause();

    /* Resumes the movement of the sliding window */
    void Unpause();

    /* Updates the IPMH for the next frame. In particular, this method moves the sliding window, prepares data
       and loads it into mesh_plot */
    void Update();

private:
    /* Goes through the entire file and sets max and min addresses */
    void FindAndSetMaxMin();

    /* Returns data from the sliding window with calculated temperatures. The newer an address is, the higher
     * its temperature is */
    [[nodiscard]] std::vector<std::pair<uintptr_t, float>> GetDataWithTemperatures() const;

    /* Returns data in a format ready to be loaded into PlotMesh. Takes data with prepared temperatures */
    [[nodiscard]] std::vector<float> GetPreparedDataForRendering(
        const std::vector<std::pair<uintptr_t, float>>& data_with_temperatures) const;

    /* Reads, prepares and loads data from the sliding window into the PlotMesh. Should be called exactly once
     * per update */
    void LoadDataForRendering();

    /* Fills an inner buffer with current addresses. Then it can be accessed from outside to calculate different metrics */
    void FillCurrentAddresses();

    /* Data from file */
    AddressBuffer data_buffer_;
    uintptr_t max_address_;
    uintptr_t min_address_;
    std::vector<uintptr_t> current_addresses_;

    /* For graphics */
    DataClamper<uintptr_t> data_clamper_;
    HilbertCurve2D* hilbert_curve_;
    SlidingWindowIteratorExtended<AddressBuffer> sliding_window_;
    Plot2DMesh plot_;

    /* Parameters */
    unsigned hilbert_degree_;
    size_t cells_;
    size_t advance_count_;
    bool is_paused_{false};
};