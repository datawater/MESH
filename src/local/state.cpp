#include "state.hpp"

#include "../utils.hpp"

void LocalState::add_connection(uuid uuid_a, uuid uuid_b,
                                const Connection& con) {
    UNUSED(uuid_a);
    UNUSED(uuid_b);
    UNUSED(con);
    mesh_todo("LocalState::add_connection");
}

void LocalState::remove_connection(uuid uuid_a, uuid uuid_b) {
    UNUSED(uuid_a);
    UNUSED(uuid_b);
    mesh_todo("LocalState::remove_connection");
}

void LocalState::delete_connection(uuid uuid_a, uuid uuid_b) {
    UNUSED(uuid_a);
    UNUSED(uuid_b);
    mesh_todo("LocalState::delete_connection");
}

const ConnectionGraph LocalState::get_connections() {
    // NOTE: Obfuscated away into a setter/getter as it may require
    // pre-processing
    return this->connections;
}
