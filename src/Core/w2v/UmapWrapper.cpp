#include "UmapWrapper.h"

#include <umap/include/umappp/Umap.hpp>

namespace umap {
void TrainEmbedding(size_t initial_dim, size_t objects_count, const double* input, size_t target_dim,
                    double* output, const TrainParams& params) {
    umappp::Umap x;
    x.set_num_threads(std::thread::hardware_concurrency());
    x.set_parallel_optimization(true);
    x.set_num_neighbors(params.neighbours);
    x.set_num_epochs(params.epochs);
    x.set_bandwidth(params.bandwidth);
    x.set_min_dist(params.min_distance);
    x.set_local_connectivity(params.local_conectivity);
    auto proxy = x.initialize(initial_dim, objects_count, input, target_dim, output);
    proxy.run(params.epochs);
}
}  // namespace umap