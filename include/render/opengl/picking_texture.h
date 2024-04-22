#pragma once
#include "frame_buffer.h"

namespace render {
class PickingTexture {
   public:
    struct Info {
        unsigned int ObjectID = 0;
        unsigned int VertexId = 0;
        unsigned int PrimID = 0;
    };

   public:
    PickingTexture(int width, int height);
    Info ReadPixel(unsigned int x, unsigned int y);
	void Draw();

   public:
    FrameBuffer buffer_;
};
}  // namespace render
