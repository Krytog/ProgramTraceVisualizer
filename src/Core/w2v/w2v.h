#pragma once

#include <Core/Plotting/Plot2DMesh/Plot2DMesh.h>
#include <GUI/Graphics/Primitives/IRenderable.h>

#include <string>
#include <memory>
#include <vector>
#include <cassert>
#include <utility>
#include <limits>

class W2VHandler {
public:
    W2VHandler(const std::string& filename);

    /* Returns a pointer to the current plot */
    [[nodiscard]] const IRenderable* GetPlot() const;

    /* Sets the scale of the plot */
    void SetPlotSize(size_t cells);

    /* Returns the current plot size */
    [[nodiscard]] size_t GetPlotSize() const;

    /* Sets the current umap dimension. If changed, it recalculates the entire embedding, so it can be slow */
    void SetDimension(size_t dimension);

    /* Returns the current dimension of the umap embedding */
    [[nodiscard]] size_t GetDimension() const;

private:
    std::unique_ptr<Plot2DMesh> plot_;
    std::vector<double> w2v_embedding_;
    std::vector<double> umap_embedding_;
    size_t plot_size_;
    size_t initial_dim_;
    size_t objects_count_;
    size_t current_umap_dim_{0};

    /* Uses a w2v model on the input file, trains the model, then stores w2v_embedding_, initial_dim_ and
     * objects_count_ */
    void InitW2VEmbedding(const std::string& filename);

    /* Uses prepared w2v_embedding to calculate and store into umap_embedding_ a umap embedding of the
     * target_dim dimension */
    void SetUmapEmbedding(size_t target_dim);

    /* Returns the coordinates of the object at the given index */
    [[nodiscard]] std::vector<double> GetObjectAtIndex(size_t index) const;

    /* Return the minimum and the maximum for each coordinate in the embeddiing */
    [[nodiscard]] std::pair<std::vector<double>, std::vector<double>> GetMinMax() const;

    /* Returns data prepared to be passed into PlotMesh */
    [[nodiscard]] std::vector<float> GetPreparedData() const;
};
