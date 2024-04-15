#include "IPmovesHandler.h"

#include <Core/Plotting/HilbertCurve/HilbertCurveManager.h>

#include <cstdint>
#include <limits>
#include <thread>
#include <mutex>
#include <vector>

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
}

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
        workers.emplace_back(MinMaxWorkerArgs{
            .mutex = &mutex,
            .file = data_buffer_.GetRawFileReader(),
            .chunk_size = max_memory,
            .from = i * per_thread * sizeof(uintptr_t),
            .to = (i + 1) * per_thread * sizeof(uintptr_t),
            .min = mins.data() + i,
            .max = maxs.data() + i
        });
    }
    workers.emplace_back(MinMaxWorkerArgs{
        .mutex = &mutex,
        .file = data_buffer_.GetRawFileReader(),
        .chunk_size = max_memory,
        .from = (threads - 1) * per_thread * sizeof(uintptr_t),
        .to = file_size,
        .min = mins.data() + (threads - 1),
        .max = maxs.data() + (threads - 1)
    });
    for (auto& worker : workers) {
        worker.join();
    }
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
    hilbert_degree_ = degree;
    hilbert_curve_ = HilbertCurveManager::GetHilbertCurve(degree);
}

unsigned IPmovesHandler::GetHilbertDegree() const {
    return hilbert_degree_;
}
