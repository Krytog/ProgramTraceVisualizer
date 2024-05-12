#pragma once

#include <Core/Plotting/PlotMesh/Plot3DMesh.h>
#include <Utils/FileReader/FileReader.h>

#include <cstdint>
#include <string>
#include <memory>
#include <limits>
#include <vector>

class TrajectoryHandler {
public:
    TrajectoryHandler(const std::string& filename);

    /* Returns a pointer to the current plot */
    [[nodiscard]] IRenderable* GetPlot() const;

    /* Sets the color for the first elements */
    void SetBeginColor(const float* color);

    /* Sets the color for the last elements */
    void SetEndColor(const float* color);

    /* Passes viewport size to the underlying plot to correctly resize things */
    void SetViewPortSize(float width, float height);

    /* Sets the dimension of the visualization */
    void SetDimension(size_t dimension);

    /* Returns the current dimension */
    [[nodiscard]] size_t GetDimension() const;

    /* If it's 3d mode, then passes input to the Camera3D of Plot3DMesh, otherwise does nothing */
    void PassMouseInput(float x, float y, float scroll);

    /* Sets the hilbert degree. Only usable in 3D mode */
    void SetHilbertDegree(size_t degree);

private:
    void SetMinMax();
    void FillAddresses();

    void Create2DVisualization();

    void Create3DVisualization();

    FileReader file_;
    uintptr_t min_{std::numeric_limits<uintptr_t>::max()};
    uintptr_t max_{std::numeric_limits<uintptr_t>::min()};
    std::vector<uintptr_t> addresses_;

    std::unique_ptr<PlotMesh> plot_;
    Plot3DMesh* plot_as_3d_mesh_{nullptr};
    size_t cells_{0};
    size_t current_dimension_{0};
    float layer_offset_{0};
    size_t hilbert_degree_{0};
};