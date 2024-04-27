#pragma once

#include <GL/glew.h>

#include <vector>

namespace render {

class FrameBuffer {
   public:
    enum Mode {
		Default = GL_FRAMEBUFFER,
        Read = GL_READ_FRAMEBUFFER,
        Write = GL_DRAW_FRAMEBUFFER,
    };

    enum TextureFormat {
        RGB = GL_RGB,
        RGB_INTEGER = GL_RGB_INTEGER,
		RED_INTEGER = GL_RED_INTEGER,
    };

    enum RenderBufferType {
        DEPTH24 = GL_DEPTH_COMPONENT24,
    };

   public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind(Mode mode = Default) const;
    void Unbind(Mode mode) const;
    bool Valid() const;

	unsigned int Id() const;

    void AttachTexture(unsigned int attachment, TextureFormat type);
    void AttachRenderBuffer(unsigned int attachment, RenderBufferType type);

   private:
    std::vector<unsigned int> texture_ids_;
    std::vector<unsigned int> render_buffer_ids_;

	int width_;
	int height_;
    unsigned int id_;
};
}  // namespace render
