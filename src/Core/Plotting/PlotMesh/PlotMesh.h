#pragma once

#include <GUI/Graphics/VertexObjects/VertexObject.h>
#include <GUI/Graphics/Shader/Shader.h>
#include <GUI/Graphics/Primitives/IRenderable.h>

#include <cstddef>

class PlotMesh : public IRenderable {
public:
    PlotMesh(const VertexObject::ArgPack& vertex_obj_args, const Shader::ShaderConfig& shader_config);

    virtual ~PlotMesh();

    /* Renders the plot mesh */
    virtual void Render() const override;

    /* Sets new grid_size */
    void SetGridSize(const size_t grid_size);

    /* Returns the current grid_size */
    [[nodiscard]] size_t GetGridSize() const;

    /* Loads new data to be rendered. Data format is as follows: [<vertex coords>, temperature]  per vertex,
     * for a total of grid_size * grid_size vertices */
    void LoadData(const GLfloat* data, GLsizei data_size);

    /* Sets the color for the elements with the highest temperature */
    void SetHighColor(const float* color);

    /* Sets the color for the elements with the lowest temperature */
    void SetLowColor(const float* color);

protected:
    size_t grid_size_{1};

    Shader shader_;
    VertexObject mesh_;
};