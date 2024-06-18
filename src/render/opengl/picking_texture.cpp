#include "render/opengl/picking_texture.h"

#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "render/opengl/frame_buffer.h"

namespace render {

bool PickingTexture::Info::operator==(const Info& b) const {
    return ObjectID == b.ObjectID && VertexId == b.VertexId && Type == b.Type;
}

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
    auto area = ReadArea(x, y, 1, 1);
    if (area.empty())
        return {0, 0, 0};

    return area[0];
}

std::vector<PickingTexture::Info> PickingTexture::ReadArea(unsigned int x, unsigned int y, int width, int height) {
    if (x + width >= width_ || y + height >= height_)
        return {};

    std::vector<Info> result(height * width);

    buffer_->Bind(FrameBuffer::Read);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(x, height_ - (y + height), width, height, FrameBuffer::RGB_INTEGER, GL_UNSIGNED_INT, result.data());
    glReadBuffer(GL_NONE);
    buffer_->Unbind(FrameBuffer::Read);
    return result;
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
    if (binded_) {
        std::cerr << "Bind already binded buffer";
        return;
    }

    binded_ = true;
    buffer_->Bind(render::FrameBuffer::Write);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PickingTexture::Unbind() {
    binded_ = false;
    buffer_->Unbind(render::FrameBuffer::Write);
}
}  // namespace render
