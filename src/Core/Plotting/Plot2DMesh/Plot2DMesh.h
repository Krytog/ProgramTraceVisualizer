#pragma once

#include <GUI/Graphics/VertexObjects/VertexObject.h>
#include <GUI/Graphics/Shader/Shader.h>
#include <GUI/Graphics/Primitives/IRenderable.h>

#include <cstddef>

class Plot2DMesh : public IRenderable {
public:
	Plot2DMesh(const size_t grid_size);

	/* Renders the plot mesh */
	void Render() const override;

	/* Sets new grid_size */
	void SetGridSize(const size_t grid_size);

	/* Returns the current grid_size */
	[[nodiscard]] size_t GetGridSize() const;

	/* Loads new data to be rendered. Data format is as follows: [x, y, temperature] per vertex, for a total of grid_size * grid_size vertices */
	void LoadData(const GLfloat* data, GLsizei data_size);

	/* Sets the color for the newest elements */
    void SetColorForNewest(const float* color);

    /* Sets the color for the oldest elements */
    void SetColorForOldest(const float* color);


private:
	size_t grid_size_;

	Shader shader_;
	VertexObject mesh_;
};