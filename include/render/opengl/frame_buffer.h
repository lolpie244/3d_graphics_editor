#pragma once

namespace render {

class FrameBuffer {
   public:
    enum class TextureFormat {
        None = 0,
        // color
        RGBA8,
        // depth
        DEPTH24STENCIL8,
        // defaults
        Depth = DEPTH24STENCIL8
    };

   public:
    FrameBuffer(int width, int height);
    void Resize(int width, int height);

   private:
    unsigned int id_;
};
}  // namespace render
