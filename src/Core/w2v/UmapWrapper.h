#pragma once

#include <cstddef>

namespace umap {
void TrainEmbedding(size_t initial_dim, size_t objects_count, const double* input, size_t target_dim,
                    double* output, size_t neighbours, size_t iterations);
}