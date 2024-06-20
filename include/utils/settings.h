#pragma once
#include <sys/socket.h>

#include <chrono>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <utility>
#include "network/tcp_hole/events.h"

namespace settings {
using namespace std::chrono_literals;

constexpr char FONT_FILE[] = "resources/font.ttf";

constexpr float SVG_RESIZE_COEF = 1.5;                  // scale, on which resize loading new SVG
constexpr glm::vec2 DEFAULT_RESOLUTION = {1920, 1080};  // scale, on which resize loading new SVG
constexpr float DEFAULT_FOV = 45;
constexpr float MOUSE_SENSATIVITY = 4.5;

const glm::vec4 DEFAULT_POINT_COLOR = {255, 255, 255, 1};
const glm::vec4 SELECTED_POINT_COLOR = {255, 0, 0, 1};

constexpr int DYNAMIC_TRIANGULATE_LIMIT = 10;
constexpr float GIZMO_SCALE = 0.2;

constexpr char SERVER_HOST[] = "";
constexpr char SERVER_PORT[] = "2345";
constexpr auto WAIT_FOR_CONNECTION = 100ms;
constexpr int CODE_LEN = 5;

static_assert(CODE_LEN <= tcp_hole::MAX_CODE_LEN);

constexpr int MAXIMUM_LIGHT_COUNT = 8;
constexpr char FILE_FORMAT[] = "*.model";
}  // namespace settings
