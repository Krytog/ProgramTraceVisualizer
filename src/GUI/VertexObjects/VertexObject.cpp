#include "VertexObject.h"

VertexObject::VertexObject(const GLfloat* data, MemoryMode memory_mode) {
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    GLuint memory_mode_bits = -1;
    switch (memory_mode) {
        case MemoryMode::STATIC {
            memory_mode_bits = GL_STATIC_DRAW;
            break;
        }
        case MemoryMode::DYNAMIC {
            memory_mode_bits = GL_STATIC_DYNAMIC;
            break;
        }
        case MemoryMode::STREAM {
            memory_mode_bits = GL_STATIC_STREAM;
            break;
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, memory_mode_bits);
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);
    const GLuint attribute_offset = 0;
    const GLuint arg_count = 3;
    const GLvoid* data_offset = (GLvoid*)0;
    glVertexAttribPointer(attribute_offset, arg_count, GL_FLOAT, GL_FALSE, arg_count * sizeof(GLfloat), data_offset);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexObject::Draw() {
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}