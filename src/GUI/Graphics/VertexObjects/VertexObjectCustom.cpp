#include "VertexObjectCustom.h"

VertexObjectCustom::VertexObjectCustom(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, MemoryMode memory_mode) :
	VertexObject(data, data_size, args_per_vertex, memory_mode) {}

VertexObjectCustom::~VertexObjectCustom() {
	for (const GLuint buffer : generated_buffers_) {
		glDeleteBuffers(1, &buffer);
	}
}

GLuint VertexObjectCustom::GenerateNewVetexBuffer() {
	GLuint new_buffer;
	glGenBuffers(1, &new_buffer);
	generated_buffers_.push_back(new_buffer);
	return new_buffer;
}

void VertexObjectCustom::SetAttribute(GLuint target_buffer, GLuint index, GLuint args_per_vertex, const void* offset) const {
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, target_buffer);
	glVertexAttribPointer(index, args_per_vertex, GL_FLOAT, GL_FALSE, args_per_vertex * sizeof(GL_FLOAT), offset);
	glEnableVertexAttribArray(index);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexObjectCustom::LoadData(GLuint target_buffer, const GLfloat* data, GLsizei data_size, MemoryMode memory_mode) const {
	glBindBuffer(GL_ARRAY_BUFFER, target_buffer);
	const GLenum memory_mode_inner = static_cast<GLenum>(memory_mode);
	glBufferData(GL_ARRAY_BUFFER, data_size, data, memory_mode_inner);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}