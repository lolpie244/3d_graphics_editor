#include "mode.h"

#include "stage/stage_manager.h"

DrawMode::DrawMode() {
    light_shader_.loadFromFile("shaders/color.vert", "shaders/color.frag");
    light_picking_shader_.loadFromFile("shaders/color.vert", "shaders/picking.frag");
}

void DrawMode::ApplyLights(data::Shader& shader, const render::LightList& lights) {
    int i = 0;
    for (auto& [_, light] : lights) {
        light->Apply(i++, shader);
        light->Draw(light_shader_);
    }
    shader.setUniform("lightsNumber", i);
    shader.setUniform("viewPosition", stage::StageManager::Instance().Camera()->GetRealPosition());
}

void DrawMode::DrawLightsPicker(const render::LightList& lights) {
    for (auto& [_, light] : lights) { light->Draw(light_picking_shader_); }
}

TextureDraw::TextureDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

MixedDraw::MixedDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    point_shader_.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking_empty.frag");
    point_picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

TransparentDraw::TransparentDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

void TextureDraw::Draw(const render::ModelsList& models, const render::LightList& lights) {
    ApplyLights(shader_, lights);
    for (auto& [_, model] : models) { model->Draw(shader_); }
}

void TextureDraw::DrawPicker(const render::ModelsList& models, const render::LightList& lights) {
    DrawLightsPicker(lights);
    for (auto& [_, model] : models) { model->Draw(picking_shader_); }
}

void MixedDraw::Draw(const render::ModelsList& models, const render::LightList& lights) {
    ApplyLights(shader_, lights);
    for (auto& [_, model] : models) {
        model->Draw(shader_);
        model->DrawPoints(point_shader_);
    }
}

void MixedDraw::DrawPicker(const render::ModelsList& models, const render::LightList& lights) {
    DrawLightsPicker(lights);
    for (auto& [_, model] : models) {
        model->Draw(picking_shader_);
        model->DrawPoints(point_picking_shader_);
    }
}

void TransparentDraw::Draw(const render::ModelsList& models, const render::LightList& lights) {
    for (auto& [_, model] : models) { model->DrawPoints(shader_); }
}

void TransparentDraw::DrawPicker(const render::ModelsList& models, const render::LightList& lights) {
    for (auto& [_, model] : models) { model->DrawPoints(picking_shader_); }
}
