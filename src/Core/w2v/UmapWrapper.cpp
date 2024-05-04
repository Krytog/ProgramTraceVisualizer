#include "UmapWrapper.h"

#include <umap/include/umappp/Umap.hpp>

namespace umap {
void TrainEmbedding(size_t initial_dim, size_t objects_count, const double* input, size_t target_dim,
                    double* output) {
    umappp::Umap x;
    x.set_num_threads(std::thread::hardware_concurrency());
    x.set_parallel_optimization(true);
    x.set_num_neighbors(20);
    x.set_num_epochs(50);
    auto proxy = x.initialize(initial_dim, objects_count, input, target_dim, output);
    for (int iter = 0; iter < 50; ++iter) {
        proxy.run(iter);
    }
}
}  // namespace umap