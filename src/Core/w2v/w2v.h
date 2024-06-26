#pragma once

#include <Core/Plotting/PlotMesh/Plot2DMesh.h>
#include <Core/Plotting/PlotMesh/Plot3DMesh.h>
#include <GUI/Graphics/Primitives/IRenderable.h>
#include <GUI/UI/Widgets/ProgressWidget.h>

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <thread>
#include <atomic>

class W2VHandler {
public:
    struct Params {
        size_t dimension;
        float color[4];
        size_t neighbours;
        size_t epochs;
        size_t cells;
        float min_dist;
    };

    W2VHandler(const std::string& filename);

    W2VHandler(const W2VHandler& other) = delete;
    W2VHandler& operator=(const W2VHandler& other) = delete;

    ~W2VHandler();

    /* Returns a pointer to the current plot */
    [[nodiscard]] const IRenderable* GetPlot() const;

    /* Sets the scale of the plot */
    void SetPlotSize(size_t cells);

    /* Returns the current plot size */
    [[nodiscard]] size_t GetPlotSize() const;

    /* Sets the color of the plot */
    void SetColor(const float* color);

    /* Sets the current umap dimension. If changed, it recalculates the entire embedding, so it can be slow */
    void SetDimension(size_t dimension);

    /* Returns the current dimension of the umap embedding */
    [[nodiscard]] size_t GetDimension() const;

    /* Prepares all the embeddings needed for rendering */
    void StartPrepare(const std::string& filename);

    /* Returns true if W2VHandler is ready to use and false otherwise */
    [[nodiscard]] bool IsReady() const;

    /* Returns different word count from w2v embedding. The return value is only valid after parsing_progress_
     * is 100 */
    [[nodiscard]] size_t GetDifferentWordCount() const;

    /* Returns total word count from w2v embedding. The return value is only valid after parsing_progress_ is
     * 100 */
    [[nodiscard]] size_t GetTotalWordCount() const;

    /* Returns the current progress of parsing from w2v embedding */
    [[nodiscard]] float GetParsingProgress() const;

    /* Returns the current progress of training w2v model for the embedding */
    [[nodiscard]] float GetTrainingProgress() const;

    /* Updates the state of W2VHandler */
    void Update();

    /* Passes width and height to all the systems that need them */
    void SetViewPortSize(float width, float height);

    /* Starts recalculating umap mapping */
    void StartRecalculate(const Params& params);

    /* If current mode is 3d, then passes the data into Plot3DMesh to control the camera. Otherwise, does nothing */
    void PassMouseInput(float x, float y, float scroll);

private:
    std::unique_ptr<PlotMesh> plot_;
    Plot3DMesh* plot_as_plot3d_{nullptr};
    std::vector<double> w2v_embedding_;
    std::vector<double> umap_embedding_;
    size_t initial_dim_{0};
    size_t objects_count_{0};
    Params current_params_;

    std::atomic<bool> is_data_loaded_{false};

    std::unique_ptr<ProgressWidget> progress_wigdet_;
    std::unique_ptr<std::thread> worker_;
    std::atomic<bool> ready_{false};
    std::atomic<bool> is_umap_embedding_recalc_pending_{false};
    std::atomic<size_t> different_words_count_{0};
    std::atomic<size_t> total_words_count_{0};
    std::atomic<float> parsing_progress_{0};
    std::atomic<float> training_progress_{0};

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

    /* Loads embedding into plot for rendering */
    void LoadData();

    /* Recreates plot according to the current dimension in params */
    void RecreatePlot();
};
