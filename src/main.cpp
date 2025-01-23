#include <cereal/archives/json.hpp>

#include <iostream>
#include <unordered_map>

#include "net/connection.hpp"
#include "net/packet.hpp"

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

    Packet<InitPacket> init_packet = Packet<InitPacket>::random();
    Packet<UpdatePacket> update_packet = Packet<UpdatePacket>::random();
    Packet<MessagePacket> message_packet = Packet<MessagePacket>::random();

    std::stringstream ss;

    {
        cereal::JSONOutputArchive archive(ss);
        archive(CEREAL_NVP(init_packet));
        archive(CEREAL_NVP(update_packet));
        archive(CEREAL_NVP(message_packet));
    }

    std::cout << ss.str() << "\n";
}