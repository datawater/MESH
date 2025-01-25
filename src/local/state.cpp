#include "state.hpp"

#include <unordered_map>

void LocalState::add_connection(uuid uuid_a, uuid uuid_b,
                                const Connection& con) {
    this->connections.x.insert(std::make_pair(
        uuid_a, std::unordered_map<uuid, std::optional<Connection>>()));
    this->connections.x.insert(std::make_pair(
        uuid_b, std::unordered_map<uuid, std::optional<Connection>>()));
    this->connections[uuid_a].insert_or_assign(uuid_b, std::optional(con));
    this->connections[uuid_b].insert_or_assign(uuid_a, std::optional(con));
}

void LocalState::remove_connection(uuid uuid_a, uuid uuid_b) {
    this->connections[uuid_a][uuid_b] = std::nullopt;
    this->connections[uuid_b][uuid_a] = std::nullopt;
}

void LocalState::delete_connection(uuid uuid_a, uuid uuid_b) {
    this->connections[uuid_a].erase(uuid_b);
    this->connections[uuid_b].erase(uuid_a);
}

const ConnectionGraph LocalState::get_connections() {
    // NOTE: Obfuscated away into a setter/getter as it may require
    // pre-processing
    return this->connections;
}
