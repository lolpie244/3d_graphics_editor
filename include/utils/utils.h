#pragma once

#include <SFML/Graphics/Color.hpp>
#include <glm/ext/vector_float4.hpp>
#include <unordered_set>

template <typename T>
inline int hash(const std::unordered_set<T>& set) {
    int result;
    for (auto& i : set) { result ^= set.hash_function()(i); }

    return result;
}

inline glm::vec4 ToVector(sf::Color color) { return {color.r / 255, color.g / 255, color.b / 255, color.a / 255}; }
