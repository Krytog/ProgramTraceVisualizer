#pragma once

#include "VertexObject.h"
#include <list>

class VertexObjectCustom : public VertexObject {
public:
	VertexObjectCustom(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, MemoryMode memory_mode);

	~VertexObjectCustom() override;

	/* Generates and prepares for usage a new vertex buffer. This VertexObject has the ownership of the generated buffer */
	GLuint GenerateNewVetexBuffer();

	/* Sets the given attribure for the VAO, using target_buffer as the buffer */
	void SetAttribute(GLuint target_buffer, GLuint index, GLuint args_per_vertex, const void* offset) const;

	/* Loades new data into the given target_buffer */
	void LoadData(GLuint target_buffer, const GLfloat* data, GLsizei data_size, MemoryMode memory_mode) const;

private:
	std::list<GLuint> generated_buffers_;
};