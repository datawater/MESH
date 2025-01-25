#pragma once

#include <cstdint>

#include "../utils/types.hpp"
#include "../utils/utils.hpp"
#include "types.hpp"

class Connection {
   public:
    f32 connection_strength;  // (value from 0 to 1)
    ecc_key public_key;
    mac_address mac;

    Connection() : connection_strength(0.0), public_key(u256()), mac(0) {}
    Connection(f32 connection_strength, ecc_key public_key, mac_address mac)
        : connection_strength(connection_strength),
          public_key(public_key),
          mac(mac) {}

    MESH_DEBUG_FUNC static Connection random() {
        return Connection(random_n<f32>(0, 1), u256::random(),
                          random_n<u64>(0, UINT64_MAX));
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(connection_strength), CEREAL_NVP(public_key));
    }

    friend std::ostream& operator<<(std::ostream& os, const Connection& c) {
        return os << std::format(
                   "{{\n    connection_strength: {},"
                   "\n    public_key: {:x}{:x}{:x}{:x},"
                   "\n    mac: {:X}:{:X}:{:X}:{:X}\n}}",
                   c.connection_strength, c.public_key.msb.msb,
                   c.public_key.msb.lsb, c.public_key.lsb.msb,
                   c.public_key.lsb.lsb,
                   static_cast<u16>((c.mac >> 48) & 0xFFFF),
                   static_cast<u16>((c.mac >> 32) & 0xFFFF),
                   static_cast<u16>((c.mac >> 16) & 0xFFFF),
                   static_cast<u16>((c.mac & 0xFFFF)));
    }
};
