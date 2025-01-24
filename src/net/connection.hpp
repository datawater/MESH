#pragma once

#include <cstdint>

#include "../types.hpp"
#include "../utils.hpp"
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
};
