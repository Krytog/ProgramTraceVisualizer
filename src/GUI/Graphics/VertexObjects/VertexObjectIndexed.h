#pragma once

#include "VertexObject.h"

class VertexObjectIndexed : public VertexObject {
public:
	/* The arrays will be copied into GPU memory, so it's ok to delete them right after the VertexObjectIndexed was created */
	VertexObjectIndexed(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, const GLuint* indices, 
						GLsizei indices_size, VertexObject::MemoryMode memory_mode);

	struct ArgPack {
		const GLfloat* data;
		GLsizei data_size;
		GLuint args_per_vertex;
		const GLuint* indices;
		GLsizei indices_size;
		VertexObject::MemoryMode memory_mode;
	};

	explicit VertexObjectIndexed(const ArgPack& arg_pack);

	~VertexObjectIndexed();

	/* Draws the object into the buffer */
	void Draw(VertexObject::DrawMode draw_mode) const override;

private:
	GLsizei indices_count_;
	GLuint EBO_;
};