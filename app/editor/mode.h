#pragma once

#include <vector>

#include "render/model.h"


class DrawMode {
   public:
    virtual void Draw(const render::ModelsList& models) = 0;
    virtual void DrawPicker(const render::ModelsList& models) = 0;
};

class TextureDraw : public DrawMode {
   public:
    TextureDraw();
    void Draw(const render::ModelsList& models) override;
    void DrawPicker(const render::ModelsList& models) override;

   private:
    data::Shader shader_;
    data::Shader picking_shader_;
};

class MixedDraw : public DrawMode {
   public:
    MixedDraw();
    void Draw(const render::ModelsList& models) override;
    void DrawPicker(const render::ModelsList& models) override;

   private:
    data::Shader shader_;
    data::Shader point_shader_;
    data::Shader picking_shader_;
};

class TransparentDraw : public DrawMode {
   public:
    TransparentDraw();
    void Draw(const render::ModelsList& models) override;
    void DrawPicker(const render::ModelsList& models) override;

   private:
    data::Shader shader_;
    data::Shader picking_shader_;
};
