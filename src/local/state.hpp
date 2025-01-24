#pragma once

#include <functional>
#include <unordered_map>

#include "../net/connection.hpp"
#include "../types.hpp"
#include "event.hpp"

using ConnectionGraph = Matrix2d<uuid, Connection>;

class LocalState {
   public:
    using MeshHandlingFunction =
        std::function<void(LocalState*, std::vector<u8>)>;

    static void add_connection(uuid uuid_a, uuid uuid_b, const Connection& con);
    static void remove_connection(uuid uuid_a, uuid uuid_b);
    static void delete_connection(uuid uuid_a, uuid uuid_b);
    ConnectionGraph const get_connections();

    inline void handle_event(event e, std::vector<u8> data) {
        this->event_handeling_functions[e](this, data);
    }

    inline void change_handeling_function(event e,
                                          MeshHandlingFunction function_ptr) {
        this->event_handeling_functions[e] = function_ptr;
    }

    LocalState();

   private:
    ConnectionGraph connections;
    std::unordered_map<event, MeshHandlingFunction> event_handeling_functions;
};
