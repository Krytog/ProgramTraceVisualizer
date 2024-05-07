#pragma once

#include <cstddef>

namespace umap {

struct TrainParams {
    size_t neighbours{15};
    size_t epochs{300};
    float min_distance{0.01};
    float bandwidth{5};
    float local_conectivity{1};
};

void TrainEmbedding(size_t initial_dim, size_t objects_count, const double* input, size_t target_dim,
                    double* output, const TrainParams& train_params);
}