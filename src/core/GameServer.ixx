module;

#include <cstdint>

export module GameServer;

import World;

export namespace GameServer {
    uint16_t entities = 0;
    uint16_t entityUpdates = 0;
    uint16_t entityDeletions = 0;
    float tps = 25.0; // ticks per second
    float latency = 0.1; // one-trip latency in ms

    bool connected = false;

    World world{};

    float mspt() {
        return 1000.0f / tps;
    }

    void reset() {
        world = World{}; // todo check if this leaks mem
    }
}