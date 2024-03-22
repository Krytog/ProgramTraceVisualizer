#pragma once

#include "../../VertexObjects/VertexObjectIndexed.h"
#include "../../Shader/Shader.h"
#include "../IRenderable.h"

#include <glm/glm.hpp> 

class Cube : public IRenderable {
public:
	Cube();

	/* Renders the cube */
	void Render() const override;

	/* Sets the position of the cube */
	void SetTransform(const glm::mat4& position);

private:
	VertexObjectIndexed mesh_;
	Shader shader_;
};