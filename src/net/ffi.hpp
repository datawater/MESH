#pragma once

#include "../local/state.hpp"
#include "../utils/utils.hpp"
#include "connection.hpp"
#include "packet.hpp"
#include "types.hpp"

MESH_EXPORT Connection* Connection_Default();
MESH_EXPORT Connection* Connection_New(f32 connection_strength, u64 pk[4],
                                       u64 mac);
MESH_EXPORT Connection* Connection_Random();

MESH_EXPORT const char* Connection_ToBinary(Connection* con);
MESH_EXPORT const char* Connection_ToJson(Connection* con);
MESH_EXPORT const char* Connection_ToString(Connection* con);

///

MESH_EXPORT InitPacket* InitPacket_Default();
MESH_EXPORT InitPacket* InitPacket_New(
    u64 author[2], u64 to[2], u64 real_to[2], bool is_encrypted,
    available_connection_types_t av_con_types, platform_t platform,
    const char* name, u64 pk[4]);
MESH_EXPORT InitPacket* InitPacket_Random();

MESH_EXPORT const char* InitPacket_ToBinary(InitPacket* pak);
MESH_EXPORT const char* InitPacket_ToJson(InitPacket* pak);

///

MESH_EXPORT MessagePacket* MessagePacket_Default();
MESH_EXPORT MessagePacket* MessagePacket_New(u64 author[2], u64 to[2],
                                             u64 real_to[2], bool is_encrypted,
                                             const char* message);
MESH_EXPORT MessagePacket* MessagePacket_Random();

MESH_EXPORT const char* MessagePacket_ToBinary(MessagePacket* pak);
MESH_EXPORT const char* MessagePacket_ToJson(MessagePacket* pak);

///

MESH_EXPORT UpdatePacket* UpdatePacket_Default();
MESH_EXPORT UpdatePacket* UpdatePacket_FromLocalState(u64 author[2], u64 to[2],
                                                      u64 real_to[2],
                                                      bool is_encrypted,
                                                      LocalState* state);
MESH_EXPORT UpdatePacket* UpdatePacket_Random();

MESH_EXPORT const char* UpdatePacket_ToBinary(UpdatePacket* pak);
MESH_EXPORT const char* UpdatePacket_ToJson(UpdatePacket* pak);

///

MESH_EXPORT BroadcastHelpPacket* BroadcastHelpPacket_Default();
MESH_EXPORT BroadcastHelpPacket* BroadcastHelpPacket_FromLocalState(
    u64 author[2], u64 to[2], u64 real_to[2], bool is_encrypted,
    const char* message, f64 location_now[2]);
MESH_EXPORT UpdatePacket* BroadcastHelpPacket_Random();

MESH_EXPORT const char* BroadcastHelpPacket_ToBinary(BroadcastHelpPacket* pak);
MESH_EXPORT const char* BroadcastHelpPacket_ToJson(BroadcastHelpPacket* pak);

///