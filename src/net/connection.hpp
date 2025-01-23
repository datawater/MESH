#pragma once

#include "../types.hpp"
#include "../utils.hpp"

typedef u8 available_connection_types_t;
enum connection_type_e {
    CONNECITON_WIFI = 1 << 0,
    CONNECTION_BLTH = 1 << 1,
};

typedef enum platform_e { PLATFORM_IOS, PLATFORM_ANDROID, PLATFORM_LINUX } platform_t;

class Connection {
   public:
    f32 connection_strength;  // (value from 0 to 1)
    ecc_key public_key;

    Connection() : connection_strength(0.0), public_key(u256()) {}
    Connection(f32 connection_strength, ecc_key public_key)
        : connection_strength(connection_strength), public_key(public_key) {}

    MESH_DEBUG_FUNC static Connection random() {
        return Connection(random_n<f32>(0, 1), u256::random());
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(connection_strength), CEREAL_NVP(public_key));
    }
};