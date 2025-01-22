#pragma once

#include <cereal/types/vector.hpp>
#include <cstddef>
#include <cstdint>
#include <vector>

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

#define UINT_STRUCT_IMPL(n, h, s)           \
   public:                                  \
    h msb;                                  \
    h lsb;                                  \
                                            \
    template <typename T>                   \
    n(T t) {                                \
        if (sizeof(T) < s / 8) {            \
            lsb = h(t);                     \
        }                                   \
    }                                       \
                                            \
    n(h msb, h lsb) : msb(msb), lsb(lsb) {} \
    n() : msb(h(0)), lsb(h(0)) {}           \
                                            \
    template <class Archive>                \
    void serialize(Archive& ar) {           \
        ar(msb, lsb);                       \
    }                                       \
    n& operator=(const n& o) {              \
        if (this == &o) return *this;       \
        this->msb = o.msb;                  \
        this->lsb = lsb;                    \
        return *this;                       \
    }

class u128 {
    UINT_STRUCT_IMPL(u128, u64, 64)
   public:
    bool operator==(const u128& o) const {
        return this->msb == o.msb && this->lsb == o.lsb;
    }
    bool operator!=(const u128& o) const {
        return !(this->msb == o.msb && this->lsb == o.lsb);
    }

    MESH_DEBUG_FUNC static u128 random() {
        return u128(random_n<u64>(0, UINT64_MAX), random_n<u64>(0, UINT64_MAX));
    }
};

class u256 {
    UINT_STRUCT_IMPL(u256, u128, 128)
   public:
    bool operator==(const u256& o) const {
        return this->msb == o.msb && this->lsb == o.lsb;
    }
    bool operator!=(const u256& o) const {
        return !(this->msb == o.msb && this->lsb == o.lsb);
    }

    MESH_DEBUG_FUNC static u256 random() {
        return u256(u128::random(), u128::random());
    }
};

typedef u128 uuid;
typedef u256 ecc_key;
typedef char* short_str;

template <typename T>
class Matrix2d {
   public:
    std::vector<std::vector<T>> x;

    Matrix2d(std::vector<std::vector<T>> x) : x(x) {}
    Matrix2d() {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(x));
    }
};

namespace std {
template <>
struct hash<u128> {
    std::size_t operator()(const u128& key) const {
        return std::hash<u64>()(key.msb) ^ (std::hash<u64>()(key.lsb) << 1);
    }
};

template <>
struct hash<u256> {
    std::size_t operator()(const u256& key) const {
        return std::hash<u128>()(key.msb) ^ (std::hash<u128>()(key.lsb) << 1);
    }
};
}  // namespace std

using unique_void_ptr = std::unique_ptr<void, void (*)(void const*)>;

template <typename T>
auto unique_void(T* ptr) -> unique_void_ptr {
    return unique_void_ptr(ptr, [](void const* data) { UNUSED(data); });
}