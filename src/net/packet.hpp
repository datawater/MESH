#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "../utils/types.hpp"
#include "../utils/utils.hpp"
#include "connection.hpp"
#include "types.hpp"

typedef enum packet_types_e {
    PACKET_EMPTY,

    // EACH PEER THIS PACKET TO EACHOTHER WHEN FIRST CONNECTING
    PACKET_INIT,
    // SHORT STRING MESSAGE, NO MORE THAN 256 BYTES
    PACKET_MESSAGE,
    // UPDATE SEND EVERY N SECONDS. SENT TO EACH CONNECTED DEVICE
    PACKET_UPDATE,
    // PACKET THAT SAYS THIS DEVICE NEEDS HELP. IS UNENCRYPTED
    PACKET_BROADCAST_HELP
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

    PacketBase()
        : type(PACKET_EMPTY),
          author(u128()),
          to(u128()),
          real_to(u128()),
          is_encrypted(false) {};
    virtual ~PacketBase() = default;

    MESH_DEBUG_FUNC static PacketBase* random(packet_type_t type);
};

class InitPacket : PacketBase {
   public:
    available_connection_types_t av_con_types;
    platform_t platform;

    std::string name;

    u256 ecc_public_key;

    InitPacket()
        : av_con_types(0),
          platform(PLATFORM_NONE),
          name(std::string()),
          ecc_public_key(u256()) {}
    InitPacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
               available_connection_types_t av_con_types, platform_t platform,
               std::string& name, const u256& ecc_public_key)
        : PacketBase(PACKET_INIT, author, to, real_to, is_encrypted),
          av_con_types(av_con_types),
          platform(platform),
          name(name),
          ecc_public_key(ecc_public_key) {}

    MESH_DEBUG_FUNC static InitPacket random() {
        std::string name = random_string(8);
        u256 key = u256::random();

        return InitPacket(uuid::random(), uuid::random(), uuid::random(),
                          random_n<bool>(0, 1), 0,
                          static_cast<platform_t>(
                              random_n<int>(PLATFORM_IOS, PLATFORM_LINUX)),
                          name, key);
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

    MessagePacket() : message(std::string()) {}

    MessagePacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
                  const std::string& message)
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
    std::unordered_map<uuid, std::shared_ptr<Connection>> connections_diff;

    UpdatePacket() : connections_diff(std::unordered_map<uuid, std::shared_ptr<Connection>>()) {}

    UpdatePacket(uuid author, uuid to, uuid real_to, bool is_encrypted,
                 std::unordered_map<uuid, std::shared_ptr<Connection>>& connections_diff)
        : PacketBase(PACKET_UPDATE, author, to, real_to, is_encrypted),
          connections_diff(connections_diff) {}

    MESH_DEBUG_FUNC static UpdatePacket random() {
        std::unordered_map<uuid, std::shared_ptr<Connection>> connections_to =
            std::unordered_map<uuid, std::shared_ptr<Connection>>();

        return UpdatePacket(uuid::random(), uuid::random(), uuid::random(),
                            random_n<bool>(0, 1), connections_to);
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(connections_diff));
    }
};

class BroadcastHelpPacket : PacketBase {
   public:
    std::string message;
    location location_now;

    BroadcastHelpPacket()
        : message(std::string()), location_now(location(0.0, 0.0)) {}
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
