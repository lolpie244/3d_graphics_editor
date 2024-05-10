#include "mode.h"

TextureDraw::TextureDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

MixedDraw::MixedDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    point_shader_.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

TransparentDraw::TransparentDraw() {
    shader_.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader_.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
}

void TextureDraw::Draw(const render::ModelsList& models) {
    for (auto& [_, model] : models) { model->Draw(shader_); }
}

void TextureDraw::DrawPicker(const render::ModelsList& models) {
    for (auto& [_, model] : models) { model->Draw(picking_shader_); }
}

void MixedDraw::Draw(const render::ModelsList& models) {
    for (auto& [_, model] : models) {
        model->Draw(shader_);
        model->DrawPoints(point_shader_);
    }
}

void MixedDraw::DrawPicker(const render::ModelsList& models) {
    for (auto& [_, model] : models) { model->DrawPoints(picking_shader_); }
}

void TransparentDraw::Draw(const render::ModelsList& models) {
    for (auto& [_, model] : models) { model->DrawPoints(shader_); }
}

void TransparentDraw::DrawPicker(const render::ModelsList& models) {
    for (auto& [_, model] : models) { model->DrawPoints(picking_shader_); }
}
