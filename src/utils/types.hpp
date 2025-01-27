#pragma once

#include <cereal/types/vector.hpp>
#include <unordered_map>

#include "int.hpp"
#include "utils.hpp"

typedef u128 uuid;
typedef u256 ecc_key;

template <typename K, typename V>
class Matrix2d {
   public:
    std::unordered_map<K, std::unordered_map<K, V>> x;

    Matrix2d(std::unordered_map<K, std::unordered_map<K, V>>& x) : x(x) {}
    Matrix2d() {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(x);
    }

    V& operator[](const std::pair<K, K>& k) {
        return this->x[k.first][k.second];
    }

    std::unordered_map<K, V>& operator[](const K& k) { return this->x[k]; }
};

using unique_void_ptr = std::unique_ptr<void, void (*)(void const*)>;

template <typename T>
auto unique_void(T* ptr) -> unique_void_ptr {
    return unique_void_ptr(ptr, [](void const* data) { UNUSED(data); });
}
