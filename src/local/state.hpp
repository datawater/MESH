#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "../net/connection.hpp"
#include "../utils/types.hpp"
#include "event.hpp"

using ConnectionGraph = Matrix2d<uuid, std::shared_ptr<Connection>>;

class LocalState {
   public:
    using MeshHandlingFunction =
        std::function<void(LocalState*, std::vector<u8>)>;
    using AstarHeuristicFunction =
        std::function<u16(LocalState*, const uuid&, const uuid&)>;

    void add_connection(uuid uuid_a, uuid uuid_b, const Connection& con);
    void delete_connection(uuid uuid_a, uuid uuid_b);

    ConnectionGraph get_connections(void) const { return this->connections; }

    std::vector<uuid> find_shortest_path(const uuid& start, const uuid& goal,
                                         AstarHeuristicFunction);

    inline void handle_event(event e, std::vector<u8> data) {
        this->event_handeling_functions[e](this, data);
    }

    inline void change_handeling_function(event e,
                                          MeshHandlingFunction function_ptr) {
        this->event_handeling_functions[e] = function_ptr;
    }

    LocalState() {};

   private:
    ConnectionGraph connections;
    std::unordered_map<event, MeshHandlingFunction> event_handeling_functions;
};
