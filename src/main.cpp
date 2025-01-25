#include <cereal/archives/json.hpp>
#include <iostream>
#include <ranges>

#include "local/state.hpp"
#include "net/connection.hpp"
#include "net/packet.hpp"

int main(void) {
    uuid uuid_to(1, 1);
    Connection con(1.0, u256(), 0);

    InitPacket init_packet = InitPacket::random();
    UpdatePacket update_packet = UpdatePacket::random();
    MessagePacket message_packet = MessagePacket::random();

    LocalState state;
    for (const int i : std::views::iota(0, 10)) {
        UNUSED(i);
        state.add_connection(uuid::random(), uuid::random(),
                             Connection::random());
    }

    for (auto const& i : state.get_connections().x) {
        for (auto const& j : i.second) {
            if (j.second.has_value()) {
                std::cout << j.first << " " << j.second.value() << std::endl;
            } else {
                std::cout << j.first << " " << "None" << std::endl;
            }
        }
    }

    std::stringstream ss;

    {
        cereal::JSONOutputArchive archive(ss);
        archive(CEREAL_NVP(init_packet));
        archive(CEREAL_NVP(update_packet));
        archive(CEREAL_NVP(message_packet));
    }

    std::cout << ss.str() << "\n";
}
