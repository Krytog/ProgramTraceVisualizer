#pragma once

#include <GL/glew.h>

class RenderBuffer {
public:
	RenderBuffer(GLint width, GLint height);
	~RenderBuffer();

	/* Binds this buffer as the current render location */
	void Bind() const;

	/* Unbinds this buffer, making it the current render location no more */
	void Unbind() const;

	/* Clears the content of this buffer */
	void Clear() const;

	/* Resizes the buffer, the current content of the buffer is discraded */
	void Resize(GLint width, GLint height);

	[[nodiscard]] GLuint GetTextureID() const;

private:
	/* Creates the inner OpenGL buffers, should be used only inside of c-tor and Resize method */
	void CreateBuffers(GLint width, GLint height);

	/* Deletes the inner OpenGL buffers, should be used only inside of d-tor and Resize method */
	void DeleteBuffers();

	GLint width_;
	GLint height_;
	GLuint FBO_ = 0;
	GLuint TextureID_ = 0;
	GLuint DepthId_ = 0;
};