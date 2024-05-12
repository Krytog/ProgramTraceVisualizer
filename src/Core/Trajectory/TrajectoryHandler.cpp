#include "TrajectoryHandler.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include "Graphics/Primitives/IRenderable.h"
#include <Core/Plotting/PlotMesh/Plot2DMesh.h>
#include <Core/IPmoves/Clamping/DataClamper.h>
#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>

namespace {
struct MinMaxWorkerArgs {
    std::mutex* mutex;
    FileReader* file;
    size_t chunk_size;
    size_t from;
    size_t to;
    uintptr_t* min;
    uintptr_t* max;
};

void MinMaxWorker(MinMaxWorkerArgs args) {
    while (args.from < args.to) {
        const size_t bytes_to_read = std::min(args.to - args.from, args.chunk_size);
        args.mutex->lock();
        auto [data_chunk, real_size] = args.file->GetChunk(args.from, bytes_to_read);
        args.mutex->unlock();
        size_t bytes_handled = 0;
        for (size_t i = 0; i < real_size; i += sizeof(uintptr_t)) {
            const uintptr_t address = *reinterpret_cast<uintptr_t*>(data_chunk.get() + i);
            if (address < *args.min) {
                *args.min = address;
            }
            if (address > *args.max) {
                *args.max = address;
            }
            bytes_handled = (i + sizeof(uintptr_t));
        }
        args.from += bytes_handled;
    }
}

static const constexpr size_t kDefaultMaxMemory = (1ull << 20);

static constexpr float GetColor(size_t position, size_t total) {
    return static_cast<float>(position) / total;
}

}  // namespace

void TrajectoryHandler::SetMinMax() {
    const size_t file_size = file_.GetSize();
    const size_t total_addresses = file_size / sizeof(uintptr_t);
    const int threads = std::thread::hardware_concurrency();
    const size_t per_thread = total_addresses / threads;
    std::vector<std::thread> workers;
    workers.reserve(threads);
    std::mutex mutex;
    std::vector<uintptr_t> mins(threads, std::numeric_limits<uintptr_t>::max());
    std::vector<uintptr_t> maxs(threads, std::numeric_limits<uintptr_t>::min());
    const size_t max_memory = kDefaultMaxMemory;
    for (int i = 0; i < threads - 1; ++i) {
        workers.emplace_back(MinMaxWorker, MinMaxWorkerArgs{.mutex = &mutex,
                                                            .file = &file_,
                                                            .chunk_size = max_memory,
                                                            .from = i * per_thread * sizeof(uintptr_t),
                                                            .to = (i + 1) * per_thread * sizeof(uintptr_t),
                                                            .min = mins.data() + i,
                                                            .max = maxs.data() + i});
    }
    workers.emplace_back(MinMaxWorker,
                         MinMaxWorkerArgs{.mutex = &mutex,
                                          .file = &file_,
                                          .chunk_size = max_memory,
                                          .from = (threads - 1) * per_thread * sizeof(uintptr_t),
                                          .to = file_size,
                                          .min = mins.data() + (threads - 1),
                                          .max = maxs.data() + (threads - 1)});
    for (auto& worker : workers) {
        worker.join();
    }
    for (int i = 0; i < threads; ++i) {
        if (min_ > mins[i]) {
            min_ = mins[i];
        }
        if (max_ < maxs[i]) {
            max_ = maxs[i];
        }
    }
}

TrajectoryHandler::TrajectoryHandler(const std::string& filename): file_(filename), plot_(std::make_unique<Plot2DMesh>(1)) {
    SetMinMax();
    FillAddresses();
}

void TrajectoryHandler::SetDimension(size_t dimension) {
    if (current_dimension_ == dimension) {
        return;
    }
    current_dimension_ = dimension;
    if (dimension == 3) {
        plot_ = std::make_unique<Plot3DMesh>(cells_);
        plot_as_3d_mesh_ = dynamic_cast<Plot3DMesh*>(plot_.get());
        Create3DVisualization();
    } else if (dimension == 2) {
        plot_ = std::make_unique<Plot2DMesh>(cells_);
        plot_as_3d_mesh_ = nullptr;
        Create2DVisualization();
    }
}

void TrajectoryHandler::FillAddresses() {
    addresses_.clear();
    size_t file_offset = 0;
    while (file_offset < file_.GetSize()) {
        auto [data_chunk, real_size] = file_.GetChunk(file_offset, kDefaultMaxMemory);
        size_t bytes_handled = 0;
        for (size_t i = 0; i < real_size; i += sizeof(uintptr_t)) {
            const uintptr_t address = *reinterpret_cast<uintptr_t*>(data_chunk.get() + i);
            addresses_.push_back(address);
            bytes_handled = (i + sizeof(uintptr_t));
        }
        file_offset += bytes_handled;
    }
}

void TrajectoryHandler::Create2DVisualization() {
    AbsoluteDataClamper<uintptr_t> clamper(min_, max_);
    std::vector<float> data;
    data.reserve(addresses_.size() * 3);
    for (size_t i = 0; i < addresses_.size(); ++i) {
        const float x = 2 * clamper.GetClamped(addresses_[i]) - 1;
        const float y = 2 * (static_cast<float>(i) / addresses_.size()) - 1;
        const float color = GetColor(i, addresses_.size());
        data.push_back(x);
        data.push_back(y);
        data.push_back(color);
    }
    plot_->LoadData(data.data(), data.size() * sizeof(float));
}

void TrajectoryHandler::Create3DVisualization() {
    AbsoluteDataClamper<uintptr_t> clamper(min_, max_);
    const auto* hilbert_curve = HilbertCurveManager::GetHilbertCurve(hilbert_degree_);
    if (layer_offset_ <= 0) { // not set
        layer_offset_ = 2.0f / addresses_.size();
    }
    const float total_height = layer_offset_ * addresses_.size();
    std::vector<float> data;
    data.reserve(addresses_.size() * 4);
    for (size_t i = 0; i < addresses_.size(); ++i) {
        const float clamped = clamper.GetClamped(addresses_[i]);
        const size_t seq_num = clamped * (cells_ * cells_ - 1);
        const auto point = hilbert_curve->Seq2XY(seq_num);
        const auto z = (static_cast<float>(i) / addresses_.size() - 0.5f) * total_height;
        const float color = GetColor(i, addresses_.size());
        data.push_back(2 * point.x - 1);
        data.push_back(2 * point.y - 1);
        data.push_back(z);
        data.push_back(color);
    }
    plot_->LoadData(data.data(), data.size() * sizeof(float));
}

void TrajectoryHandler::SetBeginColor(const float* color) {
    plot_->SetLowColor(color);
}

void TrajectoryHandler::SetEndColor(const float* color) {
    plot_->SetHighColor(color);
}

size_t TrajectoryHandler::GetDimension() const {
    return current_dimension_;
}

IRenderable* TrajectoryHandler::GetPlot() const {
    return plot_.get();
}

void TrajectoryHandler::SetViewPortSize(float width, float height) {
    if (plot_as_3d_mesh_) {
        plot_as_3d_mesh_->SetViewPortSize(width, height);
    }
}

void TrajectoryHandler::PassMouseInput(float x, float y, float scroll) {
    if (plot_as_3d_mesh_) {
        plot_as_3d_mesh_->HandleMouseMove(x, y);
        plot_as_3d_mesh_->HandleMouseScroll(scroll);
    }
}

void TrajectoryHandler::SetHilbertDegree(size_t degree) {
    if (hilbert_degree_ == degree) {
        return;
    }
    hilbert_degree_ = degree;
    cells_ = (1ull << degree);
    plot_->SetGridSize(cells_);
}
