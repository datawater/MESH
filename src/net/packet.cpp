// #include "packet.hpp"

// #include "../types.hpp"

// template <typename T>
// MESH_DEBUG_FUNC Packet<T> Packet<T>::random() {
//     packet_type_t type =
//         std::is_same<T, InitPacket>::value            ? PACKET_INIT
//         : std::is_same<T, MessagePacket>::value       ? PACKET_MESSAGE
//         : std::is_same<T, UpdatePacket>::value        ? PACKET_UPDATE
//         : std::is_same<T, BroadcastHelpPacket>::value ? PACKET_BROADCAST_HELP
//                                                       :
//                                                       PACKET_BROADCAST_HELP;

//     return Packet(type, uuid::random(), uuid::random(), uuid::random(),
//                   random_n<bool>(0, 1), T::random());
// }

// template Packet<InitPacket> Packet<InitPacket>::random();
// template Packet<MessagePacket> Packet<MessagePacket>::random();
// template Packet<UpdatePacket> Packet<UpdatePacket>::random();
// template Packet<BroadcastHelpPacket> Packet<BroadcastHelpPacket>::random();
