#pragma once

#include <Utils/FileReader/FileReader.h>
#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <Core/Plotting/HilbertCurve/HilbertCurve.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <memory>

class IPmovesHandler {
public:
    IPmovesHandler(const std::string& filename);

private:
    FileReader file_;

    void FindAndSetMaxMin();

    /* Data from file */
    size_t max_chunk_size_;
    std::vector<uintptr_t> data_chunk_;
    uintptr_t max_address_;
    uintptr_t min_address_;

    /* For graphics */
    std::unique_ptr<DataClamper<uintptr_t>> data_clamper_;
    HilbertCurve2D* hilbert_curve_;
    std::shared_ptr<Plot2DMesh> plot_;
};