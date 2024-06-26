#pragma once
#include <memory>

#include "frame_buffer.h"

namespace render {
class PickingTexture {
   public:
    struct Info {
        unsigned int ObjectID = 0;
        unsigned int VertexId = 0;
        unsigned int Type = 0;
        bool operator==(const Info& b) const;

        struct Hash {
            size_t operator()(const Info& v) const {
                auto uihash = std::hash<unsigned int>{};
                return uihash(v.ObjectID) ^ (uihash(v.VertexId) << 1) ^ (uihash(v.Type) << 2);
            }
        };
    };

   public:
    PickingTexture(int width, int height);

    void Resize(int width, int height);

    Info ReadPixel(unsigned int x, unsigned int y);
    std::vector<Info> ReadArea(unsigned int x, unsigned int y, int width, int height);

    float ReadDepth(unsigned int x, unsigned int y);

    void Bind();
    void Unbind();

   public:
    std::unique_ptr<FrameBuffer> buffer_;
    int height_;
    int width_;

    bool binded_ = false;
};
}  // namespace render
