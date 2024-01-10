#pragma once

#include "../../VertexObjects/VertexObjectIndexed.h"
#include "../../Shader/Shader.h"

#include <GLM/glm.hpp> 

class Cube {
public:
	Cube();

	/* Renders the cube */
	void Draw() const;

	/* Sets the position of the cube */
	void SetTransform(const glm::mat4& position);

private:
	VertexObjectIndexed mesh_;
public:
	Shader shader_;
};