#include "render/opengl/picking_texture.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include "render/opengl/frame_buffer.h"

namespace render {
PickingTexture::PickingTexture(int width, int height) : height_(height) { Resize(width, height); }

void PickingTexture::Resize(int width, int height) {
    if (!width || !height)
        return;

    buffer_ = std::make_unique<FrameBuffer>(width, height);
    height_ = height;
    width_ = width;

    buffer_->AttachTexture(GL_COLOR_ATTACHMENT0, FrameBuffer::RGB_INTEGER);
    buffer_->AttachRenderBuffer(GL_DEPTH_ATTACHMENT, FrameBuffer::DEPTH24);
    if (!buffer_->Valid())
        throw std::runtime_error("Frame buffer is not valid");
}

PickingTexture::Info PickingTexture::ReadPixel(unsigned int x, unsigned int y) {
    if (x >= width_ || y >= height_) {
        throw std::runtime_error("Out of bounds");
    }

    buffer_->Bind(FrameBuffer::Read);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    Info pixel;
    glReadPixels(x, height_ - y, 1, 1, FrameBuffer::RGB_INTEGER, GL_UNSIGNED_INT, &pixel);
    glReadBuffer(GL_NONE);
    buffer_->Unbind(FrameBuffer::Read);
    return pixel;
}

float PickingTexture::ReadDepth(unsigned int x, unsigned int y) {
    if (x >= width_ || y >= height_) {
        throw std::runtime_error("Out of bounds");
    }

    buffer_->Bind(FrameBuffer::Read);
    float depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadBuffer(GL_NONE);
    buffer_->Unbind(FrameBuffer::Read);
    return depth;
}

void PickingTexture::Bind() {
    buffer_->Bind(render::FrameBuffer::Write);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PickingTexture::Unbind() { buffer_->Unbind(render::FrameBuffer::Write); }
}  // namespace render
