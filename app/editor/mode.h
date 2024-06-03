#pragma once

#include <algorithm>
#include <vector>

#include "data/shader.h"
#include "render/light.h"
#include "render/model.h"

class DrawMode {
   public:
    DrawMode();

    virtual void Draw(const render::ModelsList& models, const render::LightList& lights) = 0;
    virtual void DrawPicker(const render::ModelsList& models, const render::LightList& lights) = 0;

   protected:
    void DrawLightsPicker(const render::LightList& lights);
    void ApplyLights(data::Shader& shader, const render::LightList& lights);

   private:
    data::Shader light_shader_;
    data::Shader light_picking_shader_;
};

class TextureDraw : public DrawMode {
   public:
    TextureDraw();
    void Draw(const render::ModelsList& models, const render::LightList& lights) override;
    void DrawPicker(const render::ModelsList& models, const render::LightList& lights) override;

   private:
    data::Shader shader_;
    data::Shader picking_shader_;
};

class MixedDraw : public DrawMode {
   public:
    MixedDraw();
    void Draw(const render::ModelsList& models, const render::LightList& lights) override;
    void DrawPicker(const render::ModelsList& models, const render::LightList& lights) override;

   private:
    data::Shader shader_;
    data::Shader picking_shader_;

    data::Shader point_shader_;
    data::Shader point_picking_shader_;
};

class TransparentDraw : public DrawMode {
   public:
    TransparentDraw();
    void Draw(const render::ModelsList& models, const render::LightList& lights) override;
    void DrawPicker(const render::ModelsList& models, const render::LightList& lights) override;

   private:
    data::Shader shader_;
    data::Shader picking_shader_;
};
