#pragma once
#include <memory>
#include "frame_buffer.h"

namespace render {
class PickingTexture {
   public:
    struct Info {
        unsigned int ObjectID = 0;
        unsigned int VertexId = 0;
        unsigned int Data = 0;
    };

   public:
    PickingTexture(int width, int height);

	void Resize(int width, int height);

    Info ReadPixel(unsigned int x, unsigned int y);
    float ReadDepth(unsigned int x, unsigned int y);

	void Bind();
	void Unbind();

   public:
	std::unique_ptr<FrameBuffer> buffer_;
	int height_;
	int width_;

	bool binded_ = false;

	std::pair<std::pair<int, int>, Info> cached_info_;
	std::pair<std::pair<int, int>, float> cached_depth_;
};
}  // namespace render
