#include "render/opengl/picking_texture.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include "render/opengl/frame_buffer.h"

namespace render {
PickingTexture::PickingTexture(int width, int height) : buffer_(width, height) {
    buffer_.AttachTexture(GL_COLOR_ATTACHMENT0, FrameBuffer::RGB_INTEGER);
    buffer_.AttachRenderBuffer(GL_DEPTH_ATTACHMENT, FrameBuffer::DEPTH24);
    if (!buffer_.Valid())
        throw std::runtime_error("Frame buffer is not valid");
}

PickingTexture::Info PickingTexture::ReadPixel(unsigned int x, unsigned int y) {
    buffer_.Bind(FrameBuffer::Read);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    Info pixel;
    glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);
    glReadBuffer(GL_NONE);
    buffer_.Unbind(FrameBuffer::Read);
    return pixel;
}
}  // namespace render
