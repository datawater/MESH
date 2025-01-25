#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "types.hpp"
#include "utils.hpp"

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
        msb = 0;                            \
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

    friend std::ostream& operator<<(std::ostream& os, const u128& n) {
        char* x = static_cast<char*>(malloc(sizeof(char) * 256));
        memset(x, 0, sizeof(char) * 256);
        std::snprintf(x, sizeof(char) * 256, "%" PRIx64 " %" PRIx64, n.msb,
                      n.lsb);

        return os << x;
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

    friend std::ostream& operator<<(std::ostream& os, const u256& n) {
        return os << n.msb << " " << n.lsb;
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
