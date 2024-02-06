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

	[[nodiscard]] GLuint GetTextureID() const;

private:
	GLint width_;
	GLint height_;
	GLuint FBO_;
	GLuint TextureID_;
	GLuint DepthId_;
};