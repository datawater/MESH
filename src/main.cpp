#include <cereal/archives/json.hpp>
#include <iostream>
#include <unordered_map>

#include "net/connection.hpp"
#include "net/packet.hpp"

int main(void) {
    uuid uuid_to(1, 1);
    Connection con(1.0, u256(), 0);

    InitPacket init_packet = InitPacket::random();
    UpdatePacket update_packet = UpdatePacket::random();
    MessagePacket message_packet = MessagePacket::random();

    std::stringstream ss;

    {
        cereal::JSONOutputArchive archive(ss);
        archive(CEREAL_NVP(init_packet));
        archive(CEREAL_NVP(update_packet));
        archive(CEREAL_NVP(message_packet));
    }

    std::cout << ss.str() << "\n";
}
