#pragma once

#include <unordered_set>

template <typename T>
inline int hash(const std::unordered_set<T>& set) {
	int result;
	for (auto& i : set) {
		result ^= set.hash_function()(i);
	}

	return result;
}
