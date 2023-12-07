#include "VertexObject.h"

VertexObject::VertexObject(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, MemoryMode memory_mode) {
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    const GLenum memory_mode_inner = (GLenum)memory_mode;
    glBufferData(GL_ARRAY_BUFFER, data_size, data, memory_mode_inner);
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);
    const GLuint attribute_offset = 0;
    glVertexAttribPointer(attribute_offset, args_per_vertex, GL_FLOAT, GL_FALSE, args_per_vertex * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    count_ = data_size / (args_per_vertex * sizeof(GLfloat));
}

VertexObject::VertexObject(const ArgPack& arg_pack) : VertexObject(arg_pack.data, arg_pack.data_size, arg_pack.args_per_vertex,
    arg_pack.memory_mode) {}

void VertexObject::Draw(DrawMode draw_mode) const {
    glBindVertexArray(VAO_);
    const GLenum draw_mode_inner = (GLenum)draw_mode;
    glDrawArrays(draw_mode_inner, 0, count_);
    glBindVertexArray(0);
}

VertexObject::~VertexObject() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}