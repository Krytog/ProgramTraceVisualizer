#pragma once

#include <Graphics/VertexObjects/VertexObject.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Primitives/IRenderable.h>

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

private:
	size_t grid_size_;

	Shader shader_;
	VertexObject mesh_;
};