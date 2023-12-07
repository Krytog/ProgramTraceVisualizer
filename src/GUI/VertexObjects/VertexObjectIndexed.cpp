#include "VertexObjectIndexed.h"

VertexObjectIndexed::VertexObjectIndexed(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, const GLuint* indices,
										 GLsizei indices_size, VertexObject::MemoryMode memory_mode) : 
	VertexObject(data, data_size, args_per_vertex, memory_mode) {
	glGenBuffers(1, &EBO_);
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	const GLenum memory_mode_inner = (GLenum)memory_mode;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, memory_mode_inner);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	indices_count_ = indices_size / sizeof(GLuint);
}

VertexObjectIndexed::VertexObjectIndexed(const ArgPack& arg_pack) : VertexObjectIndexed(arg_pack.data, arg_pack.data_size,
	arg_pack.args_per_vertex, arg_pack.indices, arg_pack.indices_size, arg_pack.memory_mode) {}

void VertexObjectIndexed::Draw(VertexObject::DrawMode draw_mode) const {
	glBindVertexArray(VAO_);
	const GLenum draw_mode_inner = (GLenum)draw_mode;
	glDrawElements(draw_mode_inner, indices_count_, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

VertexObjectIndexed::~VertexObjectIndexed() {
	glDeleteBuffers(1, &EBO_);
}
