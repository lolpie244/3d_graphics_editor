#pragma once
#include <sys/socket.h>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <utility>

namespace settings {
constexpr char FONT_FILE[] = "resources/font.ttf";

constexpr float SVG_RESIZE_COEF = 1.5;                  // scale, on which resize loading new SVG
constexpr glm::vec2 DEFAULT_RESOLUTION = {1920, 1080};  // scale, on which resize loading new SVG
constexpr float DEFAULT_FOV = 45;
constexpr float MOUSE_SENSATIVITY = 4.5;

const glm::vec4 DEFAULT_POINT_COLOR = {255, 255, 255, 1};
const glm::vec4 SELECTED_POINT_COLOR = {255, 0, 0, 1};

constexpr int DYNAMIC_TRIANGULATE_LIMIT = 10;
constexpr float GIZMO_SCALE = 0.2;

constexpr int PACKAGE_SIZE = 1000;
constexpr char PORT[] = "2341";
constexpr int INET_FAMILY = AF_INET6;

constexpr int MAXIMUM_LIGHT_COUNT = 8;
constexpr char FILE_FORMAT[] = ".help";
}  // namespace settings
