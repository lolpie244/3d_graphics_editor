#pragma once
#include "frame_buffer.h"

namespace render {
class PickingTexture {
   public:
    struct Info {
        unsigned int ObjectID = -1;
        unsigned int Test = -1;
        unsigned int PrimID = -1;
    };

   public:
    PickingTexture(int width, int height);
    Info ReadPixel(unsigned int x, unsigned int y);
	void Draw();

   public:
    FrameBuffer buffer_;
};
}  // namespace render
