#include <omp.h>

#include <cereal/archives/json.hpp>
#include <cmath>
#include <cstddef>
#include <iostream>

#include "local/state.hpp"
#include "net/connection.hpp"
#include "net/packet.hpp"
#include "utils/types.hpp"
#include "utils/utils.hpp"

#define N_CONS 16

uint16_t h_func(LocalState* s, uuid start, uuid end) {
    UNUSED(s);
    UNUSED(start);
    UNUSED(end);

    return 0;
}

int main(void) {
    srand(time(NULL));

    uuid uuids[N_CONS];

    // #pragma omp parallel
    for (int i = 0; i < N_CONS; i++) {
        uuids[i] = uuid::random();
    }

    LocalState state;
    for (const uuid& x : uuids) {
        for (int i = 0; i < (N_CONS >> 4) + 1; i++) {
            uuid uuid = x;

            while (uuid == x) uuid = uuids[random_n<size_t>(0, N_CONS)];

            state.add_connection(x, uuid, Connection::random());
        }
    }

    std::cerr << "digraph G {";
    auto connections = state.get_connections().x;
    for (auto const& i : connections) {
        for (auto const& j : i.second) {
            std::cerr << '"' << i.first << '"' << " -> \"" << j.first << "\";"
                      << std::endl;
        }
    }

    uuid start = uuids[random_n<size_t>(0, N_CONS)];
    uuid end = uuids[random_n<size_t>(0, N_CONS)];

    std::cerr << '"' << start << "\" [fillcolor=pink  , style=filled]; \""
              << end << "\" [fillcolor=yellow style=filled];" << "}\n\0";
    std::vector<uuid> p = state.find_shortest_path(start, end, h_func);

    for (const auto& x : p) {
        std::cout << "PATH: " << x << "\n";
    }

    return 0;
}