#pragma once

#include <GL/glew.h>

class VertexObject {
public:
    enum class MemoryMode {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
        STREAM = GL_STREAM_DRAW
    };

    /* The array will be copied into GPU memory, so it's ok to delete it right after the VertexObject was created */
    VertexObject(const GLfloat* data, GLsizei data_size, GLuint args_per_vertex, MemoryMode memory_mode);

    struct ArgPack {
        const GLfloat* data;
        GLsizei data_size;
        GLuint args_per_vertex;
        MemoryMode memory_mode;
    };

    explicit VertexObject(const ArgPack& arg_pack);

    VertexObject(const VertexObject& other) = delete;
    VertexObject& operator=(const VertexObject& other) = delete;

    virtual ~VertexObject();

    enum class DrawMode {
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY = GL_LINES_ADJACENCY,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
        PATHCES = GL_PATCHES
    };

    /* Draws the object into the buffer */
    virtual void Draw(DrawMode draw_mode) const;

    /* Sets the given attribure for the VAO */
    void SetAttribute(GLuint index, GLuint args_per_vertex, const void* offset) const;

    /* Loades new data into the VertexObject. It's not possible to override args_per_vertex, as it would take to reset the vertex attributes */
    void LoadData(const GLfloat* data, GLsizei data_size, MemoryMode memory_mode);

protected:
    GLsizei count_;
    GLsizei stride_;
    GLuint VBO_;
    GLuint VAO_;
};