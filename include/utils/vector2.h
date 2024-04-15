#pragma once

#include <SFML/System/Vector2.hpp>
#include <initializer_list>
#include <ostream>
#include <utility>

namespace utils {
template <typename T>
class Vector2 : public sf::Vector2<T> {
   public:

	using sf::Vector2<T>::Vector2;
    Vector2(sf::Vector2f data) : utils::Vector2<T>(data.x, data.y) {}

    Vector2<T> operator*(Vector2<T> b) const { return Vector2<T>(this->x * b.x, this->y * b.y); }
    Vector2<T> operator/(Vector2<T> b) const { return Vector2<T>(this->x / b.x, this->y / b.y); }

    Vector2<T> operator*(std::pair<T, T> b) const { return Vector2<T>(this->x * b.first, this->y * b.second); }
    Vector2<T> operator/(std::pair<T, T> b) const { return Vector2<T>(this->x / b.first, this->y / b.second); }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vector2<T>& vector)
{
	out << vector.x << ' ' << vector.y;
	return out;
}

typedef Vector2<float> Vector2f;
}  // namespace utils
