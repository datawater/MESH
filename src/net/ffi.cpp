#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/details/helpers.hpp>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#include "../local/state.hpp"
#include "../utils/utils.hpp"
#include "connection.hpp"
#include "packet.hpp"
#include "types.hpp"

#define U128(x) (u128((x)[0], (x)[1]))
#define U256(x) (u256(u128((x)[0], (x)[1]), u128((x)[2], (x)[3])))

#define SERIALIZE(archiver, item)                             \
    std::stringstream ss;                                     \
    {                                                         \
        cereal::archiver archive(ss);                         \
        archive(CEREAL_NVP(*item));                           \
    }                                                         \
    size_t len = ss.str().length() + 1;                       \
    char* s = static_cast<char*>(malloc(sizeof(char) * len)); \
    memset(s, 0, len);                                        \
    memcpy(s, ss.str().c_str(), sizeof(char) * len);          \
    return s

#define TO_STRING(item)                                       \
    std::stringstream ss;                                     \
    ss << *item;                                              \
    size_t len = ss.str().length() + 1;                       \
    char* s = static_cast<char*>(malloc(sizeof(char) * len)); \
    memset(s, 0, len);                                        \
    memcpy(s, ss.str().c_str(), sizeof(char) * len);          \
    return s

MESH_EXPORT Connection* Connection_Default() { return new Connection(); }

MESH_EXPORT Connection* Connection_New(f32 connection_strength, u64 pk[4],
                                       u64 mac) {
    return new Connection(connection_strength, U256(pk), mac);
}

MESH_EXPORT Connection* Connection_Random() {
    return new Connection(random_n<f32>(0, 1), u256::random(),
                          random_n<u64>(0, UINT64_MAX));
}

MESH_EXPORT const char* Connection_ToBinary(Connection* con) {
    SERIALIZE(PortableBinaryOutputArchive, con);
}

MESH_EXPORT const char* Connection_ToJson(Connection* con) {
    SERIALIZE(JSONOutputArchive, con);
}

MESH_EXPORT const char* Connection_ToString(const Connection* con) {
    TO_STRING(con);
}

///

MESH_EXPORT InitPacket* InitPacket_Default() { return new InitPacket(); }

MESH_EXPORT InitPacket* InitPacket_New(
    u64 author[2], u64 to[2], u64 real_to[2], bool is_encrypted,
    available_connection_types_t av_con_types, platform_t platform,
    const char* name, u64 pk[4]) {
    std::string x(name);

    return new InitPacket(U128(author), U128(to), U128(real_to), is_encrypted,
                          av_con_types, platform, x, U256(pk));
}

MESH_EXPORT InitPacket* InitPacket_Random() {
    std::string name = random_string(8);
    u256 key = u256::random();

    return new InitPacket(
        uuid::random(), uuid::random(), uuid::random(), random_n<bool>(0, 1), 0,
        static_cast<platform_t>(random_n<int>(PLATFORM_IOS, PLATFORM_LINUX)),
        name, key);
}

MESH_EXPORT const char* InitPacket_ToBinary(InitPacket* pak) {
    SERIALIZE(PortableBinaryOutputArchive, pak);
}

MESH_EXPORT const char* InitPacket_ToJson(InitPacket* pak) {
    SERIALIZE(JSONOutputArchive, pak);
}

///

MESH_EXPORT MessagePacket* MessagePacket_Default() {
    return new MessagePacket();
}

MESH_EXPORT MessagePacket* MessagePacket_New(u64 author[2], u64 to[2],
                                             u64 real_to[2], bool is_encrypted,
                                             const char* message) {
    std::string s(message);
    return new MessagePacket(U128(author), U128(to), U128(real_to),
                             is_encrypted, s);
}

MESH_EXPORT MessagePacket* MessagePacket_Random() {
    std::string name = random_string(8);

    return new MessagePacket(uuid::random(), uuid::random(), uuid::random(),
                             random_n<bool>(0, 1), name);
}

MESH_EXPORT const char* MessagePacket_ToBinary(MessagePacket* pak) {
    SERIALIZE(PortableBinaryOutputArchive, pak);
}

MESH_EXPORT const char* MessagePacket_ToJson(MessagePacket* pak) {
    SERIALIZE(JSONOutputArchive, pak);
}

///

MESH_EXPORT UpdatePacket* UpdatePacket_Default() { return new UpdatePacket(); }

MESH_EXPORT UpdatePacket* UpdatePacket_FromLocalState(u64 author[2], u64 to[2],
                                                      u64 real_to[2],
                                                      bool is_encrypted,
                                                      LocalState* state) {
    return new UpdatePacket(U128(author), U128(to), U128(real_to), is_encrypted,
                            state->get_connections()[U128(author)]);
}

MESH_EXPORT UpdatePacket* UpdatePacket_Random() {
    auto x = std::unordered_map<uuid, std::shared_ptr<Connection>>();
    return new UpdatePacket(uuid::random(), uuid::random(), uuid::random(),
                            random_n<bool>(0, 1), x);
}

MESH_EXPORT const char* UpdatePacket_ToBinary(UpdatePacket* pak) {
    SERIALIZE(PortableBinaryInputArchive, pak);
}

MESH_EXPORT const char* UpdatePacket_ToJson(UpdatePacket* pak) {
    SERIALIZE(JSONOutputArchive, pak);
}

///

MESH_EXPORT BroadcastHelpPacket* BroadcastHelpPacket_Default() {
    return new BroadcastHelpPacket();
}

MESH_EXPORT BroadcastHelpPacket* BroadcastHelpPacket_FromLocalState(
    u64 author[2], u64 to[2], u64 real_to[2], bool is_encrypted,
    const char* message, f64 location_now[2]) {
    return new BroadcastHelpPacket(U128(author), U128(to), U128(real_to),
                                   is_encrypted, std::string(message),
                                   location(location_now[0], location_now[1]));
}

MESH_EXPORT BroadcastHelpPacket* BroadcastHelpPacket_Random() {
    return new BroadcastHelpPacket(
        uuid::random(), uuid::random(), uuid::random(), random_n<bool>(0, 1),
        std::string(),
        location(random_n<f64>(-90, 90), random_n<f64>(-180, 180)));
}

MESH_EXPORT const char* BroadcastHelpPacket_ToBinary(BroadcastHelpPacket* pak) {
    SERIALIZE(PortableBinaryOutputArchive, pak);
}

MESH_EXPORT const char* BroadcastHelpPacket_ToJson(BroadcastHelpPacket* pak) {
    SERIALIZE(JSONOutputArchive, pak);
}