#include <cereal/archives/json.hpp>
#include <iostream>
#include <unordered_map>

#include "connection.hpp"
#include "packet.hpp"

int main(void) {
    available_connection_types_t av_con_types =
        CONNECITON_WIFI | CONNECTION_BLTH;
    platform_t platform = LINUX;
    uuid uuid_to(1, 1);
    Connection con(1.0, u256());

    std::unordered_map<uuid, Connection> connections_to;
    connections_to[uuid_to] = con;

    InitPacket init_packet_base(av_con_types, platform, "VROOMVROOM", u256());
    UpdatePacket update_packet_base(connections_to);

    Packet<InitPacket> init_packet(PACKET_INIT, uuid(), uuid(), uuid(), false,
                                   init_packet_base);

    Packet<UpdatePacket> update_packet(PACKET_UPDATE, uuid(), uuid(), uuid(),
                                       false, update_packet_base);

    std::stringstream ss;

    {
        cereal::JSONOutputArchive archive(ss);
        archive(CEREAL_NVP(init_packet));
        archive(CEREAL_NVP(update_packet));
    }

    std::cout << ss.str() << "\n";
}