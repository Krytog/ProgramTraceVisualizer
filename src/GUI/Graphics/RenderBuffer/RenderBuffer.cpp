#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(GLint width, GLint height): width_(width), height_(height) {
    CreateBuffers(width_, height_);
}

RenderBuffer::~RenderBuffer() {
    DeleteBuffers();
}

void RenderBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glViewport(0, 0, width_, height_);
}

void RenderBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderBuffer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint RenderBuffer::GetTextureID() const {
    return TextureID_;
}

void RenderBuffer::Resize(GLint width, GLint height) {
    DeleteBuffers();
    width_ = width;
    height_ = height;
    CreateBuffers(width, height);
}

void RenderBuffer::CreateBuffers(GLint width, GLint height) {
    glGenFramebuffers(1, &FBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

    glCreateTextures(GL_TEXTURE_2D, 1, &TextureID_);
    glBindTexture(GL_TEXTURE_2D, TextureID_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID_, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &DepthId_);
    glBindTexture(GL_TEXTURE_2D, DepthId_);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthId_, 0);

    GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(TextureID_, buffers);

    Unbind();
}

void RenderBuffer::DeleteBuffers() {
    glDeleteFramebuffers(1, &FBO_);
    glDeleteTextures(1, &TextureID_);
    glDeleteTextures(1, &DepthId_);
}

std::pair<GLint, GLint> RenderBuffer::GetSize() const {
    return { width_, height_ };
}