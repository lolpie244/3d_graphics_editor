#pragma once
#include <glm/ext/vector_float2.hpp>
#include <utility>

namespace settings {
constexpr float SVG_RESIZE_COEF = 1.5;  // scale, on which resize loading new SVG
constexpr glm::vec2 DEFAULT_RESOLUTION = {1920, 1080};  // scale, on which resize loading new SVG
constexpr float DEFAULT_FOV = 45;
constexpr float MOUSE_SENSATIVITY = 4.5;
}
