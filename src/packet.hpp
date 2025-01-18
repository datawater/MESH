#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include <unordered_map>

#include "connection.hpp"
#include "types.hpp"

typedef enum packet_types_e {
    PACKET_INIT,
    // EACH PEER THIS PACKET TO EACHOTHER WHEN FIRST CONNECTING
    // SHORT STRING MESSAGE, NO MORE THAN 256 BYTES
    PACKET_MESSAGE,
    // UPDATE SEND EVERY N SECONDS. SENT TO EACH CONNECTED DEVICE
    PACKET_UPDATE,
    // PACKET THAT SAYS THIS DEVICE NEEDS HELP. IS UNENCRYPTED
    PACKET_BROADCAST_HELP,
} packet_type_t;

template <typename T>
class Packet {
   public:
    uuid author;
    uuid to;
    uuid real_to;

    packet_type_t packet_type;
    usize packet_length;

    bool is_encrypted;

    T packet;

    Packet(packet_type_t packet_type, uuid author, uuid to, uuid real_to,
           bool is_encrypted, T packet)
        : packet_type(packet_type),
          author(author),
          to(to),
          real_to(real_to),
          packet(packet),
          is_encrypted(is_encrypted),
          packet_length(0) {};

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(author), CEREAL_NVP(to), CEREAL_NVP(real_to),
           CEREAL_NVP(packet_type), CEREAL_NVP(packet_length),
           CEREAL_NVP(is_encrypted), CEREAL_NVP(packet));
    }
};

class InitPacket {
   public:
    available_connection_types_t av_con_types;
    platform_t platform;

    std::string name;

    u256 ecc_public_key;

    InitPacket(available_connection_types_t av_con_types, platform_t platform,
               std::string name, u256 ecc_public_key)
        : av_con_types(av_con_types),
          platform(platform),
          name(name),
          ecc_public_key(ecc_public_key) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(av_con_types), CEREAL_NVP(platform), CEREAL_NVP(name),
           CEREAL_NVP(ecc_public_key));
    }
};

class MessagePacket {
   public:
    std::string message;

    MessagePacket(std::string message) : message(message) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(message));
    }
};

class UpdatePacket {
   public:
    // Maybe introduce a diff based approach
    std::unordered_map<uuid, Connection> connections_to;

    UpdatePacket(std::unordered_map<uuid, Connection> connections_to)
        : connections_to(connections_to) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(connections_to));
    }
};

class BroadcastHelpPacket {
   public:
    std::string message;
    location location_now;

    BroadcastHelpPacket(std::string message, location location_now)
        : message(message), location_now(location_now) {}

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(message), CEREAL_NVP(location_now));
    }
};