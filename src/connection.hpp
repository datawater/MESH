#pragma once

#include "types.hpp"

typedef struct location {
   public:
    f32 longitude;
    f32 latitude;

    location(f32 longitude, f32 latitude)
        : longitude(longitude), latitude(latitude) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(longitude), CEREAL_NVP(latitude));
    }
} location;

typedef u8 available_connection_types_t;
enum connection_type_e {
    CONNECITON_WIFI = 1 << 0,
    CONNECTION_BLTH = 1 << 1,
};

typedef enum platform_e { IOS, ANDROID, LINUX } platform_t;

class Connection {
   public:
    f32 connection_strength;  // (value from 0 to 1)
    ecc_key public_key;

    Connection() : connection_strength(0.0), public_key(u256()) {}
    Connection(f32 connection_strength, ecc_key public_key)
        : connection_strength(connection_strength), public_key(public_key) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(connection_strength), CEREAL_NVP(public_key));
    }
};