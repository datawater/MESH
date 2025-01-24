#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include <unordered_map>

#include "../types.hpp"
#include "../utils.hpp"
#include "connection.hpp"
#include "types.hpp"

typedef enum packet_types_e {
    // EACH PEER THIS PACKET TO EACHOTHER WHEN FIRST CONNECTING
    PACKET_INIT,
    // SHORT STRING MESSAGE, NO MORE THAN 256 BYTES
    PACKET_MESSAGE,
    // UPDATE SEND EVERY N SECONDS. SENT TO EACH CONNECTED DEVICE
    PACKET_UPDATE,
    // PACKET THAT SAYS THIS DEVICE NEEDS HELP. IS UNENCRYPTED
    PACKET_BROADCAST_HELP,
} packet_type_t;

class PacketBase {
   public:
    uuid author;
    uuid to;
    uuid real_to;
    packet_type_t type;
    bool is_encrypted;

    PacketBase(packet_type_t type, uuid author, uuid to, uuid real_to,
               bool is_encrypted)
        : type(type),
          author(author),
          to(to),
          real_to(real_to),
          is_encrypted(is_encrypted) {}

    virtual ~PacketBase() = default;

    template <class Archive>
    void serialize(Archive& ar);

    MESH_DEBUG_FUNC static PacketBase* random(packet_type_t type);
};

class InitPacket : PacketBase {
   public:
    available_connection_types_t av_con_types;
    platform_t platform;

    std::string name;

    u256 ecc_public_key;

    InitPacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
               available_connection_types_t av_con_types, platform_t platform,
               std::string name, u256 ecc_public_key)
        : PacketBase(PACKET_INIT, author, to, real_to, is_encrypted),
          av_con_types(av_con_types),
          platform(platform),
          name(name),
          ecc_public_key(ecc_public_key) {}

    MESH_DEBUG_FUNC static InitPacket random() {
        return InitPacket(uuid::random(), uuid::random(), uuid::random(),
                          random_n<bool>(0, 1), 0,
                          static_cast<platform_t>(
                              random_n<int>(PLATFORM_IOS, PLATFORM_LINUX)),
                          random_string(8), u256::random());
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(av_con_types),
           CEREAL_NVP(platform), CEREAL_NVP(name), CEREAL_NVP(ecc_public_key));
    }
};

class MessagePacket : PacketBase {
   public:
    std::string message;

    MessagePacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
                  std::string message)
        : PacketBase(PACKET_MESSAGE, author, to, real_to, is_encrypted),
          message(message) {}

    MESH_DEBUG_FUNC static MessagePacket random() {
        return MessagePacket(uuid::random(), uuid::random(), uuid::random(),
                             random_n<bool>(0, 1), random_string(8));
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(message));
    }
};

class UpdatePacket : PacketBase {
   public:
    // Maybe introduce a diff based approach
    std::unordered_map<uuid, Connection> connections_to;

    UpdatePacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
                 std::unordered_map<uuid, Connection> connections_to)
        : PacketBase(PACKET_UPDATE, author, to, real_to, is_encrypted),
          connections_to(connections_to) {}

    MESH_DEBUG_FUNC static UpdatePacket random() {
        return UpdatePacket(uuid::random(), uuid::random(), uuid::random(),
                            random_n<bool>(0, 1),
                            std::unordered_map<uuid, Connection>());
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(connections_to));
    }
};

class BroadcastHelpPacket : PacketBase {
   public:
    std::string message;
    location location_now;

    BroadcastHelpPacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
                        std::string message, location location_now)
        : PacketBase(PACKET_BROADCAST_HELP, author, to, real_to, is_encrypted),
          message(message),
          location_now(location_now) {}

    MESH_DEBUG_FUNC static BroadcastHelpPacket random() {
        return BroadcastHelpPacket(uuid::random(), uuid::random(),
                                   uuid::random(), random_n<bool>(0, 1),
                                   random_string(8), location::random());
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(message),
           CEREAL_NVP(location_now));
    }
};
