#include "IPmovesHandler.h"

#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>
#include <sys/types.h>

#include <cstdint>
#include <limits>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <unordered_map>
#include "Core/Plotting/Plot2DMesh/Plot2DMesh.h"

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

inline float GetTemperature(size_t pos, size_t total) {
    return static_cast<float>(pos) / total;
}

}  // namespace

void IPmovesHandler::FindAndSetMaxMin() {
    const size_t file_size = data_buffer_.GetRawFileReader()->GetSize();
    const size_t total_addresses = file_size / sizeof(uintptr_t);
    const int threads = std::thread::hardware_concurrency();
    const size_t per_thread = total_addresses / threads;
    std::vector<std::thread> workers;
    workers.reserve(threads);
    std::mutex mutex;
    std::vector<uintptr_t> mins(threads, std::numeric_limits<uintptr_t>::max());
    std::vector<uintptr_t> maxs(threads, std::numeric_limits<uintptr_t>::min());
    const size_t max_memory = data_buffer_.GetMaxMemory();
    for (int i = 0; i < threads - 1; ++i) {
        workers.emplace_back(MinMaxWorker, MinMaxWorkerArgs{.mutex = &mutex,
                                                            .file = data_buffer_.GetRawFileReader(),
                                                            .chunk_size = max_memory,
                                                            .from = i * per_thread * sizeof(uintptr_t),
                                                            .to = (i + 1) * per_thread * sizeof(uintptr_t),
                                                            .min = mins.data() + i,
                                                            .max = maxs.data() + i});
    }
    workers.emplace_back(MinMaxWorker,
                         MinMaxWorkerArgs{.mutex = &mutex,
                                          .file = data_buffer_.GetRawFileReader(),
                                          .chunk_size = max_memory,
                                          .from = (threads - 1) * per_thread * sizeof(uintptr_t),
                                          .to = file_size,
                                          .min = mins.data() + (threads - 1),
                                          .max = maxs.data() + (threads - 1)});
    for (auto& worker : workers) {
        worker.join();
    }
    min_address_ = std::numeric_limits<uintptr_t>::max();
    max_address_ = std::numeric_limits<uintptr_t>::min();
    for (int i = 0; i < threads; ++i) {
        if (min_address_ > mins[i]) {
            min_address_ = mins[i];
        }
        if (max_address_ < maxs[i]) {
            max_address_ = maxs[i];
        }
    }
}

void IPmovesHandler::SetMaxMemory(size_t max_memory) {
    data_buffer_.SetMaxMemory(max_memory);
}

size_t IPmovesHandler::GetMaxMemory() const {
    return data_buffer_.GetMaxMemory();
}

void IPmovesHandler::SetProgress(float alpha) {
    sliding_window_.SetProgress(alpha);
}

float IPmovesHandler::GetProgress() const {
    return sliding_window_.GetProgress();
}

void IPmovesHandler::SetHilbertDegree(unsigned degree) {
    if (hilbert_degree_ == degree) {
        return;
    }
    hilbert_degree_ = degree;
    hilbert_curve_ = HilbertCurveManager::GetHilbertCurve(degree);
    const size_t cells = 1ull << degree;
    plot_ = std::make_shared<Plot2DMesh>(cells);
    cells_ = cells;
}

unsigned IPmovesHandler::GetHilbertDegree() const {
    return hilbert_degree_;
}

void IPmovesHandler::SetAdvanceCount(size_t count) {
    advance_count_ = count;
}

size_t IPmovesHandler::GetAdvanceCount() const {
    return advance_count_;
}

void IPmovesHandler::SetWindowSize(size_t window_size) {
    sliding_window_.SetSize(window_size);
}

size_t IPmovesHandler::GetSlidingWindowSize() const {
    return sliding_window_.GetWindowSize();
}

std::shared_ptr<Plot2DMesh> IPmovesHandler::GetPlot() const {
    return plot_;
}

IPmovesHandler::IPmovesHandler(const std::string& filename)
    : data_buffer_(filename), sliding_window_(data_buffer_) {
    const unsigned basic_hilbert_degree = 10;
    SetHilbertDegree(basic_hilbert_degree);

    const size_t basic_max_memory = 1ull << 20;
    SetMaxMemory(basic_max_memory);

    FindAndSetMaxMin();
    data_clamper_.SetBounds(min_address_, max_address_);

    const size_t basic_advance_count = 1;
    SetAdvanceCount(basic_advance_count);
}

std::vector<std::pair<uintptr_t, float>> IPmovesHandler::GetDataWithTemperatures() const {
    auto [begin, end, offset] = sliding_window_.GetWindowWithOffset();
    const size_t before_begin = std::max(offset, static_cast<ptrdiff_t>(0));
    std::vector<std::pair<uintptr_t, float>> output;
    output.reserve(sliding_window_.GetWindowSize());
    for (auto iter = begin; iter != end; ++iter) {
        const size_t pos = before_begin + (iter - begin);
        const float temperature = GetTemperature(pos, sliding_window_.GetWindowSize() - 1);
        const uintptr_t address = *iter;
        output.emplace_back(address, temperature);
    }
    return output;
}

std::vector<float> IPmovesHandler::GetPreparedDataForRendering(
    const std::vector<std::pair<uintptr_t, float>>& data_with_temperatures) const {
    std::unordered_map<size_t, float> cells_with_temperature;
    for (auto [address, temperature] : data_with_temperatures) {
        const float clamped = data_clamper_.GetClamped(address);
        const size_t seq_num = clamped * (cells_ * cells_ - 1);
        if (cells_with_temperature[seq_num] < temperature) {
            cells_with_temperature[seq_num] = temperature;
        }
    }
    std::vector<float> output;
    output.reserve(cells_with_temperature.size() * 3);
    for (auto [seq_num, temperature] : cells_with_temperature) {
        const auto point = hilbert_curve_->Seq2XY(seq_num);
        output.push_back(2 * point.x - 1);
        output.push_back(2 * point.y - 1);
        output.push_back(temperature);
    }
    return output;
}

void IPmovesHandler::LoadDataForRendering() const {
    const auto data_with_temperatures = GetDataWithTemperatures();
    const auto ready_data = GetPreparedDataForRendering(data_with_temperatures);
    plot_->LoadData(ready_data.data(), ready_data.size() * sizeof(GLfloat));
}

void IPmovesHandler::Pause() {
    is_paused_ = true;
}

void IPmovesHandler::Unpause() {
    is_paused_ = false;
}

void IPmovesHandler::Update() {
    if (is_paused_) {
        return;
    }
    if (!sliding_window_) {
        sliding_window_.Restart();
    }
    LoadDataForRendering();
    sliding_window_.Advance(advance_count_);
}
