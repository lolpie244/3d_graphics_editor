#include "render/opengl/picking_texture.h"

#include <iostream>
#include <stdexcept>
#include <array>

#include "render/opengl/frame_buffer.h"

namespace render {
PickingTexture::PickingTexture(int width, int height): buffer_(width, height) {
    buffer_.AttachTexture(GL_COLOR_ATTACHMENT0, FrameBuffer::RGB);
    buffer_.AttachRenderBuffer(GL_DEPTH_ATTACHMENT, FrameBuffer::DEPTH24);
    buffer_.AttachTexture(GL_COLOR_ATTACHMENT1, FrameBuffer::RED_INTEGER);
    if (!buffer_.Valid())
        throw std::runtime_error("Frame buffer is not valid");

	// buffer_.Bind();
 //    constexpr std::array<GLuint, 2> attachments { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
 //    glDrawBuffers(attachments.size(), attachments.data());
	// buffer_.Unbind(FrameBuffer::Default);
}

PickingTexture::Info PickingTexture::ReadPixel(unsigned int x, unsigned int y) {
    buffer_.Bind(FrameBuffer::Read);
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    Info pixel;
	unsigned int id;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, &id);
	std::cout << "HHHHHHERERE" << ' ' << id << '\n';

    glReadBuffer(GL_NONE);
    buffer_.Unbind(FrameBuffer::Read);
    return pixel;
}
}  // namespace render
