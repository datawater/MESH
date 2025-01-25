#pragma once

#include <cereal/types/vector.hpp>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "utils.hpp"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t usize;

typedef float f32;
typedef double f64;

#include "int.hpp"

typedef u128 uuid;
typedef u256 ecc_key;
typedef char* short_str;

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

    V& operator[](std::pair<K, K>& k) { return this->x[k.first][k.second]; }

    std::unordered_map<K, V>& operator[](K& k) { return this->x[k]; }
};

using unique_void_ptr = std::unique_ptr<void, void (*)(void const*)>;

template <typename T>
auto unique_void(T* ptr) -> unique_void_ptr {
    return unique_void_ptr(ptr, [](void const* data) { UNUSED(data); });
}
