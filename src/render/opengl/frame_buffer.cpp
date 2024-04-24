#include "render/opengl/frame_buffer.h"

#include <GL/glew.h>

namespace render {
FrameBuffer::FrameBuffer(int width, int height) : width_(width), height_(height) { glGenFramebuffers(1, &id_); }
FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id_);

    glDeleteTextures(texture_ids_.size(), texture_ids_.data());
    glDeleteRenderbuffers(render_buffer_ids_.size(), render_buffer_ids_.data());
}

void FrameBuffer::Bind(Mode mode) const { glBindFramebuffer(mode, id_); }

void FrameBuffer::Unbind(Mode mode) const { glBindFramebuffer(mode, 0); }

bool FrameBuffer::Valid() const {
    Bind(Mode::Default);
    bool is_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    Unbind(Mode::Default);
    return is_valid;
}

unsigned int FrameBuffer::Id() const { return id_; }

std::pair<unsigned int, unsigned int> TextureType(FrameBuffer::TextureFormat format) {
    switch (format) {
        case FrameBuffer::RGB: return {GL_RGB8, GL_FLOAT};
        case FrameBuffer::RGB_INTEGER: return {GL_RGB32UI, GL_UNSIGNED_INT};
        case FrameBuffer::RED_INTEGER: return {GL_R16UI, GL_UNSIGNED_SHORT};
    }
    return {0, 0};
}

void FrameBuffer::AttachTexture(unsigned int attachment, TextureFormat format) {
    texture_ids_.push_back(0);
    unsigned int& texture = texture_ids_[texture_ids_.size() - 1];

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    auto [internal_type, type] = TextureType(format);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_type, width_, height_, 0, format, type, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    Bind(Mode::Default);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    Unbind(Mode::Default);
}

void FrameBuffer::AttachRenderBuffer(unsigned int attachment, RenderBufferType type) {
    render_buffer_ids_.push_back(0);
    unsigned int& render_buffer = render_buffer_ids_[render_buffer_ids_.size() - 1];
    glGenRenderbuffers(1, &render_buffer);

    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, type, width_, height_);

    Bind(Mode::Default);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    Unbind(Mode::Default);
}

}  // namespace render
