#pragma once

#include "../utils/types.hpp"

typedef u8 available_connection_types_t;
enum connection_type_e {
    CONNECITON_WIFI_DIRECT_HOST = 1 << 0,
    CONNECITON_WIFI_DIRECT_CLIENT = 1 << 1,
    CONNECTION_BLE = 1 << 2,
    CONNECTION_BLTH_5 = 1 << 3,
};

typedef enum platform_e {
    PLATFORM_IOS,
    PLATFORM_ANDROID,
    PLATFORM_LINUX
} platform_t;

typedef struct location {
   public:
    f32 longitude;
    f32 latitude;

    location(f32 longitude, f32 latitude)
        : longitude(longitude), latitude(latitude) {}

    MESH_DEBUG_FUNC static location random() {
        return location(random_n<f64>(-180, 180), random_n<f64>(-90, 90));
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(longitude), CEREAL_NVP(latitude));
    }
} location;

typedef u64 mac_address;
