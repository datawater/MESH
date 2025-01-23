#pragma once

#include "../types.hpp"
#include "../net/connection.hpp"

class LocalState {
    Matrix2d<uuid, Connection> connections;
};