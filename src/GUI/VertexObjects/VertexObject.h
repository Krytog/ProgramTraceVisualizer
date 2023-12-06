#pragma once

#include <GL/glew.h>

class VertexObject {
public:
    enum class MemoryMode {
        STATIC,
        DYNAMIC,
        STREAM
    };

    /* The array will be copied into GPU memory, so it's ok to delete it right after the VertexObject was created */
    VertexObject(const GLfloat* data, MemoryMode memory_mode);
    virtual ~VertexObjectBase();

    enum class DrawMode {
        GL_POINTS,
        GL_LINE_STRIP,
        GL_LINE_LOOP,
        GL_LINES,
        GL_LINE_STRIP_ADJACENCY,
        GL_LINES_ADJACENCY,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP_ADJACENCY,
        GL_TRIANGLES_ADJACENCY,
        GL_PATCHES
    };

    /* Draws the object into the buffer */
    void Draw(DrawMode draw_mode) const;

protected:
    GLenum GetGLenumFromMemoryMode(MemoryMode memory_mode) const;
    GLenum GetGLenumFromDrawMode(DrawMode draw_mode) const;

private:
    GLuint VBO_;
    GLuint VAO_;
    GLsizei count_;
};