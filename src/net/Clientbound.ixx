module;

#include <assert.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <__ostream/basic_ostream.h>

export module Clientbound;

import Reader;
import World;
import GameServer;

export enum class ClientboundHeaders : uint8_t {
    Pong = 0,
    Init,
    Entities,
    Arena,
    Camera,
    Scoreboard,
    Attributes,
    Upgrades,
    Death,
    UI,
    Server,
    World,
    Indicators,
    Notify,
    Tanks,
    Commands,
};

export void decodeMessages(Reader &reader) {
    while (reader.bytesLeft()) {
        switch (reader.trivial<uint8_t>()) {
            case static_cast<uint8_t>(ClientboundHeaders::Pong):
                break;
            case static_cast<uint8_t>(ClientboundHeaders::Init):
                break;
            case static_cast<uint8_t>(ClientboundHeaders::Entities): {
                assert(GameServer::connected);
                const auto deletionCounts = reader.uleb128<uint32_t>();
                for (uint32_t i = 0; i < deletionCounts; ++i) {
                    if (const auto id = reader.uleb128<uint32_t>(); !GameServer::world.deleteEntity(id)) {
                        std::cout << "Tried to delete entity " << id << " that we don't know about" << std::endl;
                    }
                }
                const auto updateCount = reader.uleb128<uint32_t>();
                for (uint32_t i = 0; i < updateCount; ++i) {
                    GameServer::world.getOrCreateEntity(reader.uleb128<uint32_t>()).decode(reader);
                }
                break;
            }
            case static_cast<uint8_t>(ClientboundHeaders::Arena):
                assert(GameServer::connected);
                GameServer::world.arena.decode(reader);
                break;
            case static_cast<uint8_t>(ClientboundHeaders::Camera):
                assert(GameServer::connected);
                //GameServer::world.camera.decode(reader);
                break;
        }
    }
}