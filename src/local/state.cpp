#include "state.hpp"

#include <memory>
#include <unordered_map>

void LocalState::add_connection(uuid uuid_a, uuid uuid_b,
                                const Connection& con) {
    this->connections.x.insert(std::make_pair(
        uuid_a, std::unordered_map<uuid, std::shared_ptr<Connection>>()));
    this->connections.x.insert(std::make_pair(
        uuid_b, std::unordered_map<uuid, std::shared_ptr<Connection>>()));
    this->connections[uuid_a].insert_or_assign(
        uuid_b, std::make_shared<Connection>(con));
    this->connections[uuid_b].insert_or_assign(
        uuid_a, std::make_shared<Connection>(con));
}

void LocalState::delete_connection(uuid uuid_a, uuid uuid_b) {
    this->connections[uuid_a].erase(uuid_b);
    this->connections[uuid_b].erase(uuid_a);
}
